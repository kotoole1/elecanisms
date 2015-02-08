#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"
#include "uart.h"

volatile uint16_t pin5;
volatile uint16_t switch1;
volatile uint16_t lastAngle;
uint16_t flipThreshold = 200;
volatile uint16_t numPassed;

typedef enum {
    ON,
    PWM,
    OFF,
    FREE
} MotorState;

MotorState motor_state = FREE;

void stop_motor() {
    pin_clear(&D[5]);
    pin_clear(&D[6]);
}

void start_motor_clockwise() {
    pin_clear(&D[3]);
    pin_set(&D[4]);
    pin_set(&D[5]);
}

void freespin_motor() {
    pin_set(&D[3]);
    pin_clear(&D[4]);
    stop_motor();
}

void pwm_motor(void)
{
    start_motor_clockwise();
    oc_pwm(&oc1, &D[4], NULL, 500, 1023 << 6);
}

void flipped(){
    printf("%d \n", numPassed);
    numPassed = 0;
}

void read_pins() {
    numPassed = numPassed + 1;
    pin5 = pin_read(&A[5]) >> 6;
    // printf("%d, \n %d \n", lastAngle - pin5, flipThreshold);
    uint16_t difference = lastAngle - pin5; 
    if (!(difference > 0x8000) && difference >= flipThreshold) {
        flipped();
    }
    lastAngle = pin5;
}

void switch_state() {
    switch1 = sw_read(&sw1);
    if (motor_state == FREE) {
        if (!switch1) {
            motor_state = PWM;
            pwm_motor();
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

void setup_pins() {
    int i;
    for (i = 0; i<6; i++) {
        pin_analogIn(&A[i]);
    }
}

void setup_motor() {
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
    init_pin();
    init_clock();
    init_ui();
    init_uart();
    init_timer();
    init_oc();
    setup_pins();
    setup_motor();

    timer_setPeriod(&timer1, 0.0001);
    timer_start(&timer1);
    timer_setPeriod(&timer2, 0.01);
    timer_start(&timer2);

    lastAngle = pin_read(&A[5]) >> 6;
    while (1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            read_pins();
        }
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            switch_state();
        }
    }
}
