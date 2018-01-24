#include <c64.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "coreTiming.h"
#include "data.h"
#include "dashboard.h"

void draw() {

    return;
}

int main(void) {
    clrscr();
    setupDashboard();

    setupIsr(draw);

    while(1) {

    }

    return EXIT_SUCCESS;
}
