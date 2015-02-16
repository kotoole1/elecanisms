from hellousb import hellousb

if __name__ == '__main__':
    usb = hellousb()
    while True:
        raw_input()
        usb.get_pin()
