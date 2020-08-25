These sketches are largely based on the demo sketch written by the Gant’s developer: https://github.com/kriswiner/CMWX1ZZABZ/tree/master/Gnat

Information necessary to install the Gnat board and required libraries to run these sketches in Arduino can be found here: https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0. 

Sometimes a faulty sketch or a lower clock speed can break the normal USB Serial based integration into the Arduindo IDE. In this case plugin the board using USB cable and toggle the RESET button (next to “GND” black wire) while holding down the BOOT button  (next to the reset button) and program a known to be working sketch to go back to a working USB Serial setup.

For maximum power savings you can run the Gnat at a CPU speed of 4.2 MHz. To set the CPU speed in the Arduino GUI go to Tools => CPU Speed and select 4.2 MHz. If you want to see serial output form "print" statements then you need to select 32 MHz for the clock speed. To reload a sketch after using 4.2 MHz clock speed you will need to reset the Gnat using the instructions above.

Sketch descriptions
TurtleTracker_TideArray
This sketch uses an array high tide times to determine when to start and stop high/low tide cycling rates. See the TideGuide in the UserGuides directory for information about how to create the high tide times array. 

TurtleTracker_TideTiming
This sketch uses a reference high tide time and a constant period between high tide times to determine when to start and stop high/low tide cycling rates. This can have deviations of over one hour when compared with actual high tide times for certain times of the year. 

GetDevEUI
This skectch reads the Device EUI from a Gnat and prints it to the Serial Monitor 
