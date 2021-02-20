// Demonstration of STM32 Independant Watchdog Timer (IWDG) watchdog timer setup using registers directly. No library needed.
// based on forum post https://www.stm32duino.com/viewtopic.php
// Josh C Taylor, 2021

// The watchdog is a 12-bit counter which adds 1 for each watchdog clock cycle
// The watchdog clockcycle is set as the LSI clock (about 37kHz for STM32L082xx2 as used in GNAT) devided by a prescaler
// The watchdog counts down from a 12 bit value. If it reaches zero, it triggers a hard reset
// To prevent the reset the counter is reset to its starting value by calling iwdg_feed in the software loop
// This prevents the device getting stuck
// comment out iwdg_feed in the loop for demonstration
// Serial output will count until the watch dog is triggered, when reset it keeps running

// reference sheet for STM32L0x2
// https://www.st.com/resource/en/reference_manual/dm00108281-ultralowpower-stm32l0x2-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

// In addition a power cycle counter is implemented in non-volotile memory to track the resets. 
// This is useful for autonomous devices as we otherwise know what caused errors.
// use initaliseResetCounter.ino to initalaise and reset the counter and hardware version.

// To Do:
// - Log power cycles via LoRa as an error tracking feature

#include <EEPROM.h>
int resetCountAdr = 0;
byte resetCounter;
byte hardwareVerAdr = 1;
byte hardwareVer;
float softwareVer = 0.1;

int i = 0;  // counter for demonstration of software watchdog timer.

typedef enum iwdg_prescaler {
  IWDG_PRE_4 = 0x0,     /**< Divide by 4 */
  IWDG_PRE_8 = 0x1,     /**< Divide by 8 */
  IWDG_PRE_16 = 0x2,    /**< Divide by 16 */
  IWDG_PRE_32 = 0x3,    /**< Divide by 32 */
  IWDG_PRE_64 = 0x4,    /**< Divide by 64 */
  IWDG_PRE_128 = 0x5,   /**< Divide by 128 */
  IWDG_PRE_256 = 0x6    /**< Divide by 256 */
} iwdg_prescaler;

// function to reset timer
void iwdg_feed(void) {
  IWDG->KR = 0xAAAA;
}

// IWDG initalisation function
void iwdg_init(iwdg_prescaler prescaler, uint16_t reload) {
  // see page 595 of reference manual for STM32L082xx2 registers
  IWDG->KR = 0x5555; // 
  IWDG->PR = prescaler;
  IWDG->RLR = reload;
  IWDG->KR = 0xCCCC; // starts the watchdog
  IWDG->KR = 0xAAAA;
}

void setup()  {
  // update the non-volatile power cycle counter
  resetCounter = EEPROM.read(resetCountAdr);
  resetCounter ++;  // add one to the number of power cycles
  EEPROM.write(resetCountAdr, resetCounter);
  hardwareVer = EEPROM.read(hardwareVerAdr);  // get the hardware version from flash

  iwdg_init(IWDG_PRE_256, 1000); // start watchdog counter with 256 prescaler and reset when count = 1000
  // GNAT is STM32L082, LSI is approx 37 kHz
  // giving a watchdog clock of ~37 kHz / 256 = ~144 Hz  
  // 1000 clocks gives ~ 6.9 sec before reset.
  
  Serial.begin(115200);  ///< start usb serial at 115200 baud
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
   // display start up information on serial console
  Serial.println("\n \n \n");
  Serial.println("Turtle Tracker \n");
  Serial.print("hardware version: ");
  Serial.println(hardwareVer, DEC);
  Serial.print("software version: ");
  Serial.println(softwareVer);
  Serial.print("power cycles: ");
  Serial.println(resetCounter, DEC);
  Serial.println("\n \n \n");
  
}

void loop() {
  ++i;
  Serial.println(i);
  delay(1000);
  iwdg_feed();  // for the demo, comment this out and the MCU will hard reset every 6.9 sec
}

// to use it call iwdg_init() at setup with divider value and reload value you need, divider is for LSI clock, always check your mcu for actual value (for F103 LSI is 40khz)
// for example with 32khz LSI clock and divider 32, reload value 1000 will be one second to avoid reboot by calling iwdg_feed in the main loop
