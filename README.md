Usage
=======

Ensure the proper software is installed according to this documentation:
http://elecanisms.olin.edu/handouts/20150120_BuildTools.pdf

CD to the directory in which the blink code is located.

Run the command in the linux terminal:
    scons
This will compile your code into a .hex file

CD out of the Blink folder to to the site_scons folder

In the site_scons folder, run the command: python bootloadergui.py

A graphical interface will open. With the board plugged in, first push the large black button near the red button and then the red button, while holding the black button. This puts the board into programming mode

Click File, import hex. Navigate to the blink folder and select the blink.hex file. Write it to the board, and then press the reset button. The board should then run the code.