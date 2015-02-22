from hellousb import hellousb
import time 

if __name__ == '__main__':
    usb = hellousb()
    while True:
        # key = raw_input()
        # if key == 'z':
        # 	usb.reset()	
        usb.get_angle()
        time.sleep(.1)