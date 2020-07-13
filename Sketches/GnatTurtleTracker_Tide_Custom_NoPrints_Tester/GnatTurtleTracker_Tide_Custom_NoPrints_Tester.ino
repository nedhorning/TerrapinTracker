#include <STM32L0.h>
#include "LoRaWAN.h"
#include "TimerMillis.h"
#include "GNSS.h"
#include <RTC.h>
#include "CayenneLPP.h"

const char *appEui = "70B3D57ED0029388";
const char *appKey = "9C5E64056A46C2872DED008E20A6F95A";
const char *devEui = "383434306a379301";

CayenneLPP myLPP(64);

bool isHighTide = false;
bool initialized = false;
uint32_t myAcqTime =  30;  // Seconds to acquire a GPS fix
uint32_t myPeriodHT  = 60;   // Repeat period in seconds for GPS fixes during high tide
uint32_t myPeriodLT  = 60; // Repeat period in seconds for GPS fixes during low tide
uint32_t highTideCycle = 1224;  //Average number of seconds between high tides
//uint32_t highTideCycle = 44712;  //Average number of seconds between high tides
uint32_t minutesBeforeHighTide = 2; // Number of minutes before high tide to start high frequency GPS
uint32_t minutesAfterHighTide = 2; // Number of minutes after high tide to stop high frequency GPS

// Set high tide reference date/time using GMT. The date/time must be before the looger will be started but within the same year
uint16_t highTideRefMonth = 2; // January = 1 and December = 12
uint16_t highTideRefDay = 3;
uint16_t highTideRefHour = 17;  // 24 hour clock
uint16_t highTideRefMinute = 23;
uint16_t highTideRefSecond = 51;
uint32_t millisToHighTide;

uint16_t daysPerMonth[] = {0, 31,29,31,30,31,30,31,31,30,31,30}; // For leap years, do not include days for December
//uint16_t daysPerMonth[] = {0, 31,28,31,30,31,30,31,31,30,31,30}; // For non-leap years, do not include days for December

uint32_t totalSeconds;
uint32_t refHighTide;  // Number of second from beginning of year to reference high tide 

int32_t startHighFrequency; // Seconds until high tide
uint32_t durationHighFrequency = (minutesBeforeHighTide + minutesAfterHighTide) * 60 * 1000; //Duration of high frequency GPS looging in milliseconds
uint32_t timeNow, timeRTC; 
uint32_t startHT_Time = 0; //Time high tide monitoring cycle starts

GNSSLocation myLocation;

volatile bool isTracking = true;

TimerMillis myTimerSuspend;
TimerMillis myTimerResume;

uint32_t UID[3] = {0, 0, 0}; 

#define pps          4     // 1 Hz  MAX M8Q GNSS fix pulse

uint8_t freshGPS = 0;
uint16_t Hour = 1, Minute = 1, Second = 1, Millisec, Year = 1, Month = 1, Day = 1;
uint8_t hours = 12, minutes = 0, seconds = 0, years = 1, months = 1, days = 1;
uint32_t subSeconds, milliseconds;
bool ppsFlag = false, alarmFlag = true;
uint16_t fixType = 0;
int32_t latOut, longOut;
uint32_t startTimeToFix;
int16_t timeToFix = 0;

float Long, Lat, Alt, EPE;

uint8_t isHighTideByte;
int32_t latInt, longInt, altInt;
int16_t EPE_Int; 
byte latArray[4], longArray[4], altArray[4], EPE_Array[2], timeToFixArray[2], timeArray[4];

void setup()
{
  refHighTide = dateToSeconds(highTideRefMonth, highTideRefDay, highTideRefHour, highTideRefMinute, highTideRefSecond);
  
  /* Enable USB UART */
  Serial.begin(115200);
  delay(4000);

  /* Get LoRa/LoRaWAN ID for SX1276 */
  STM32L0.getUID(UID);

  pinMode(pps, INPUT); // select pps as input from MAX M8Q

   /* Initialize and configure GNSS */
  GNSS.begin(Serial1, GNSS.MODE_UBLOX, GNSS.RATE_1HZ); // Start GNSS
  while (GNSS.busy()) { } 

  GNSS.setConstellation(GNSS.CONSTELLATION_GPS_AND_GLONASS); // choose satellites
  while (GNSS.busy()) { } 

  GNSS.setAntenna(GNSS.ANTENNA_EXTERNAL);  
  while (GNSS.busy()) { }

  GNSS.enableWakeup();

  /* Set the RTC time */
  RTC.setHours(hours);
  RTC.setMinutes(minutes);
  RTC.setSeconds(seconds);
  
  // Set the RTC date
  RTC.setDay(days);
  RTC.setMonth(months);
  RTC.setYear(years);

 // set alarm to update the RTC periodically
  RTC.setAlarmTime(0, 0, 0);
  RTC.enableAlarm(RTC.MATCH_SS); // alarm once a minute
  RTC.attachInterrupt(alarmMatch);

  attachInterrupt(pps, CAMM8QintHandler, RISING);

  // Configuree LoRaWAN connection
    LoRaWAN.begin(US915);
    LoRaWAN.setADR(false);
    LoRaWAN.setDataRate(1);
    LoRaWAN.setTxPower(10);
    LoRaWAN.setSubBand(2); // 1 for MTCAP, 2 for TT gateways
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
}

void loop()
{
  /*GNSS*/
  if(GNSS.location(myLocation))
  {
  if (myLocation.fixType() != GNSSLocation::TYPE_NONE)
  {
      Hour   = myLocation.hours();
      Minute = myLocation.minutes();
      Second = myLocation.seconds();
      Year   = myLocation.year();
      Month  = myLocation.month();
      Day    = myLocation.day();
      
     if (myLocation.fixType() != GNSSLocation::TYPE_TIME)
      {
      Lat = myLocation.latitude();
      myLocation.latitude(latOut);
      Long = myLocation.longitude();
      myLocation.longitude(longOut);
      Alt = myLocation.altitude();
      EPE = myLocation.ehpe(); // use this as accuracy figure of merit
      
      // Wait until a good fix is acquired 
      if( (myLocation.fixType() != GNSSLocation::TYPE_2D) && (EPE <= 30.0f) && myLocation.fullyResolved()) 
      {
          if (!initialized) 
          {
              isTracking = false;
              initialized = true;
              freshGPS = 1;
              timeRTC = RTC.getEpoch();
              callbackSuspend();
              myTimerResume.start(callbackResume, 1000, myPeriodLT * 1000);
              timeNow = dateToSeconds(Month, Day, Hour, Minute, Second);
              startHighFrequency = (highTideCycle - ((timeNow - refHighTide) % highTideCycle)) - (minutesBeforeHighTide * 60);  
              if (startHighFrequency < 0)
              {
                  startHighFrequency = startHighFrequency + highTideCycle; // If in a high tide cycle wait until the next one
              }
              millisToHighTide = millis() + (startHighFrequency * 1000);
          } else
          {
             if (isTracking)
             {
                 isTracking = false;
                 freshGPS = 1;
                 timeToFix = round((millis() - startTimeToFix)/1000);  
                 myTimerSuspend.stop();
                 timeRTC = RTC.getEpoch();
                 callbackSuspend();
             }
          }
       }
    }
  } 

  } /* end of GNSS Location handling */

  /*RTC*/
  if (alarmFlag) { // update RTC output whenever there is a GNSS pulse
    alarmFlag = false;
    // Read RTC
    RTC.getDate(days, months, years);
    RTC.getTime(hours, minutes, seconds, subSeconds);
    milliseconds = ((subSeconds >> 17) * 1000 + 16384) / 32768;
        
    } // end of alarm section
    
    if (initialized)
    {
       if (isHighTide)
       {
           if ((millis() - startHT_Time) > durationHighFrequency)
           {
               isHighTide = false;
               beginLowTideCycle();
           }
       } else
       {
          if (millis() > millisToHighTide)
          {
            isHighTide = true;
            beginHighTideCycle();
          }
       }
   }
   STM32L0.stop();        // Enter STOP mode and wait for an interrupt
   
}  /* end of loop*/

void callbackLoRaTx(void)
{     
    if (!LoRaWAN.busy() && LoRaWAN.joined())
     {
        latInt = round(Lat * 1000000);
        latArray[0] = (latInt & 0xFF000000) >> 24;
        latArray[1] = (latInt & 0x00FF0000) >> 16;
        latArray[2] = (latInt & 0x0000FF00) >> 8;
        latArray[3] = (latInt & 0x000000FF);
        longInt = round(Long * 1000000);
        longArray[0] = (longInt & 0xFF000000) >> 24;
        longArray[1] = (longInt & 0x00FF0000) >> 16;
        longArray[2] = (longInt & 0x0000FF00) >> 8;
        longArray[3] = (longInt & 0x000000FF);
        altInt = round(Alt * 1000000);
        altArray[0] = (altInt & 0xFF000000) >> 24;
        altArray[1] = (altInt & 0x00FF0000) >> 16;
        altArray[2] = (altInt & 0x0000FF00) >> 8;
        altArray[3] = (altInt & 0x000000FF);
        
        EPE_Int = round(EPE * 100);
        EPE_Array[0] = (EPE_Int & 0xFF00) >> 8;
        EPE_Array[1] = (EPE_Int & 0x00FF);

        timeToFixArray[0] = (timeToFix & 0xFF00) >> 8;
        timeToFixArray[1] = (timeToFix & 0x00FF);

        isHighTideByte = isHighTide;

        timeArray[0] = (timeRTC & 0xFF000000) >> 24;
        timeArray[1] = (timeRTC & 0x00FF0000) >> 16;
        timeArray[2] = (timeRTC & 0x0000FF00) >> 8;
        timeArray[3] = (timeRTC & 0x000000FF);

        LoRaWAN.beginPacket();
        LoRaWAN.write(latArray[0]);
        LoRaWAN.write(latArray[1]);
        LoRaWAN.write(latArray[2]);
        LoRaWAN.write(latArray[3]);
        LoRaWAN.write(longArray[0]);
        LoRaWAN.write(longArray[1]);
        LoRaWAN.write(longArray[2]);
        LoRaWAN.write(longArray[3]);
        LoRaWAN.write(altArray[0]);
        LoRaWAN.write(altArray[1]);
        LoRaWAN.write(altArray[2]);
        LoRaWAN.write(altArray[3]);
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
     }    
}

void callbackSuspend(void)
{
    GNSS.suspend();
    callbackLoRaTx();  
    freshGPS = 0;
}

void callbackResume(void)
{
    GNSS.resume();
    startTimeToFix = millis();
    isTracking = true;
    myTimerSuspend.start(callbackSuspend, myAcqTime * 1000);  
}

uint32_t dateToSeconds(uint32_t mo, uint32_t da, uint32_t ho, uint32_t mi, uint32_t se)
{
    uint16_t totalDays = 0;
    for (int i = 1; i < mo; i++)
    {
        totalDays = totalDays + daysPerMonth[i];
    }
    totalDays = totalDays + da;
    totalSeconds = (totalDays * 86400) + (ho * 3600) + (mi * 60) + se;
    return totalSeconds;
}

void beginHighTideCycle(void) // Start the high tide cycle
{
       myTimerResume.restart(1000, (myPeriodHT * 1000));
       startHT_Time = millis();
       timeNow = dateToSeconds(months, days, hours, minutes, seconds);
       startHighFrequency = highTideCycle + ((highTideCycle - ((timeNow - refHighTide) % highTideCycle)) - (minutesBeforeHighTide * 60));
       millisToHighTide = millis() + (startHighFrequency * 1000);
}

void beginLowTideCycle(void) // Start the low tide cycle
{
    myTimerResume.restart(1000, (myPeriodLT * 1000));
}

void CAMM8QintHandler()
{
  ppsFlag = true;
  STM32L0.wakeup();
}

void alarmMatch()
{
  alarmFlag = true;
  STM32L0.wakeup();
}
