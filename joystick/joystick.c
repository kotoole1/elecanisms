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

volatile uint16_t read;
volatile uint16_t lastRead;
volatile uint16_t lastLastRead;
double partialFlips;
double firstPartialFlips;

int16_t rawDiff;
int16_t lastRawDiff;
int16_t lastRawOffset;
int16_t rawOffset;
int16_t flipNumber;

uint16_t flipThreshold = 200;
volatile uint16_t numPassed = 0;
volatile double joystickAngle = 0.0;

bool flipped;

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

double joystickPosition(uint16_t read, int16_t flipNumber) {
    partialFlips = (double) (read / 1024);
    return (double) (flipNumber + partialFlips - firstPartialFlips);
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
    printf("%d\n", joystickAngle);
//     // printf("Hello World!\n");
//     BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
//     BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit    
}

void VendorRequests(void) {
    WORD temp;

    switch (USB_setup.bRequest) {
        case GET_PIN:
            printf("hello");
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;        
        case GET_ANGLE:
            printAngle();
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
    while (USB_USWSTAT!=CONFIG_STATE) {     // while the peripheral is not configured...
        ServiceUSB();                       // ...service USB requests
    }

    timer_setPeriod(&timer1, 0.0001);
    timer_start(&timer1);
    timer_setPeriod(&timer2, 0.01);
    timer_start(&timer2);

    read = pin_read(&A[5]) >> 6;
    lastRead = read;
    lastLastRead = read;
    firstPartialFlips = read / 1024;
    while (1) {
        ServiceUSB();                       // service any pending USB requests
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            read_pins();
            updateAngle();
        }
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            switch_state();
        }
    }
}
