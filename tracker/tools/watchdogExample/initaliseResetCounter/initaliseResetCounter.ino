// Tool to initalise data stored on flash memory for STM32
// Joshua C Taylor 2021

#include <EEPROM.h>
int resetCountAdr = 0;
byte resetCounter;
byte hardwareVerAdr = 1;
byte hardwareVer = 1;

void setup() {
   Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("\n \n \n Flashing tool version 1.0 \n");
  Serial.print("Previous power cycles: ");
  Serial.println(EEPROM.read(resetCountAdr), DEC);

  // initalise the power cycle counts to zero
  EEPROM.write(resetCountAdr, 0);
  Serial.print ("power cycle counter set to: ");
  Serial.println(EEPROM.read(resetCountAdr), DEC);
  
  // set the hardware version
  Serial.print ("Hardware Version set to: ");
  EEPROM.write(hardwareVerAdr, hardwareVer);
  Serial.println(EEPROM.read(hardwareVerAdr), DEC); 
  Serial.println();
  
}

void loop() {
  // nothing here
}
