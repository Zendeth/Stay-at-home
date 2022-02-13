#include "encoder.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "fromnayuki.h"

#define QR_QUIET_ZONE 4															// size of quiet zone in modules
#define MAX_QR_FORMAT 9															// maximum QR code format that we support
#define QR_SIDE(v) ((v)*4+17)												// side of QR code in modules, not including quiet zone
#define QR_BYTSIZ(v) ((QR_SIDE(v)*QR_SIDE(v)+15)>>3)	// size of QR code representation in bytes; includes version as first byte

#define MYMAX(x,y) ((x)>(y)?(x):(y))

static const uint8_t qr_maxbyt[] = { 14,26,42,62,84,106,122,152,180 }; // maximum byte length vs version, base 1
static_assert(sizeof(qr_maxbyt) == MAX_QR_FORMAT, "check qr_maxbyt vs MAX_QR_FORMAT");

#if 0

// For generating error correction codes.
static const uint8_t qr_ecccwb[] = { 10, 16, 26, 18, 24, 16, 18, 22, 22 };
static_assert(sizeof(qr_ecccwb) == MAX_QR_FORMAT, "check qr_ecccwb vs MAX_QR_FORMAT");
#define qrcodegen_REED_SOLOMON_DEGREE_MAX 26  // highest value in the above array

// Data modules except(C) from Table 1 — Codeword capacity, divided by 16
static const uint8_t qr_tbl1D4[] = { 13, 22, 35, 50, 67, 86, 98, 121, 146 };
static_assert(sizeof(qr_tbl1D4) == MAX_QR_FORMAT, "check qr_tbl1D vs MAX_QR_FORMAT");

// Number of error correction blocks
static const uint8_t qr_numeccb[] = { 1, 1, 1, 2, 2, 4, 4, 4, 5 };
static_assert(sizeof(qr_numeccb) == MAX_QR_FORMAT, "check qr_numeccb vs MAX_QR_FORMAT");


// Appends error correction bytes to each block of the given data array, then interleaves
// bytes from the blocks and stores them in the result array. data[0 : dataLen] contains
// the input data. data[dataLen : rawCodewords] is used as a temporary work area and will
// be clobbered by this function. The final answer is stored in result[0 : rawCodewords].
void myaddEccAndInterleave(int fmtidx, uint8_t data[], uint8_t result[]) {
	// Calculate parameter numbers
	int numBlocks = qr_numeccb[fmtidx];
	int blockEccLen = qr_ecccwb[fmtidx];
	int rawCodewords = qr_tbl1D4[fmtidx] << 1;
	int dataLen = rawCodewords - blockEccLen * numBlocks;
	int numShortBlocks = numBlocks - rawCodewords % numBlocks;
	int shortBlockDataLen = rawCodewords / numBlocks - blockEccLen;

	// Split data into blocks, calculate ECC, and interleave
	// (not concatenate) the bytes into a single sequence
	uint8_t rsdiv[qrcodegen_REED_SOLOMON_DEGREE_MAX];
	reedSolomonComputeDivisor(blockEccLen, rsdiv);
	const uint8_t* dat = data;
	for (int i = 0; i < numBlocks; i++) {
		int datLen = shortBlockDataLen + (i < numShortBlocks ? 0 : 1);
		uint8_t* ecc = &data[dataLen];  // Temporary storage
		reedSolomonComputeRemainder(dat, datLen, rsdiv, blockEccLen, ecc);
		for (int j = 0, k = i; j < datLen; j++, k += numBlocks) {  // Copy data
			if (j == shortBlockDataLen)
				k -= numShortBlocks;
			result[k] = dat[j];
		}
		for (int j = 0, k = dataLen + i; j < blockEccLen; j++, k += numBlocks)  // Copy ECC
			result[k] = ecc[j];
		dat += datLen;
	}
}
#endif

// much of thos code comes from Project Nayuki
static void myencode(uint8_t qrcode[], int version) {
	enum qrcodegen_Ecc ecl = qrcodegen_Ecc_MEDIUM;
	enum qrcodegen_Mask mask = qrcodegen_Mask_AUTO;
	uint8_t tempBuffer[QR_BYTSIZ(MAX_QR_FORMAT)];
	// Compute ECC, draw modules
	addEccAndInterleave(qrcode, version, ecl, tempBuffer);
	initializeFunctionModules(version, qrcode);
	drawCodewords(tempBuffer, getNumRawDataModules(version) / 8, qrcode);
	drawLightFunctionModules(qrcode, version);
	initializeFunctionModules(version, tempBuffer);

	// Do masking
	if (mask == qrcodegen_Mask_AUTO) {  // Automatically choose best mask
		long minPenalty = LONG_MAX;
		for (int i = 0; i < 8; i++) {
			enum qrcodegen_Mask msk = (enum qrcodegen_Mask)i;
			applyMask(tempBuffer, qrcode, msk);
			drawFormatBits(ecl, msk, qrcode);
			long penalty = getPenaltyScore(qrcode);
			if (penalty < minPenalty) {
				mask = msk;
				minPenalty = penalty;
			}
			applyMask(tempBuffer, qrcode, msk);  // Undoes the mask due to XOR
		}
	}
	assert(0 <= (int)mask && (int)mask <= 7);
	applyMask(tempBuffer, qrcode, mask);  // Apply the final choice of mask
	drawFormatBits(ecl, mask, qrcode);  // Overwrite old format bits
}

// Encode QR-code to a pbm file; limited to redundancy M, mode 1 to 9
// return 0 iff OK
int encoder(
	const char* str,        // String to encode, coded per ISO/IEC 8859-1 (includes ASCII), up to 180 characters
	int square_size,        // Number of pixel for an elementary square (1 to 32)
	const char* filename    // Name for the pbm file to create (including .pbm extension)
) {
	int res = 1;						// future return code, error

	// check parameters
	if (str != NULL && (unsigned)(square_size - 1) < QR_MAX_SQUARE_SIZE && filename != NULL) {

		// find appropriate fmtidx (the standard's Version minus 1)
		int fmtidx;
		size_t bytlen = strlen(str);
		for (fmtidx = 0; fmtidx < sizeof(qr_maxbyt); ++fmtidx) {
			unsigned bytsiz = qr_maxbyt[fmtidx];
			if (bytlen <= bytsiz) { // found the right fmtidx
				uint8_t qrcode[QR_BYTSIZ(MAX_QR_FORMAT)];		// the QR code builds here

				// build data to encode into qrcode
				// mode (4 bits) + length (8 bites) + string + 0 (4 bits) + padding
				unsigned qrcsiz = 0;
				uint8_t c, m;
				qrcode[0] = (uint8_t)((bytlen >> 4) | 0x40);
				c = (uint8_t)(bytlen << 4);
				while (qrcsiz <= bytlen) {
					m = str[qrcsiz];
					qrcode[++qrcsiz] = (m >> 4) | c;
					c = m << 4;
				}
				bytsiz += 2;
				while (++qrcsiz != bytsiz) {
					qrcode[qrcsiz] = 0xEC;
					if (++qrcsiz == bytsiz) break;
					qrcode[qrcsiz] = 0x11;
				}
				// for (int j = 0; j < qrcsiz; ++j) printf("%02X", qrcode[j]);  printf("\n");
				myencode(qrcode, fmtidx + 1);

				{ // add the quiet zone and save qrcode as a PBM file
					FILE* f;
					uint8_t line[((QR_QUIET_ZONE * 2 + QR_SIDE(MAX_QR_FORMAT)) * QR_MAX_SQUARE_SIZE + 7) >> 3];
					qrcsiz = fmtidx * 4 + 21; // number of modules on the side of the QR code, not counting quiet zone
					unsigned pixsiz = (QR_QUIET_ZONE * 2 + qrcsiz) * square_size;	// size of a line in pixels including quiet zone
					unsigned bytsiz = (pixsiz + 7) >> 3;	// size of a line in bytes including quiet zone
					assert(bytsiz <= sizeof(line));
					(void)remove(filename);
					f = fopen(filename, "wb");
					if (f != NULL) {
						if (fprintf(f,
							"P4\n# QRCode created by stay at home\n%d %d\n", pixsiz, pixsiz) > 0) {
							int i, x, y, z = qrcsiz + QR_QUIET_ZONE;
							for (y = -QR_QUIET_ZONE; y < z; ++y) { // for each line
								// we build a complete line of bits, in big-endian format
								uint8_t * p = line; // current byte, mask, and pointer in a line
								m = 0x80;
								c = 0;
								for (x = -QR_QUIET_ZONE; x < z; ++x) { // for each colum
									if ((unsigned)x < qrcsiz && (unsigned)y < qrcsiz) { // in the active zone of the QR code
										i = qrcsiz * y + x; // bit index in qrcode 
										if ((qrcode[(i >> 3) + 1] >> (i & 7)) & 1) { // black pixel
											for (i = 0; i < square_size; ++i) {
												c |= m;
												m >>= 1;
												if (m == 0) {
													*p++ = c;
													c = 0;
													m = 0x80;
												}
											}
											continue;
										}
									}
									// white pixel
									for (i = 0; i < square_size; ++i) {
										m >>= 1;
										if (m == 0) {
											*p++ = c;
											c = 0;
											m = 0x80;
										}
									}
								}
								if (m != 0x80) // flush incomplete byte at end of line of the pbm
									*p++ = c;
								assert(p - line == bytsiz);
								for (i = 0; i < square_size; ++i) // write the line repeatedly
									if (fwrite(line, bytsiz, 1, f) != 1)
										goto ioerr;
							}
							res = 0; // sucess if we wrote all the file
						}
					ioerr:
						if (fclose(f) != 0) // error on close
							res = 1;
						if (res) // if there was an error, remove the file
							(void)remove(filename);
					}
				} // done writing the file
				break; // out of the loop searching fmtidx
			}
		}
	}
	return res;
}
