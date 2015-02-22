#include <p24FJ128GB206.h>
#include "common.h"
#include "timer.h"
#include "uart.h"
#include "pin.h"
#include "analogRead.h"
#include <stdio.h>

volatile uint16_t pin5;

void read_pins(void) {
    pin5 = pin_read(&A[5]) >> 6;
}

void init_analog_read(void) {
    int i;
    for (i = 0; i<6; i++) {
        pin_analogIn(&A[i]);
    }
}
