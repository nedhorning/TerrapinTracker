/* Gnat Asset Tracker contains:
   CMWX1ZZABZ (STM32L082 and SX1276)
   BMA400 accelerometer sensor
   MAX M8Q Concurrent GNSS engine

   https://hackaday.io/project/25790-asset-tracker

   Prints the Device EUI from the Gnat to the Serial monitor
*/
#include <STM32L0.h>
#include "LoRaWAN.h"

uint32_t UID[3] = {0, 0, 0}; 
char buffer[32];

void setup()
{
  /* Enable USB UART */
  Serial.begin(115200);
  delay(4000);
  Serial.println("Serial enabled!");

  /* Get LoRa/LoRaWAN ID for SX1276 */
  STM32L0.getUID(UID);
  Serial.print("STM32L0 MCU UID = 0x"); Serial.print(UID[0], HEX); Serial.print(UID[1], HEX); Serial.println(UID[2], HEX); 

  LoRaWAN.getDevEui(buffer, 18);
  Serial.print("STM32L0 Device EUI = "); Serial.println(buffer); 
  
 }

void loop()
{
}
