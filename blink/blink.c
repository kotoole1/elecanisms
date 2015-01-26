#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();

    led_on(&led1);
    timer_setPeriod(&timer2, 0.1);
    timer_start(&timer2);
    
    int timer_switch = 0;
    int timer_count = 0;
    int timer_max_count = 10;
    int led3_on = 1;
    int sw3_prev = 0;
    int sw3_now = 0;
    
    while (1) {
        sw3_now = !sw_read(&sw3);
        if (sw3_now && !sw3_prev)
        {
            timer_switch = (timer_switch + 1) % 4;
            switch (timer_switch) {
                case 0:
                    timer_max_count = 10;
                    break;
                case 1:
                    timer_max_count = 5;
                    break;
                case 2:
                    timer_max_count = 2;
                    break;
                case 3:
                    timer_max_count = 1;
                    break;
                    
                default:
                    timer_max_count = 50;
                    break;
            }
        }
        sw3_prev = sw3_now;
        
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);
            timer_count = (timer_count + 1) % timer_max_count;
            led3_on = timer_count ? led3_on : !led3_on;
        }
        
        led_write(&led3, led3_on);
    }
}
