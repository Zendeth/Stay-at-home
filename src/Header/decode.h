#ifndef DECODE_H
#define DECODE_H

#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int power(int base, int exp);
int len_mat(char**mat);
void Mask(char**qr, int len);
void init_qr(char** qr_mat, int size);
char* Decode_QR(char** qr, int size);

#endif
