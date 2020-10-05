Setting GNSS configuration for the Gnat

The GNSS receiver that is mounted on the Gnat has a number of options that can be set in the Arduino sketch. Below are the ones that are used in the Arduino sketch for tracking. We are working to better understand other settings that are possible. 

The second half of this document has functions that can be used to extract data from the GNSS receiver.

 ``` C++
 /* Initialize and configure GNSS */
  GNSS.begin(Serial1, GNSS.MODE_UBLOX, GNSS.RATE_1HZ); // Start GNSS
  while (GNSS.busy()) { } 
  
  GNSS.setConstellation((GNSSClass::GNSSconstellation)11); 
   while (GNSS.busy()) { } 

  GNSS.setAntenna(GNSS.ANTENNA_EXTERNAL);  
  while (GNSS.busy()) { }

  GNSS.enableWakeup();
 ```

The setting that lets you select the GNSS constelation(s) you want to access is:

 ``` C++
 GNSS.setConstellation((GNSSClass::GNSSconstellation)11); 
 ```

The “11” above is a bit mask that is used to select the GPS + GLONASS + Galileo constellations. 
The permissible GNSS constellation options along with their mask value are:

``` C++
1: GPS
2: GLONASS
4: BeiDou
8:  Galileo
9: GPS + Galileo
11: GPS + Galileo + GLONASS
13: GPS + Galileo + BeiDou
3: GPS + GLONASS
5: GPS + BeiDou
10: Galileo + GLONASS
12: Galileo + BeiDou
6: GLONASS + BeiDou
```

**Functions to extract GNSS data**
Many examples of the possible functions is in the example Arduino sketch, GPS.ino that is available from the GitHub repository: https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0/tree/master/libraries/GNSS
