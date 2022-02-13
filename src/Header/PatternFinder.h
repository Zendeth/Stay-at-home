# ifndef _FPFIND_H_
# define _FPFIND_H_

# include <stdio.h>
# include "Dmat.h"
# include "Dvector.h"
# include "../Header/MySDL.h"

struct FPat
{
    struct Dmat *centers; //coords of center of FP
    struct Dvector *ems_vector; //estimated module size
};

struct FPat *findFP(SDL_Surface *img);

# endif