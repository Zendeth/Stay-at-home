# ifndef _SEGQR_H_
# define _SEGQR_H_

# include "GeoTrans.h"
//# include "proc_code.h"
# include "ExtractCode.h"
# include "ValidFinder.h"
# include "PatternFinder.h"
# include "PreProcessing.h"
# include "Draw.h"
# include <stdio.h>

struct PCode *Segmentation(SDL_Surface *img);

/*  Same as above, but shows also the different steps of the segmentation */
int SegmentationDemo(SDL_Surface *img, SDL_Surface *demo);

/* Combines previous functions into one function that takes the path to the img
 * to treat and if a Demo is asked*/
struct PCode *SegmentationFromFile(char *File, int Demo);

/* Frees the structure PCode */
//void Free_PCode(struct PCode *c);

# endif