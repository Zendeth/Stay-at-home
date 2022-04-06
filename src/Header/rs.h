#ifndef RS_H
#define RS_H

#include "scanner.h"

typedef struct poly poly_t;

struct poly
{
	size_t d;    // degree
	byte c[512]; // coefficients
};

byte rs_decode(size_t n_data, byte* data, byte n_sym);

#endif