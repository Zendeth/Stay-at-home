#ifndef PREPROCING_H_
#define PREPROCING_H_

#include"../sdl/mysdl.h"

SDL_Surface *grayscale(SDL_Surface* img);
void binarize(SDL_Surface* img, int threshold);
int otsu(SDL_Surface* img);
SDL_Surface *contrast(SDL_Surface* img);
#endif