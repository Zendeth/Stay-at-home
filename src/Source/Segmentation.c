# include "../Header/Segmentation.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*---Free Functions---*/

static inline
void print_mat(char **mat, int size)
{
    printf("\n");
    for(int i = 0; i < size; i++)
    {
        printf("[ ");
        for(int j = 0; j < size; j++)
        {
            printf("%c ", mat[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
}

static inline
void free_FPat(struct FPat *f)
{
    if(f == NULL)
        return;
    free_Dmat(f->centers);
    free_Dvector(f->ems_vector);
    free(f);
}

static inline
void free_FPresults(struct FPresults *f)
{
    if(f == NULL)
        return;
    free(f);
}

static inline
void free_QrCode(struct QrCode *q)
{
    if(q == NULL)
        return;
  
    for(int i = 0; i < (q->version * 4 + 17); i++)
    {
        free(q->mat[i]);
    }
    free(q->mat);
    free(q);
}

static inline
void free_GeoImg(struct GeoImg *g)
{
    free(g->coordA);
    free(g->coordB);
    free(g->coordC);
    SDL_FreeSurface(g->img);
    free(g);
}

static inline
void free_segmentation(struct FPat *f, struct FPresults *fp, struct GeoImg *g, struct QrCode *q)
{
    free_FPat(f);
    free_FPresults(fp);
    free_GeoImg(g);
    free_QrCode(q);
}



/*---Main Functions---*/

static inline
void ImageProcessing(SDL_Surface *img)
{
    grayscale(img);
    int threshold = otsu(img);
    if(threshold == 0)
        threshold = 1;
    binarize(img, threshold);
}

static inline 
void ImageProcessingDemo(SDL_Surface *img)
{
    grayscale(img);
    //display_image(img);
    int threshold = otsu(img);
    if(threshold == 0)
        threshold = 1;
    binarize(img, threshold);
    //display_image(img);
}

static inline
void writeWhiteEpi(char *s)
{
    int i = 0;
    while(s[i] != '\0')
    {
        if(s[i] == '1' || s[i] == '0')
            s[i] = 'w';
        i++;
    }
}

// turn qr code into pbm file
void into_pbm(char ** grid, int s, const char* filename)
{
    FILE* fptr;
    fptr = fopen(filename,"w");

    if (fptr == NULL)
    {
        fprintf(stderr, "Error creating result file!");
        exit(1);
    }
    fprintf(fptr, "P1\n%u %u\n", s, s);

    
    for (ssize_t i = 0; i < s; i++)
    {
        for (ssize_t j = 0; j < s; j++)
        {
            fprintf(fptr, "%c ", grid[i][j]);
        }
        fprintf(fptr, "\n");
    }
    
    fclose(fptr);
}

/*struct PCode *Segmentation(SDL_Surface *img)
{
    ImageProcessing(img);
    struct FPat *f = findFP(img);
    struct FPresults *fp = QrCode_found(f);
    if(fp == NULL)
        err(EXIT_FAILURE, "Segmentation error : No Valid QrCode found");
    struct GeoImg *g = GeoTransform(img, fp);
    ImageProcessing(g->img);
    struct QrCode *qr = extract_QrCode(g);
    struct PCode *c = get_code(qr);
    free_segmentation(f, fp, g, qr);
    SDL_FreeSurface(img);
    return c;
}*/


/*struct PCode *SegmentationEpi(SDL_Surface *img, SDL_Surface *color)
{
    ImageProcessing(img);
    struct FPat *f = findFP(img);
    struct FPresults *fp = QrCode_found(f);
    if(fp == NULL)
        err(EXIT_FAILURE, "Segmentation error : No Valid QrCode found");
    struct GeoImg *g = GeoTransform(img, fp);
    ImageProcessing(g->img);
    struct GeoImg *gcolor = GeoTransform(color, fp);
    //display_image(gcolor->img);
    struct QrCode *qr = extract_EpCode(g, gcolor);
    struct PCode *c = get_code(qr);
    writeWhiteEpi(c->BStream);
    //printf("%s", c->BStream);
    free_segmentation(f, fp, g, qr);
    SDL_FreeSurface(img);
    return c;
}*/

int SegmentationDemo(SDL_Surface *img, SDL_Surface *demo)
{
    printf("|----------QRCode to matrix demonstration----------");
    printf("|\n|\n");
    
    ImageProcessingDemo(img);
    printf("|     - Image preprocessing Done\n");
    struct FPat *f = findFP(img);
    struct FPresults *fp = QrCode_found(f);
    if(fp == NULL)
    {   
        printf("| No QrCode were found, exit...\n");
        err(EXIT_FAILURE, "Segmentation error : No Valid QrCode found");
    }
    drawFP(demo, f->centers, f->ems_vector, fp->indexA);
    //display_image(demo);
    printf("|     - Finder Pattern and QrCode found and validated\n");
    struct GeoImg *g = GeoTransform(img, fp);
    ImageProcessingDemo(g->img);
    printf("|     - Affine transformation done\n");
    struct QrCode *qr = extract_QrCode(g);
    //display_image(g->img);
    printf("|     - Version determined -> V = %d\n", qr->version);
    printf("|     - Alignement Patterns found and image sampled into bit matrix \n");
    printf("|\n");
    printf("| QR Matrix :\n");
    print_mat(qr->mat, qr->version * 4 + 17);
    into_pbm(qr->mat, qr->version * 4 + 17, "result_seg.pbm");
    //free_segmentation(f, fp, g, qr);*/
    SDL_FreeSurface(img);
    SDL_FreeSurface(demo);
    return 0;
}

/*struct PCode *SegmentationFromFile(char *File, int Demo)
{
    init_sdl();
    SDL_Surface *img = load_image(File);
    struct PCode *c;
    if(Demo == 1)
    {
        SDL_Surface *demo = load_image(File);
        display_image(img);
        c = SegmentationDemo(img, demo);
    }
    else if(Demo == 2)
    {
        SDL_Surface *color = load_image(File);
        c = SegmentationEpi(img, color);
    }
    else
    {
        c = Segmentation(img);
    }
    
    SDL_Quit();
    return c;
}*/

/*
int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        if(strcmp(argv[2], "-d") == 0)
            SegmentationFromFile(argv[1], 1);
        else if(strcmp(argv[2], "-e") == 0)
            SegmentationFromFile(argv[1], 2);
    }
    else if(argc > 1)
        SegmentationFromFile(argv[1], 0);
    else
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
*/