#include "sim_io.h"
#include <stdio.h>
#include <time.h>

/* Porty */
#define P1 0x300 //out
#define P2 0x301 //out
#define P3 0x300 //in
#define P4 0x301 //in

/* Hdnoty */
#define Max 0xff
#define Refresh 8.8e+5
#define RegidRef 120

/*

    * Písmo

*/

int A[] = {0x00, 0x7f, 0xff, 0xcc, 0xcc, 0xff, 0x7f, 0x00};
int G[] = {0x00, 0x7e, 0xff, 0xc3, 0xcb, 0xef, 0x6e, 0x00};
int E[] = {0x00, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0x00};
int QUE[] = {0x00, 0x00, 0x00, 0xfb, 0xfb, 0x00, 0x00, 0x00};
int B[] = {0x00, 0xff, 0xff, 0x99, 0x99, 0xff, 0x66, 0x00};
int C[] = {0x00, 0x7e, 0xff, 0xc3, 0xc3, 0xe7, 0x66, 0x00};
int O[] = {0x00, 0x7e, 0xff, 0xc3, 0xc3, 0xff, 0x7e, 0x00};
int S[] = {0x00, 0x73, 0xfb, 0x99, 0x99, 0xdf, 0xce, 0x00};
int T[] = {0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xc0, 0x00};

// --------

/* 

    * Sloupce
    * Registry

*/
int Sloupce[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

enum Registry {
    R0 = 0x00,
    R1 = 0x01,
    R2 = 0x02,
    R3 = 0x04,
    R4 = 0x08,
    R5 = 0x10,
    R6 = 0x20,
    R8 = 0x80
};

// -------

void Spi(int cas) {
    struct timespec spanek =
    { .tv_sec = 0, .tv_nsec = cas};
    nanosleep(&spanek, NULL);
}

int NastavHodnotu(int Hodnota) {
    return Max ^ Hodnota;
}

void UlozHodnotu() {
    Spi(Refresh);
    outb(R0, P2);
    Spi(RegidRef);
    outb(Max, P1);
}

void ZapisNaReg(int Reg,int Hodnota){
    outb(Reg, P2);
    outb(NastavHodnotu(Hodnota), P1);
    UlozHodnotu();
}

int main(void)
{  
    if(ioperm(0x300,2,1)){
        printf("Sudo?");
        return -1;
    }
    ZapisNaReg(R8, 0);
    Spi(RegidRef);
    UlozHodnotu();
    int i = 0;
    while (1)
    {
        if(i == 8 ) i = 0;
        /* Nastav hodnoty na R1 - R6 */
        // TACCOS
        /*
        ZapisNaReg(R1, T[i]);
        ZapisNaReg(R2, A[i]);
        ZapisNaReg(R3, C[i]);
        ZapisNaReg(R4, C[i]);
        ZapisNaReg(R5, O[i]);
        ZapisNaReg(R6, S[i]);
        */
        // GOOSE!
        
        ZapisNaReg(R1, G[i]);
        ZapisNaReg(R2, O[i]);
        ZapisNaReg(R3, O[i]);
        ZapisNaReg(R4, S[i]);
        ZapisNaReg(R5, E[i]);
        ZapisNaReg(R6, QUE[i]); 
        
        /* Nastav hodnotu na R8*/
        Spi(Refresh);
        ZapisNaReg(R8, Sloupce[i]);
        Spi(RegidRef);
        ZapisNaReg(R8, 0);
        Spi(RegidRef);
        UlozHodnotu();
        i++;
    } 
    ioperm(0x300,2,0);
}
