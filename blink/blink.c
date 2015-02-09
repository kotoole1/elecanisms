#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();

    timer_setPeriod(&timer1, 0.3);
    timer_setPeriod(&timer2, 0.25);    
    timer_setPeriod(&timer3, 0.5);
    
    timer_start(&timer2);
    timer_start(&timer1);
    timer_start(&timer3);

    while (1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            led_toggle(&led1);
        }
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led2);
        }
        if (timer_flag(&timer3)) {
            timer_lower(&timer3);
            led_toggle(&led3);
        }
    }
}

