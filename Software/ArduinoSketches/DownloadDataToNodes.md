**Adding the ability to send data from LoRaWAN gateways to nodes**

With the node – gateway setup we are using we use Class A LoRaWAN nodes since those use the lease amount of power. One of the strategies Class A nodes use to conserve energy is to turn on their receiver very infrequently and for very brief periods. The only time a Class A node is “listening” for a download signal is for two very brief periods (~1 second) immediately after an upstream message is transmitted to the gateway. If there is a data message queued on the gateway to be sent to a specific node it will be sent during those brief receive windows. 

The message that is sent must be encoded using base64 encoding. An easy way to do this using a terminal window is to use the following printf command:
printf 33 | base64     -  This converts 33 to base64  (MzM=). Sending these commands can be done using the ChirpStack web API. To access this functionality open the API then click on Applications then click on the application containing the node you want to send to package to. Next, click on the node you want to send the data to. This will open a page with “Enqueue downlink payload” follow by a “Port” text box, “Confirmed downlink” checkbox and a “Base64 encoded string” text box. 

You can enter the base64 encoded package ((MzM= in our example above). If you want a confirmation message (displayed in the “DEVICE DATA” page) check the box. The port (a number from 1 - 253) can be use by node to determine what to do with the data package. For our purposes the port is use to specify which variable will be modified using the key below. When you hit the “ENQUEUE PAYLOAD” button the data package will be queued and will be sent the next time the node transmits data to the gateway. Only one message can be sent at a time so if you have more than one message queued one package (base64 encoded string) will be sent each time a node transmits data. 

In order to receive a download data package the Arduino script on the node need to have this line in the setup portion of the script:

``` C++
LoRaWAN.onReceive(receiveCallback);
```

The above line instructs the LoRa radio to run the decode function (receiveCallback) when a data package is received. An example function is pasted at the end of this document.

This is an example of fport and variable pairs. In the following list the number is the fport value to be used to update the following variable with the data in the downloaded packet. For example if I wanted to change minSNR to 33 I would end 8 for fport and MzM= for the base64 encoded string.

**fport and variable**
    1. maxEPE  // Maximum EPE value 
    2. myAcqTime  // Seconds to acquire a GPS fix
    3. myPeriodHT  // Repeat period in seconds for GPS fixes during high tide
    4. myPeriodLT  // Repeat period in seconds for GPS fixes during low tide
    5. minutesBeforeHighTide  // Number of minutes before high tide to start high frequency GPS
    6. minutesAfterHighTide   // Number of minutes after high tide to stop high frequency GPS
    7. minWithSignalCount   // Minimum number of satellites to receive signal within specified length of time
    8. minSNR //  minimum signal to noise ratio to count as a received signal
    9. maxSignalSearchTime //  milliseconds to receive a signal from minWithSignalCount number of satellites 

``` C++
Example Arduino decode function:
void receiveCallback(void)
{
  if (LoRaWAN.parsePacket())
  {
    uint32_t size;
    uint8_t data[256];

    size = LoRaWAN.read(&data[0], sizeof(data));
    if (size)
    {
      data[size] = '\0';
      switch(LoRaWAN.remotePort()) 
      {
        case 1:
          maxEPE = (uint16_t)strtoul((const char*)&data[0], NULL, 10);
          break;
        case 2:
          myAcqTime = (uint32_t)strtoul((const char*)&data[0], NULL, 10);
          break;
        case 3:
          myPeriodHT = (uint32_t)strtoul((const char*)&data[0], NULL, 10);
          myTimerResume.restart(1000, (myPeriodHT * 1000));
          break;
        case 4:
          myPeriodLT = (uint32_t)strtoul((const char*)&data[0], NULL, 10);
          myTimerResume.restart(1000, myPeriodLT * 1000);
          break;
        case 5:
          minutesBeforeHighTide = (uint32_t)strtoul((const char*)&data[0], NULL, 10);
          durationHighFrequency = (minutesBeforeHighTide + minutesAfterHighTide) * 60 * 1000;
          break;
        case 6:
          minutesAfterHighTide = (uint32_t)strtoul((const char*)&data[0], NULL, 10);
          durationHighFrequency = (minutesBeforeHighTide + minutesAfterHighTide) * 60 * 1000;
          break;
        case 7:
          minWithSignalCount = (uint8_t)strtoul((const char*)&data[0], NULL, 10);
          break;
        case 8:
          minSNR = (uint8_t)strtoul((const char*)&data[0], NULL, 10);
          break;
        case 9:
          maxSignalSearchTime = (uint32_t)strtoul((const char*)&data[0], NULL, 10);
          break;
      }
    }
  }
}
```
