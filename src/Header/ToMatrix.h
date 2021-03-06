# ifndef _AP_SAMP_H_
# define _AP_SAMP_H_

# include <stdlib.h>
# include "GeoTrans.h"
# include "Draw.h"
# include "Data_c.h"

struct QrCode
{
    int version;
    char **mat;
};

void SampleCodeV2_6(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB, int WC, int HA, int HB, int HC);
void SampleCodeV7_40(struct GeoImg *qrimg, struct QrCode *qr, int WA, int WB, int WC, int HA, int HB, int HC);
# endif