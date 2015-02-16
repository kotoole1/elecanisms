#ifndef _MOTOR_H_
#define _MOTOR_H_

typedef enum {
    ON,
    PWM,
    OFF,
    FREE
} MotorState;

extern MotorState motor_state;
extern volatile uint16_t switch1;

void stop_motor();
void start_motor_clockwise();
void freespin_motor();
void pwm_motor(int duty);
void init_motor();

#endif
