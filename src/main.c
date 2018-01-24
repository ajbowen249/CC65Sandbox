#include <c64.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "coreTiming.h"
#include "data.h"

char frames = 0;
char anim = 0;

void assignSprite(unsigned int sheetIndex, unsigned int vicIndex, char stretchX, char stretchY) {
    char* spritePointer = (unsigned char*)(((VIC.addr & 0xF0) >> 4) * 1024) + 1016;
    char mask = 0x01 << vicIndex;
    char maskInv = ~mask;
    char stretchXByte = (stretchX ? 0x01 : 0x00) << vicIndex;
    char stretchYByte = (stretchY ? 0x01 : 0x00) << vicIndex;
    char* spriteAddress = (char*)(SPRITESHEET + (sheetIndex * 0x40));

    spritePointer += vicIndex;
    *spritePointer =  (SPRITESHEET + (sheetIndex * 0x40)) / 0x40;

    VIC.spr_ena = VIC.spr_ena | mask;
    VIC.spr0_color = spriteAddress[63];
    VIC.spr_exp_x = (VIC.spr_exp_x & maskInv) | stretchXByte;
    VIC.spr_exp_y = (VIC.spr_exp_y & maskInv) | stretchYByte;
}

void draw() {
    ++frames;
    if (frames % 30 == 0) {
        VIC.spr0_y = 128;
        VIC.spr0_x = (VIC.spr0_x + 1) % 256;
    }
    if (frames == 60) {
        frames = 0;
        anim = (anim + 1) % 5;
        assignSprite(anim, 0, 1, 1);
    }
    return;
}

int main(void) {
    clrscr();

    assignSprite(0, 0, 1, 1);
    setupIsr(draw);

    while(1) {

    }

    return EXIT_SUCCESS;
}
