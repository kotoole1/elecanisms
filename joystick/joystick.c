#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"
#include "uart.h"
#include "motor.h"
#include "analogRead.h"

volatile uint16_t lastAngle;
uint16_t flipThreshold = 200;
volatile uint16_t numPassed = 0;

void switch_state() {
    switch1 = sw_read(&sw1);
    if (motor_state == FREE) {
        if (!switch1) {
            motor_state = PWM;
            pwm_motor(1023);
        }
    }
    if (motor_state == PWM)
    {
        if (switch1)
        {
            motor_state = FREE;
            freespin_motor();
        }
    }
}

void get_angle(void) {
    printf("%d\n", pin5);
}

int16_t main(void) {
    init_pin();
    init_clock();
    init_ui();
    init_uart();
    init_timer();
    init_oc();
    init_analog_read();
    init_motor();

    timer_setPeriod(&timer1, 0.0001);
    timer_start(&timer1);
    timer_setPeriod(&timer2, 0.01);
    timer_start(&timer2);

    lastAngle = pin_read(&A[5]) >> 6;
    while (1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            read_pins();
            get_angle();
        }
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            switch_state();
        }
    }
}
