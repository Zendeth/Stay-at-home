#include "decode.h"

int power(int base, int exp)
{
    int result = 1;
    for (; exp > 0; exp--)
    {
        result = result * base;
    }
    return result;
}

// Size of a matrix (with size => size x size)
/*int len_mat(char**mat)
{
    return sizeof(mat[0])/sizeof(char);
}*/

// Resolve the mask patterns (for now only j % 3 = 0)
void Mask(char**qr, int len)
{
    for (int y = 0; y < len; y++)
        for (int x = 0; x < len; x += 3)
        {
            if (qr[y][x] == '0')
            {
                qr[y][x] = '1';
                /*char* q = (char *)malloc(21 * sizeof(char));
                free q;*/
            }
            else if (qr[y][x] == '1')
                qr[y][x] = '0';
        }  
}

// Set the function patterns (0=>2 && 1=>3)
void init_qr(char** qr_mat, int size)
{
    // Vertical
    int y = 0;
    // Horizontal
    int x = 0;

    // Upper left fixed pattern
    for (; y < 9; y++)
        for (; x < 9; x++)
            qr_mat[y][x] = 'a';

    // Upper right fixed pattern
    for (y = 0; y < 9; y++)
        for (x = size - 8; x < 8; x++)
            qr_mat[y][x] = 'a';

    // Lower left fixed pattern
    for (y = size - 8; y < 8 ; y++)
        for (x = 0; x < 9; x++)
            qr_mat[y][x] += 'a';
    
    // Upper timing pattern
    y = 6;
    for (x = 9; x < size - 8; x++)
        qr_mat[y][x] += 'a';

    // Left timing pattern
    x = 6;
    for (y = 9; y < size - 8; y++)
        qr_mat[y][x] += 'a'; 
}

char* Decode_QR(char** qr, int size)
{

    ///int size = 21; //len_mat(qr);

    init_qr(qr, size);

    // Number of bits (a chunk of bites is contained in a x by y square/rectangle)
    int bit = 4*2; /* For now we will take 8 bit chunks */

    int y = size-1;
    int x = size-1;
    
    // Ecoding mode
    int* enc_mode = malloc(sizeof(int)*4);
    for (; y >= size - 2; y--)
    {
        for (x = size; x >= size - 2; x--)
        {
            *enc_mode = qr[y][x];//*(*(qr + y) + x);
            enc_mode += 1;
            printf("hello\n");
            //fflush(STDOUT);
        }
    }


    // TODO: Figure out wich mask to take
    Mask(qr, size);

    free(enc_mode);
    // Lenght of data
    int length = 0;
    while (bit != 0)
    {
        y -= 1;
        x += 1;
        if (qr[y][x] == 1)  
            length += (int)power(2, bit);
        bit--;

        x -= 1;
        if (qr[y][x] == 1)  
            length += (int)power(2, bit);
        bit--;
    }

    char* res = (char*) malloc(length*sizeof(char));
    //int end = 0;
    int my = -1;
    //int mx = 1;
    for (int l = 0; l < length; l++) //while (length != 0)
    {
        bit = 4*2;
        // Travel through a chunk (for now each chunk will represent a character)
        // each loop should traverse 2 bit
        while (bit != 0)
        {
            y += my;
            x += 1;
            // x > size || x < 0) // or qr[y][x] < 0 
            if (y > size || y < 0 )
            {
                my = -my;
                y += my;
                x -= 2;
                if (qr[y][x] == '1')  
                    res[l] += power(2, bit);
                
                bit--;
            }
            if (qr[y][x] == '1')  
            {
                res[l] += power(2, bit);
                bit--;
            }
            else if (qr[y][x] == '0')
            {
                bit--;
            }
            
            //if (bit == 0) break;

            x -= 1;

            if (qr[y][x] == '1')  
            {
                res[l]= power(2, bit);
                bit--;
            }
            else if (qr[y][x] == '0')
            {
                bit--;
            }
        }
        res++;
        
    }
        return res;

}

