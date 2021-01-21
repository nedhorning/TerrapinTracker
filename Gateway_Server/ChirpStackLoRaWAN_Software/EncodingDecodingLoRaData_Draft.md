07/10/20

**Encoding and decoding LoRa data using Arduino and ChirpStack**

In this guide I will describe the following two methods to encode and decode LoRa data. 
 - CayenneLPP – Simple to use but quite constrained with regard to data types
 - Custom script – Requires writing encode and decode software but it is very flexible with regard to data types that can be broadcast 

I will provide information to implement both approaches on a LoRa system using Arduino nodes and a ChirpStack gateway with the Application Server installed. The Arduino code is written for the Gnat LoRa tracker using the ArduinoCore-stm32l0-master CayenneLPP library but the basic approach should work for other CayenneLPP implementations in the Arduino environment.

CayenneLPP encoder for Arduino
Information about the different fields (data types) and their constraints can be found in the Cayenne Docs: https://developers.mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload. In the Arduino script you need to initialize an CayenneLPP object specifying the maximum payload size in bytes:

	``` javascript
	CayenneLPP myLPP(64);
	```

A typical implementation in an Arduino script is:

``` javascript
if (!LoRaWAN.busy() && LoRaWAN.joined())
     {
        myLPP.reset();   // Reset the buffer
        myLPP.addGPS(1, Lat, Long, Alt);   // Add a GPS coordinate
        myLPP.addAnalogInput(2, EPE);    // Add a value of float data type
        myLPP.addAnalogInput(3, timeToFix); 
        myLPP.addDigitalInput(4, freshGPS);   // Add a value of integer data type
        myLPP.addDigitalInput(5, isHighTide);       
        LoRaWAN.sendPacket(myLPP.getBuffer(), myLPP.getSize());
     }
```

In the above example each variable is assigned to a specific CayenneLPP data type that is specified by the method name. Each method has an argument to specify the channel parameter which is a key that will be used to identify that variable when it is decoded.  

CayenneLPP decoder for ChirpStack Application Server 
To set up the CayenneLPP decoder you need to open the ChirpStack Application Server web interface and then click on the “Device-profiles” link on the left. When you create or edit a device-profile click on the “CODEC” tab near the top of the window. Using the drop-down menu select “Cayenne LPP” then click on “UPDATE DEVICE-PROFILE” to save the settings. Your decoded data should now be visible in the Application interface (“using the “DEVICE DATA” tab) and those decoded data will be sent to any integration you set up.

Custom script encoder for Arduino
Encoding data for transmission by a LoRa node requires that all data types are converted to byte arrays. There are different ways to do that but the approach I take is to convert floating point data to integers by multiplying by a factor that will give me the precision I need for my application. Here is an example that converts a floating point latitude value to an integer: 

``` javascript
float latitude = 45.385043   
int32_t latInt 

latInt = round(Lat * 1000000);   //  In the main loop of the script
```

Next, that integer needs to be converted to a byte array using bitwise operations. The length of the byte array depends on the number of bytes required for the data type you will use for this variable when you decode it. In my example with latitude I want to decode this into a 4-byte signed integer. I prefer using separate byte arrays for each variable since it is easier for me to read but you could put all values into a single array if you wanted your code to be more compact.

``` javascript
byte latArray[4];  // Define 4-byte array near the beginning of the Arduino script

latArray[0] = (latInt & 0xFF000000) >> 24;
latArray[1] = (latInt & 0x00FF0000) >> 16;
latArray[2] = (latInt & 0x0000FF00) >> 8;
latArray[3] = (latInt & 0x000000FF);
```

If you wanted to convert a 2-byte integer (int16_t) named EPE you could do this:

``` javascript
byte EPE_Array[2];   // Define 2-byte array near the beginning of the Arduino script

EPE_Int = round(EPE * 100);
EPE_Array[0] = (EPE_Int & 0xFF00) >> 8;
EPE_Array[1] = (EPE_Int & 0x00FF);
```

Variables that have a datatype that is represented by a single byte (e.g., uint8_t, int8_t, bool) do not need to be converted to a byte array. 

Next, all of the bytes (in my example there are 18 bytes) need to be written to a buffer that will be sent by the LoRa radio. Here is an example using a few different variables:

		``` javascript
        LoRaWAN.beginPacket();
        LoRaWAN.write(latArray[0]);
        LoRaWAN.write(latArray[1]);
        LoRaWAN.write(latArray[2]);
        LoRaWAN.write(latArray[3]);
        LoRaWAN.write(longArray[0]);
        LoRaWAN.write(longArray[1]);
        LoRaWAN.write(longArray[2]);
        LoRaWAN.write(longArray[3]);
        LoRaWAN.write(EPE_Array[0]);
        LoRaWAN.write(EPE_Array[1]);
        LoRaWAN.write(timeToFixArray[0]);
        LoRaWAN.write(timeToFixArray[1]);
        LoRaWAN.write(freshGPS);
        LoRaWAN.write(isHighTide);
        LoRaWAN.write(timeArray[0]);
        LoRaWAN.write(timeArray[1]);
        LoRaWAN.write(timeArray[2]);
        LoRaWAN.write(timeArray[3]);
        LoRaWAN.endPacket();
		```

As noted above, this would be more compact if all byktes were stored in a single array and you used a loop to write them to the buffer.

Custom script decoder for ChirpStack Application Server 
To set up the custom script decoder you need to open the ChirpStack Application Server web interface and then click on the “Device-profiles” link on the left. When you create or edit a device-profile click on the “CODEC” tab near the top of the window. Using the drop-down menu select “Custom JavaScript codec functions”.

The “Custom JavaScript codec functions” page has two text boxes, the top one is for decoding code and the bottom is for encoding code. We will only use the top box. 

The following elements are required:
 - The function signature must be: function Decode(fPort, bytes) where “bytes” is a byte array with all of the data recived from the node
 - The individual bytes need to transformed to the intended data type which requires bitwise operations (bit shifting) and when floating point values are being decoded they also need to be divided by the appropriate factor


Below is an example Jave Script function that can be used to decode the encoded example above. In this example I put each byte into an array for a specific variable and then send that to the “intFromBytes” function.

``` javascript
function Decode(fPort, bytes) {
  function intFromBytes( x ){
        var val = 0;
        for (var i = 0; i < x.length; ++i) {        
            val += x[i];        
            if (i < x.length-1) {
                val = val << 8;
            }
        }
        return val;
    }
  var latArray = [bytes[0],bytes[1],bytes[2],bytes[3]];
  var latitude = intFromBytes(latArray)/1000000;
  var longArray = [bytes[4],bytes[5],bytes[6],bytes[7]];
  var longitude = intFromBytes(longArray)/1000000;
  var altArray = [bytes[8],bytes[9],bytes[10],bytes[11]];
  var altitude = intFromBytes(altArray)/1000000;
  var EPE_Array = [bytes[12], bytes[13]];
  var EPE = intFromBytes(EPE_Array)/100;
  var timeToFixArray = [bytes[14], bytes[15]];
  var timeToFix = intFromBytes(timeToFixArray);
  var freshGPS = bytes[16];
  var isHighTide = bytes[17];
  var time_Array = [bytes[18],bytes[19],bytes[20],bytes[21]];
  var time = intFromBytes(time_Array);
  var vbat_Array = [bytes[22], bytes[23]];
  var vbat = intFromBytes(vbat_Array)/1000;
  return {"latitude":latitude,"longitude":longitude,"altitude":altitude,"epe":EPE,"timeToFix":timeToFix,"freshGPS":freshGPS,"isHighTide":isHighTide,"time":time, "vbat":vbat};
}
```

 
To save the settings click on “UPDATE DEVICE-PROFILE”. Your decoded data should now be visible in the Application interface (“using the “DEVICE DATA” tab) and those decoded data will be sent to any integration you set up.
