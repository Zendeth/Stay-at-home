#ifndef ENCODER_H
#define ENCODER_H

// Encode QR-code to a pbm file; limited to redundancy M, mode 1 to 9
// return 0 iff OK
int encoder(
  const char* str,        // String to encode, coded per ISO/IEC 8859-1 (includes ASCII), 0 to 180 characters
  int square_size,        // Number of pixel for an elementary square (1 to 32)
  const char* filename    // Name for the pbm file to create (including .pbm extension)
);
#define QR_MAX_SQUARE_SIZE 32   // maximum value of square_size
#define QR_MAX_LEN_STR     180  // maximum length of str

#endif
