from hellousb import hellousb

if __name__ == '__main__':
    usb = hellousb()
    while True:
        key = raw_input()
        if key == 'z':
        	usb.reset()	
        usb.get_angle()
