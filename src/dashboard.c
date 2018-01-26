#include "graphicsUtils.h"
#include "data.h"
#include "moreC64.h"
#include <c64.h>
#include <conio.h>

#define NUM_OPTIONS 5
#define SELECTED_OFFSET 9

int selection = 0;
unsigned int baseSprites[NUM_OPTIONS] = {
    1, 2, 3, 4, 6
};

void updateMenuSprites() {
    unsigned int i;
    for (i = 0; i < NUM_OPTIONS; ++i) {
        unsigned int sprite = baseSprites[i];
        if (i == selection) {
            sprite += SELECTED_OFFSET;
        }

        assignSprite(sprite, i, 1, 1);
    }
}

void changeSelection(char direction) {
    int offset = direction ? 1 : -1;
    selection += offset;
    if (selection >= NUM_OPTIONS) {
        selection = 0;
    }
    else if (selection < 0) {
        selection = NUM_OPTIONS - 1;
    }

    updateMenuSprites();
}

void setupDashboard() {
    updateMenuSprites();

    VIC.spr_mcolor = 0xFF;
    VIC.spr_mcolor0 = SPRITE_M_COLOR_1;
    VIC.spr_mcolor1 = SPRITE_M_COLOR_2;

    VIC.spr0_x = 37;
    VIC.spr0_y = 128;

    VIC.spr1_x = 99;
    VIC.spr1_y = 128;

    VIC.spr2_x = 160;
    VIC.spr2_y = 128;

    VIC.spr3_x = 221;
    VIC.spr3_y = 128;

    VIC.spr4_x = 0x1B;
    VIC.spr_hi_x |= 0x10;
    VIC.spr4_y = 128;

    VIC.bordercolor = 0x0F;
    VIC.bgcolor0 = 0x01;

    cputsxy(15, 5, "Hi Michael");
    cputsxy(1, 15, "Activity");
    cputsxy(11, 15, "Me");
    cputsxy(16, 15, "Partners");
    cputsxy(25, 15, "Account");
    cputsxy(33, 15, "Learn");

    return;
}

char dashboardMain(void) {
    while(1) {
        if(kbhit()) {
            char key = cgetc();
            switch(key) {
            case CURSOR_LEFT:
                changeSelection(0);
                break;
            case CURSOR_RIGHT:
                changeSelection(1);
                break;
            default:
                break;
            }
        }
    }

    return SC_DASHBOARD;
}
