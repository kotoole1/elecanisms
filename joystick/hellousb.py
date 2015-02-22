import usb.core

class hellousb:

    def __init__(self):
        self.GET_PIN = 0
        self.GET_ANGLE = 1
        self.RESET = 2
        self.SET_MODE = 3
        self.dev = usb.core.find(idVendor = 0x6666, idProduct = 0x0003)
        if self.dev is None:
            raise ValueError('no USB device found matching idVendor = 0x6666 and idProduct = 0x0003')
        self.dev.set_configuration()

    def close(self):
        self.dev = None

    def set_mode(self, mode):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_MODE, int(mode))
        except usb.core.USBError:
            print "Could not send HELLO vendor request."

    def reset(self):
        try:
            self.dev.ctrl_transfer(0x40, self.RESET)
        except usb.core.USBError:
            print "Could not send SET_VALS vendor request."        

    def get_pin(self):
        try:
            self.dev.ctrl_transfer(0x40, self.GET_PIN)
        except usb.core.USBError:
            print "Could not send HELLO vendor request."

    def get_angle(self):
        try:
            self.dev.ctrl_transfer(0x40, self.GET_ANGLE)
        except usb.core.USBError:
            print "Could not send HELLO vendor request."

    def set_vals(self, val1, val2):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_VALS, int(val1), int(val2))
        except usb.core.USBError:
            print "Could not send SET_VALS vendor request."

    def get_vals(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_VALS, 0, 0, 4)
        except usb.core.USBError:
            print "Could not send GET_VALS vendor request."
        else:
            return [int(ret[0])+int(ret[1])*256, int(ret[2])+int(ret[3])*256]

    def print_vals(self):
        try:
            self.dev.ctrl_transfer(0x40, self.PRINT_VALS)
        except usb.core.USBError:
            print "Could not send PRINT_VALS vendor request."
            