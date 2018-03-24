#include "freq2pitch.h"

#define C_0 65.405
#define D_0 73.416
#define E_0 82.407
#define F_0 87.307
#define G_0 97.999
#define A_0 110.000
#define B_0 123.471

int freq2pitch(double freq,int major){

    if (freq < (double)major*(double)62) return 0;
    if (freq > (double)major*(double)(D_0+4.5) *(double) 16) return 0;

    if ((double)62*(double)major<freq && freq<(double)major*(double)(C_0+4))
        return 1;
    else if (freq<(double)major*(double)(D_0+4.5))
        return 2;
    else if (freq<(double)major*(double)(E_0+2.5))
        return 3;
    else if (freq<(double)major*(double)(F_0+5))
        return 4;
    else if (freq<(double)major*(double)(G_0+6))
        return 5;
    else if (freq<(double)major*(double)(A_0+6.5))
        return 6;
    else if (freq<(double)major*(double)(B_0+3.5))
        return 7;
    else if (freq<(double)major*(double)(C_0+4) *(double) 2)
        return 8;
    else if (freq<(double)major*(double)(D_0+4.5) *(double) 2)
        return 9;
    else if (freq<(double)major*(double)(E_0+2.5) *(double) 2)
        return 10;
    else if (freq<(double)major*(double)(F_0+5) *(double) 2)
        return 11;
    else if (freq<(double)major*(double)(G_0+6) *(double) 2)
        return 12;
    else if (freq<(double)major*(double)(A_0+6.5) *(double) 2)
        return 13;
    else if (freq<(double)major*(double)(B_0+3.5) *(double) 2)
        return 14;
    else if (freq<(double)major*(double)(C_0+4) *(double) 4)
        return 15;
    else if (freq<(double)major*(double)(D_0+4.5) *(double) 4)
        return 16;
    else if (freq<(double)major*(double)(E_0+2.5) *(double) 4)
        return 17;
    else if (freq<(double)major*(double)(F_0+5) *(double) 4)
        return 18;
    else if (freq<(double)major*(double)(G_0+6) *(double) 4)
        return 19;
    else if (freq<(double)major*(double)(A_0+6.5) *(double) 4)
        return 20;
    else if (freq<(double)major*(double)(B_0+3.5) *(double) 4)
        return 21;
    else if (freq<(double)major*(double)(C_0+4) *(double) 8)
        return 22;
    else if (freq<(double)major*(double)(D_0+4.5) *(double) 8)
        return 23;
    else if (freq<(double)major*(double)(E_0+2.5) *(double) 8)
        return 24;
    else if (freq<(double)major*(double)(F_0+5) *(double) 8)
        return 25;
    else if (freq<(double)major*(double)(G_0+6) *(double) 8)
        return 26;
    else if (freq<(double)major*(double)(A_0+6.5) *(double) 8)
        return 27;
    else if (freq<(double)major*(double)(B_0+3.5) *(double) 8)
        return 28;\
    else if (freq<(double)major*(double)(C_0+4) *(double) 16)
        return 29;
    else if (freq<(double)major*(double)(D_0+4.5) *(double) 16)
        return 30;



}





