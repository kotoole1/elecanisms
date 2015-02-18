#include <p24FJ128GB206.h>
#include <math.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "pin.h"
#include "oc.h"
#include "uart.h"
#include "motor.h"
#include "analogRead.h"
#include "usb.h"
#include <stdio.h>
#include <stdbool.h> 

#define GET_PIN       0
#define GET_ANGLE     1
#define RESET         2

volatile uint16_t read;
volatile uint16_t lastRead;
volatile uint16_t lastLastRead;
double partialFlips;
double firstPartialFlips;
uint16_t motorValue;
bool motorDirection;
double buffer = 0.2;

int16_t rawDiff;
int16_t lastRawDiff;
int16_t lastRawOffset;
int16_t rawOffset;
int16_t flipNumber;

uint16_t flipThreshold = 200;
volatile uint16_t numPassed = 0;
volatile double joystickAngle = 0.0;

bool flipped;


double joystickPosition(uint16_t read, int16_t flipNumber) {
    partialFlips =  ((double) read) / 1024;
    return (flipNumber + partialFlips - firstPartialFlips);
}

void updateAngle(void) {
    // Calculate differences between subsequent MR sensor readings
    read = pin5;
    rawDiff = read - lastRead;
    lastRawDiff = read - lastLastRead;
    rawOffset = abs(rawDiff);
    lastRawOffset = abs(lastRawDiff);
    
    // Update position record-keeping vairables
    lastLastRead = lastRead;
    lastRead = read;
    
    // Keep track of flips over 180 degrees
    if((lastRawOffset > flipThreshold) && (!flipped)) { // enter this anytime the last offset is greater than the flip threshold AND it has not just flipped
        if(lastRawDiff > 0) {        // check to see which direction the drive wheel was turning
            flipNumber--;              // cw rotation 
        } else {                     // if(rawDiff < 0)
            flipNumber++;              // ccw rotation
        }
        // if(rawOffset > flipThreshold) { // check to see if the data was good and the most current offset is above the threshold
        //     updatedPos = read + (flipNumber * rawOffset); // update the pos value to account for flips over 180deg using the most current offset 
        //     tempOffset = rawOffset;
        // } else {                     // in this case there was a blip in the data and we want to use lastactualOffset instead
        //     updatedPos = read + (flipNumber * lastRawOffset);  // update the pos value to account for any flips over 180deg using the LAST offset
        //     tempOffset = lastRawOffset;
        // }
        flipped = true;            // set boolean so that the next time through the loop won't trigger a flip
    } else {                       // anytime no flip has occurred
        flipped = false;
    }
    joystickAngle = joystickPosition(read, flipNumber) ; // need to update pos based on what most recent offset is 
}

void printAngle() {
    printf("%f, %d, %f\n", joystickAngle, motorValue, fabs(joystickAngle)); 
}

void updateMotor(void){
    switch (motor_state) {
        case SPRING:
            if (joystickAngle > buffer) {
                motorValue = (uint16_t)((joystickAngle - buffer )* 100);
                motorDirection = false;
                pwm_motor(motorValue, motorDirection);

            }
            else if (joystickAngle < -buffer){
                motorValue = (uint16_t)((-joystickAngle + buffer) * 100);                
                motorDirection = true;
                pwm_motor(motorValue, motorDirection);

            }
            else {
                freespin_motor();              
            
            }
            break;
    }
}



void reset(void) {

    read = pin_read(&A[5]) >> 6;
    firstPartialFlips = ((double) read) /1024;
    lastRead = read;
    lastLastRead = read;
    flipNumber = 0;
    joystickAngle = 0.0;
}

void VendorRequests(void) {
    WORD temp;

    switch (USB_setup.bRequest) {
        case GET_PIN:
            printf("Hello Pin");
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;        
        case GET_ANGLE:
            printAngle();
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;
        case RESET:
            reset();
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;
    }
}

void VendorRequestsIn(void) {
    switch (USB_request.setup.bRequest) {
        default:
            USB_error_flags |= 0x01;                    // set Request Error Flag
    }
}

void VendorRequestsOut(void) {
    switch (USB_request.setup.bRequest) {
        default:
            USB_error_flags |= 0x01;                    // set Request Error Flag
    }
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
    InitUSB();
    reset();

    while (USB_USWSTAT!=CONFIG_STATE) {     // while the peripheral is not configured...
        ServiceUSB();                       // ...service USB requests
    }

    timer_setPeriod(&timer1, 0.0005);
    timer_start(&timer1);

    while (1) {
        ServiceUSB();                       // service any pending USB requests
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            read_pins();
            updateAngle();
            updateMotor();
        }
    }
}
