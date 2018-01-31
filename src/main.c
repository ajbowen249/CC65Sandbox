#include <c64.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "coreTiming.h"
#include "data.h"
#include "dashboard.h"
#include "activity.h"

typedef char(*screenMain)(void);

screenMain switchScreens(char screenNumber) {
    switch(screenNumber) {
    case SC_DASHBOARD:
        setupDashboard();
        return dashboardMain;
    case SC_ACTIVITY:
        setupActivity();
        return activityMain;
    default:
        exit(EXIT_SUCCESS);
    }
}

int main(void) {
    screenMain screenFunc;

    clrscr();
    screenFunc = switchScreens(SC_DASHBOARD);

    while(1) {
        screenFunc = switchScreens(screenFunc());
    }

    return EXIT_SUCCESS;
}
