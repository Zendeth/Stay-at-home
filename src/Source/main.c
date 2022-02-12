#include "../Header/MySDL.h"
#include "../Header/PreProcessing.h"

int main(int argc, char *argv[])
{
    SDL_Surface* img = load_image(argv[1]);
    img = grayscale(img);
    img = contrast(img);
    binarize(img,100);
    otsu(img);
    display_image(img);
    printf("aaaaa");
    save_image(img, "output/test.png");
    
}