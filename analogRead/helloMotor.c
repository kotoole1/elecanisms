#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"

void stop_motor() {
    pin_clear(&D[5]);
    pin_clear(&D[6]);
}

void start_motor_clockwise() {
    pin_set(&D[5]);
}

void setup_motor() {
    init_pin();
    pin_digitalOut(&D[2]);
    pin_digitalOut(&D[3]);
    pin_digitalOut(&D[4]);
    pin_set(&D[2]);
    pin_clear(&D[3]);
    pin_set(&D[4]);
    pin_digitalOut(&D[5]);
    pin_digitalOut(&D[6]);
    stop_motor();
}

int16_t main(void) {
    setup_motor();
    // start_motor_clockwise();
    init_clock();
    init_ui();
    init_timer();

    // led_on(&led1);
    // timer_setPeriod(&timer2, 0.5);
    // timer_start(&timer2);

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);
        }
        
        led_write(&led3, !sw_read(&sw2));

        pin_write(&D[5], !sw_read(&sw1));
    }
}
