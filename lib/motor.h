#include <stdbool.h> 

#ifndef _MOTOR_H_
#define _MOTOR_H_

typedef enum {
    ON,
    PWM,
    OFF,
    FREE,
    SPRING,
    DAMPER,
    TEXTURE,
    WALL,
} MotorState;

extern MotorState motor_state;
extern volatile uint16_t switch1;

void stop_motor();
void start_motor_clockwise();
void freespin_motor();
void pwm_motor(uint16_t duty, bool direction);
void init_motor();

#endif
