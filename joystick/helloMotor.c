#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"

volatile uint16_t pin5;
volatile uint16_t switch1;

typedef enum {
    ON,
    PWM,
    OFF
} MotorState;

MotorState motor_state = OFF;

void pwm_motor(void)
{
    oc_pwm(&oc1, &D[4], NULL, 500, 1023 << 6);
}

void stop_motor() {
    pin_clear(&D[5]);
    pin_clear(&D[6]);
}

void read_pins(_TIMER *self) {
    pin5 = pin_read(&A[5]) >> 6;
    switch1 = sw_read(&sw1);
    if (motor_state == OFF) {
        if (switch1) {
            motor_state = PWM;
            pwm_motor();
        }
    }
    if (motor_state == PWM)
    {
        if (!switch1)
        {
            motor_state = OFF;
            stop_motor();
        }
    }
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
    init_clock();
    init_ui();
    init_timer();
    init_oc();

    timer_every(&timer2, 0.001, read_pins);

    while (1) {
        // if (switch1)
        // {
        //     oc_pwm(&oc1, &D[4], NULL, 500, 1023 << 6);
        // }
        // else
        // {
        //     pin_clear(&D[4]);
        // }
    }
}
