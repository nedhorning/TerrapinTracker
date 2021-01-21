# Tracker Firmware Installation
﻿These sketches are largely based on the demo sketch written by the Gant’s developer: https://github.com/kriswiner/CMWX1ZZABZ/tree/master/Gnat

## Download and Configure Arduoino
The Gnat LoRa/GNSS asset tracker is programmed using Arduino. The Arduino IDE can be downloaded from the Arduino web site: <https://www.arduino.cc/>.

Software libraries and information required to configure the Gnat for use within the Arduino environment can be found here:
<https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0>.

Once the Arduino environment is properly configured you need to specify the correct board for the Gnat (Gnat-Lo82CZ). Instruction on how to do that can be found at the GrumpyOldPizza GitHub repository noted above.

[add instructions here for clarity]

For maximum power savings you can run the Gnat at a CPU speed of 4.2 MHz. To set the CPU speed in the Arduino GUI go to Tools => CPU Speed and select 4.2 MHz. If you want to see serial output form "print" statements then you need to select 32 MHz for the clock speed. To reload a sketch after using 4.2 MHz clock speed you will need to reset the Gnat using the instructions above.

## Sketch descriptions
At this point we are not using all of the sensors on the Gnat in an effort to improve battery life and reduce the size of data packets.

[add descriptions of options which can be changed in sketches, consider rewriting and including only the lastest sketch.]

### TurtleTracker_TideArrayNumSatsDownstreamComm
This sketch is similar to TurtleTracker_TideArray but it adds the capability to send downstream messages to the tracker to update variables. Inside the folder is a key that shows the fport/variable assignments.

### GetDevEUI (./tools)
This skectch reads the Device EUI from a Gnat and prints it to the Serial Monitor

### TurtleTracker_TideTiming (./superceeded)
This sketch uses a reference high tide time and a constant period between high tide times to determine when to start and stop high/low tide cycling rates. This can have deviations of over one hour when compared with actual high tide times for certain times of the year.

###TurtleTracker_TideArray (./superceeded)
This sketch uses an array high tide times to determine when to start and stop high/low tide cycling rates. See the TideGuide in the UserGuides directory for information about how to create the high tide times array.

## Troubleshooting
Sometimes a faulty sketch or a lower clock speed can break the normal USB Serial based integration into the Arduindo IDE. In this case plugin the board using USB cable and toggle the RESET button (next to “GND” black wire) while holding down the BOOT button  (next to the reset button) and program a known to be working sketch to go back to a working USB Serial setup.
