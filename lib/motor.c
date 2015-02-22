#include <p24FJ128GB206.h>
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"
#include "uart.h"
#include "motor.h"
#include <stdbool.h> 



MotorState motor_state = DAMPER;
MotorDirection motor_direction = ZERO;
volatile uint16_t switch1;
volatile bool lastDirection = false;

void stop_motor() {
    pin_clear(&D[5]);
    pin_clear(&D[6]);
}

void start_motor_clockwise() {
    pin_clear(&D[3]);
    pin_set(&D[4]);
    pin_set(&D[5]);
    pin_clear(&D[6]);
}

void start_motor_counterClockwise() {
    pin_clear(&D[3]);
    pin_set(&D[4]);
    pin_clear(&D[5]);
    pin_set(&D[6]);   
}

void freespin_motor() {
    pin_set(&D[3]);
    pin_clear(&D[4]);
    stop_motor();
}

void pwm_motor(uint16_t duty, bool direction)
{
    if (direction != motor_direction) {
        if (direction) {
            start_motor_clockwise();
        }
        else {
            start_motor_counterClockwise();
        }
        lastDirection = direction;
    }
        oc_pwm(&oc1, &D[4], NULL, 500, duty << 6);
}



void init_motor() {
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
