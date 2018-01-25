#include "graphicsUtils.h"
#include "data.h"
#include <c64.h>

void setupDashboard() {
    assignSprite(1, 0, 1, 1);
    assignSprite(2, 1, 1, 1);
    assignSprite(3, 2, 1, 1);
    assignSprite(4, 3, 1, 1);
    assignSprite(6, 4, 1, 1);

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

    return;
}
