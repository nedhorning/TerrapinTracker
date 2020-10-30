Notes for the Gnat

URL: <https://www.tindie.com/products/tleracorp/gnat-loragnss-asset-tracker/>

Information about adding the board manager to Arduino is here: <https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0>

**Can’t upload sketch because port not found**
Sometimes a faulty sketch can render the normal USB Serial based integration into the Arduindo IDE not working. In this case plugin the board using USB cable and toggle the RESET button (next to “GND” black wire) while holding down the BOOT button  (next to the reset button) and program a known to be working sketch to go back to a working USB Serial setup.

**Setting the CPU clock speed**
For testing and to see the serial output set the CPU speed to 32 MHz but to deploy in the field use a speed of 4.2 MHz. Set the CPU speed in the Arduino GUI under Tools => CPU Speed. 

**The original sketch from the developer requires some edits before running:**
An object of the TimerMillis class has 3 variables. The first is the name of the callback function that will be called. Second is the delay which is the number of milliseconds to wait after a sketch is started before calling the callback function. The last is a period which is the number of milliseconds between subsequent callbacks. Below is an example from the sketch and it is important that these values are set the way you want them. For testing it’s probably best to have short periods but longer times would be used when it is deployed in the field to save power.

``` java
LoRaTimer.start(callbackLoRaTx, 300000, 600000);      //  10 minute period, delayed 5 minutes
NoMotionActivityTimer.start(callbackNoMotionActivity, 100000, 7200000);    // low  freq (two hours)
InMotionActivityTimer.start(callbackInMotionActivity, 100000, 600000);    // high freq (one minute) 
```

You need to specify the network sub-band on his line:

``` java
LoRaWAN.setSubBand(2); // 1 for MTCAP, 2 for TT gateways
```

These three variables need to be set for each device and application:

``` java
const char *appEui = "70B3D57ED000964D";
const char *appKey = "7DE66B18F7105B19A1427AFEB2514597";
const char *devEui = "3739323261377909";
```

If you don't have your devEui you can run the sketch with the serial terminal open and the devEui for your Gnat will be displayed when the program starts and you can copy and paste it into the sketch then run it again. I got the appEui value from ChirpStack application when I set up a device. The appEui isn’t needed when using ChurpStack but is necessary if you are using The Things Network.
