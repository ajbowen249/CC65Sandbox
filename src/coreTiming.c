#include "coreTiming.h"
#include <stdio.h>
#include <stdlib.h>
#include <c64.h>
#include <6502.h>
#include "moreC64.h"

unsigned char vic2Isr(void);

#define ISR_STACK_SIZE 1024
unsigned char isrStack[ISR_STACK_SIZE];
iterruptCallback g_rasterCallback;

void setupIsr(iterruptCallback rasterCallback) {
    char icr;
    g_rasterCallback = rasterCallback;

    // This sequence for setting up the raster interrupt is
    // more or less pulled from
    // http://codebase64.org/doku.php?id=base:introduction_to_raster_irqs

    SEI();

    // Disable the two CIA interrupts.
    CIA1.icr = 0x7F;
    CIA2.icr = 0x7F;

    // Reading from ICR prevents any pending interrupts from the CIAs.
    icr = CIA1.icr;
    icr = CIA2.icr;

    // IMPROVE: This has flags for raster line, sprite-background collision,
    //          sprite-sprite collision, and lightpen impulse. Set this up
    //          based on provided callbacks.
    VIC.imr = 0x01;

    // Register for an interrupt at rasterline 0.
    VIC.rasterline = 0x00;
    // IMPROVE: The hsb here is the 9th bit for the rasterline.
    //          Other bits do other things. Pass them in?
    VIC.ctrl1 &= 0x7F;

    set_irq(vic2Isr, isrStack, ISR_STACK_SIZE);

    CLI();

    return;
}

unsigned char vic2Isr(void) {
    g_rasterCallback();
    return IRQ_HANDLED;
}
