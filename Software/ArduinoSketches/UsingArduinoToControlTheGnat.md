Program a Gnat LoRa node

The Gnat LoRa/GNSS asset tracker is programmed using Arduino. At this
point we are not using all of the sensors on the Gnat in an effort to
improve battery life and reduce the size of data packets.

The Arduino IDE can be downloaded from the Arduino web site:
<https://www.arduino.cc/>. On that website you will also find
information about using Arduino. Software libraries and information
required to configure the Gnat for use within the Arduino environment
can be found here:
<https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0>. Note that the
Gnat requires the purchase and installation of a LoRa antenna, GNSS
antenna before powering with a battery or USB.

Once the Arduino environment is properly configured you need to specify
the correct board for the Gnat (Gnat-Lo82CZ). Instruction on how to do
that can be found at the GrumpyOldPizza GitHub repository noted above

You can download a sketch from from Gnat GitHub repository
(<https://github.com/kriswiner/CMWX1ZZABZ/tree/master/Gnat>) or from the
TerrapinTracker GitHub repository
(<https://github.com/nedhorning/TerrapinTracker>) in the Sketches
directory. You can use the GetDevEUI sketch from the TerrapinTracker
GitHub repository to get the Dev EUI that is necessary for monitoring
data sent from a LoRa node such as the Gnat.
