# ifndef _EXTCODE_H_
# define _EXTCODE_H_

# include <math.h>
# include "ToMatrix.h"
# include <stdlib.h>
# include <err.h>

struct QrCode *extract_QrCode(struct GeoImg *qrimg);
struct QrCode *extract_EpCode(struct GeoImg *qrimg, struct GeoImg *coimg);

# endif