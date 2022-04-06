#ifndef PBM_H
#define PBM_H

#include <stdio.h>

#include "scanner.h"

void load_pbm(scanner_t* scanner, FILE* f);
void into_pbm(scanner_t* scanner, const char* filename);

#endif