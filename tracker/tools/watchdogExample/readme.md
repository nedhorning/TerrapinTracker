# Demonstration of STM32 Independant Watchdog Timer (IWDG) watchdog timer setup using registers directly. No library needed.

The GNAT uses the STM STM32L082xx2 MCU. This has an independant watchdog timer implemented as a 12-bit count down timer.

The time for each watchdog clock cycle time is by deviding the LSI clock (about 37kHz for STM32L082xx2) devided by a prescaler. 

The watchdog counts down from a 12 bit value. If it reaches zero, it triggers a hard reset.

To prevent the reset the counter is reset to its starting value by calling iwdg_feed in the software loop. This prevents the device getting stuck due to software or hardware issues causing it to hang.

For the demo, comment out `iwdg_feed` in the loop for demonstration. The serial output will count until the watchdog is triggered after ~ 7 seconds, when the counter is reset using `iwdg_feed` it will keep running.

## To do:
- Add a power cycle counter using non-volotile flash / EEPROM to show if there is a recurring problem with the software or hardware
- Log power cycles via LoRa as an error tracking feature
