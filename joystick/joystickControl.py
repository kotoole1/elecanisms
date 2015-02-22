from hellousb import hellousb
import time 


class MotorState(object):
    ON = 0
    PWM = 1
    OFF = 2
    FREE = 3
    SPRING = 4
    DAMPER = 5
    TEXTURE = 6
    WALL = 7

if __name__ == '__main__':
    usb = hellousb()
    while True:
        key = raw_input()
        if key is 'z':
            usb.reset() 
        if key is 's':
            usb.set_mode(MotorState.SPRING)
        if key is 'd':
            usb.set_mode(MotorState.DAMPER)
        if key is 't':
            usb.set_mode(MotorState.TEXTURE)
        if key is 'w':
            usb.set_mode(MotorState.WALL)
        if key is 'g':
            usb.get_angle()
        # time.sleep(.1)