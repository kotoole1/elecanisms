Usage
=======

Ensure the proper software is installed according to this documentation:
http://elecanisms.olin.edu/handouts/20150120_BuildTools.pdf

CD to the directory in which the blink code is located.
Connect the PIC via a USB port and make sure you have usb.core permissions

Run the command in the linux terminal:
    scons && python ../site_scons/bootloadercmd.py -i blink.hex -w -n
This will compile your code into a .hex file, write the code to the PIC and verify that it worked correctly. 

If the system is working properly than the three LDS of the pic should be flashing at different rates

