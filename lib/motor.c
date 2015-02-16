#include <p24FJ128GB206.h>
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"
#include "uart.h"
#include "motor.h"

MotorState motor_state = FREE;
volatile uint16_t switch1;

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

void pwm_motor(int duty)
{
    start_motor_clockwise();
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
