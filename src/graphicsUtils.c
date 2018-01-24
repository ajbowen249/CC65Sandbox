#include "data.h"
#include <c64.h>

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
    *((&VIC.spr0_color) + vicIndex) = spriteAddress[63];
    VIC.spr_exp_x = (VIC.spr_exp_x & maskInv) | stretchXByte;
    VIC.spr_exp_y = (VIC.spr_exp_y & maskInv) | stretchYByte;
}