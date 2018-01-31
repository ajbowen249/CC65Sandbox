#include "graphicsUtils.h"
#include "data.h"
#include "moreC64.h"
#include <c64.h>
#include <conio.h>

#define NUM_OPTIONS 4
#define SELECTED_OFFSET 9

int act_selection;
unsigned int act_baseSprites[NUM_OPTIONS] = {
    9, 7, 7, 8
};

void act_updateMenuSprites() {
    unsigned int i;
    for (i = 0; i < NUM_OPTIONS; ++i) {
        unsigned int sprite = act_baseSprites[i];
        if (i == act_selection) {
            sprite += SELECTED_OFFSET;
        }

        assignSprite(sprite, i, 0, 0);
    }
}

void act_changeselection(char direction) {
    int offset = direction ? 1 : -1;
    act_selection += offset;
    if (act_selection >= NUM_OPTIONS) {
        act_selection = 0;
    }
    else if (act_selection < 0) {
        act_selection = NUM_OPTIONS - 1;
    }

    act_updateMenuSprites();
}

void setupActivity() {
    clrscr();
    deactivateAllSprites();
    act_selection = 1;

    act_updateMenuSprites();

    VIC.spr_mcolor = 0xFF;
    VIC.spr_mcolor0 = SPRITE_M_COLOR_1;
    VIC.spr_mcolor1 = SPRITE_M_COLOR_2;

    VIC.spr0_x = 24;
    VIC.spr0_y = 51;

    VIC.spr1_x = 31;
    VIC.spr1_y = 85;

    VIC.spr2_x = 31;
    VIC.spr2_y = 116;

    VIC.spr3_x = 31;
    VIC.spr3_y = 148;

    VIC.bordercolor = 0x0F;
    VIC.bgcolor0 = 0x01;

    cputsxy(14, 1, "All Activity");
    cputsxy(4,  5, "Your Google Pixel");
    cputsxy(4,  9, "Tom's iPhone X");
    cputsxy(4,  13, "Your Osborne-1");

    return;
}

char getSelectedScreen() {
    switch(act_selection) {
    case 0:
        return SC_DASHBOARD;
    default:
        return SC_DASHBOARD;
    }
}

char activityMain(void) {
    while(1) {
        if(kbhit()) {
            char key = cgetc();
            switch(key) {
            case CURSOR_UP:
                act_changeselection(0);
                break;
            case CURSOR_DOWN:
                act_changeselection(1);
                break;
            case RETURN:
                return getSelectedScreen();
            default:
                break;
            }
        }
    }

    return SC_DASHBOARD;
}
