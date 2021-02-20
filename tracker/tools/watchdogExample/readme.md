# Demonstration of STM32 Independant Watchdog Timer (IWDG) watchdog timer setup using registers directly. No library needed.

The IWDG module is used in production systems to generate a reset signal to the CPU in case some catastrophic event causes the software to become "stuck" or unresponsive.

The GNAT uses the STM32L082xx2 MCU which has an independant watchdog timer implemented as a 12-bit count down timer. The reference manual is [here](https://www.st.com/resource/en/reference_manual/dm00108281-ultralowpower-stm32l0x2-advanced-armbased-32bit-mcus-stmicroelectronics.pdf).
 The registers set in the code are on page 595. 

The watchdog clock comes from dividing the LSI clock. For the GNAT / STM32L082xx2 it is about 37kHz. The prescaler is defined in the code by the `iwdg_prescaler` ENUM as follows:

```
typedef enum iwdg_prescaler {
  IWDG_PRE_4 = 0x0,     /**< Divide by 4 */
  IWDG_PRE_8 = 0x1,     /**< Divide by 8 */
  IWDG_PRE_16 = 0x2,    /**< Divide by 16 */
  IWDG_PRE_32 = 0x3,    /**< Divide by 32 */
  IWDG_PRE_64 = 0x4,    /**< Divide by 64 */
  IWDG_PRE_128 = 0x5,   /**< Divide by 128 */
  IWDG_PRE_256 = 0x6    /**< Divide by 256 */
} iwdg_prescaler;
```

The hex value can also be used directly e.g. `0x6`. However using an enum is human readable and shows the options without needing the reference docs.

The watchdog counts down from a programmer defined 12 bit value. This is set in `iwdg_init(prescaler, counterStartValue)`. It cannot be higher than 4095. If the counter it reaches zero, it triggers a hard reboot.

For example, we want a very long watchdog period for testing. We select a prescaler of 256 and a maximum counter value of 1000. The `IWDG_PRE_256` tag selected the hex value 0x6 from the `iwdg_prescale` enum.

```
iwdg_init(IWDG_PRE_256, 1000);
```

For the GNAT / STM32L082 this gives a watchdog clock of ~144 Hz (~37 kHz / 256). 1000 clocks gives a reboot in ~ 6.9 sec. For production systems the reboot should be triggered when the process has taken too long to still be working as intended.

To prevent the hard reboot, the counter can be reset before it reaches zero by calling `iwdg_feed`. When this is placed in the software loop the hard reboot only happens if the device is stuck for some reason providing a failsafe.

For the demo, comment out `iwdg_feed` in the loop for demonstration. The serial output will count until the watchdog is triggered after ~ 7 seconds, when the counter is reset using `iwdg_feed` it will keep running.

# Demonstration of non-volotile power cycle counter
A power cycle counter is implemented in non-volotile memory to track each power cycle. This is useful for autonomous devices as would not otherwise know about resets. This is the most simple form of error handling. 

Use initaliseResetCounter.ino (in tools folder) to inialise and reset the counter. It also sets the hardware version of the board which survives when the device is programmed.

## To do:
- Log power cycles and startup message via LoRa as an error tracking feature
