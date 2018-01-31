#include "graphicsUtils.h"
#include "data.h"
#include "moreC64.h"
#include <c64.h>
#include <conio.h>

#define NUM_OPTIONS 5
#define SELECTED_OFFSET 9

int dash_selection;
unsigned int dash_baseSprites[NUM_OPTIONS] = {
    1, 2, 3, 4, 6
};

void dash_updateMenuSprites() {
    unsigned int i;
    for (i = 0; i < NUM_OPTIONS; ++i) {
        unsigned int sprite = dash_baseSprites[i];
        if (i == dash_selection) {
            sprite += SELECTED_OFFSET;
        }

        assignSprite(sprite, i, 1, 1);
    }
}

void dash_changeselection(char direction) {
    int offset = direction ? 1 : -1;
    dash_selection += offset;
    if (dash_selection >= NUM_OPTIONS) {
        dash_selection = 0;
    }
    else if (dash_selection < 0) {
        dash_selection = NUM_OPTIONS - 1;
    }

    dash_updateMenuSprites();
}

void setupDashboard() {
    clrscr();
    deactivateAllSprites();
    dash_selection = 0;

    assignSprite(19, 5, 0, 0);

    dash_updateMenuSprites();

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

    VIC.spr5_x = 42;
    VIC.spr5_y = 227;

    VIC.bordercolor = 0x0F;
    VIC.bgcolor0 = 0x01;

    cputsxy(15, 5, "Hi Michael");
    cputsxy(1,  15, "Activity");
    cputsxy(11, 15, "Me");
    cputsxy(16, 15, "Partners");
    cputsxy(25, 15, "Account");
    cputsxy(33, 15, "Learn");
    cputsxy(5,  23, "This Commodore 64 is monitored.");

    return;
}

char dash_getSelectedScreen() {
    switch(dash_selection) {
    case 0:
        return SC_ACTIVITY;
    default:
        return SC_DASHBOARD;
    }
}

char dashboardMain(void) {
    while(1) {
        if(kbhit()) {
            char key = cgetc();
            switch(key) {
            case CURSOR_LEFT:
                dash_changeselection(0);
                break;
            case CURSOR_RIGHT:
                dash_changeselection(1);
                break;
            case RETURN:
                return dash_getSelectedScreen();
            default:
                break;
            }
        }
    }

    return SC_DASHBOARD;
}
