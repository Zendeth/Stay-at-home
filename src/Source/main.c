#include <stdlib.h>
#include <string.h>

#include "../Header/pbm.h"
#include "../Header/decoder.h"

# include "../Header/MySDL.h"
# include "../Header/PreProcessing.h"
# include "../Header/PatternFinder.h"
# include "../Header/ValidFinder.h"
# include "../Header/Draw.h"
# include "../Header/Segmentation.h"
# include <stdio.h>

void free_Finder(struct FPat *f)
{
    if(f == NULL)
        return;
    free_Dmat(f->centers);
    free_Dvector(f->ems_vector);
    free(f);
}

static inline
void imageProcessing(SDL_Surface *img)
{
    grayscale(img);
    int threshold = otsu(img);
    if (threshold == 0)
        threshold = 1;
    binarize(img, threshold);
}



int main(int argc, char *argv[])
{
    SDL_Surface *img = load_image(argv[1]);
    SDL_Surface *show = load_image(argv[1]);
    imageProcessing(img);
    /*warn("Image Processing done");

    //display_image(show);
    struct FPat *f = findFP(img);
    warn("Find Finder Patterns done");

    struct FPresults *fp = QrCode_found(f);
    warn("QrCode validation done");


    drawFP(img,f->centers,f->ems_vector,2);

    save_image(img,"output/test.png");*/
    SegmentationDemo(img,show);
	decode_demo();
}