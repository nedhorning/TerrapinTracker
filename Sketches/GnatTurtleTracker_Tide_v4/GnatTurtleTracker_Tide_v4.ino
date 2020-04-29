#include <STM32L0.h>
#include "LoRaWAN.h"
#include "TimerMillis.h"
#include "GNSS.h"
#include <RTC.h>
#include "CayenneLPP.h"

// Gnat Asset Tracker gnat2
const char *appEui = "70B3D57ED0029388";
const char *appKey = "9C5E64056A46C2872DED008E20A6F95A";
const char *devEui = "383434306a379301";

CayenneLPP myLPP(64);

// Cricket pin assignments
#define myLed    10 // blue led 

uint8_t LoRaData[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool isHighTide = false;
bool initialized = false;

uint32_t myAcqTime =  30;  // Seconds to acquire a GPS fix
uint32_t myPeriodHT  = 60;   // Repeat period in seconds for GPS fixes during high tide
uint32_t myPeriodLT  = 240; // Repeat period in seconds for GPS fixes during low tide
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

// uint16_t totalDays;
uint32_t totalSeconds;
uint32_t refHighTide;  // Number of second from beginning of year to reference high tide 

int32_t startHighFrequency; // Seconds until high tide
uint32_t durationHighFrequency = (minutesBeforeHighTide + minutesAfterHighTide) * 60 * 1000; //Duration of high frequency GPS looging in milliseconds
//uint32_t durationLowFrequency =  - durationHighFrequency; Duration of low frequency GPS looging in milliseconds
uint32_t startTimer; 
uint32_t timeNow;
uint32_t startHT_Time = 0; //Time high tide monitoring cycle starts
//int switchVar = 1; // Use 1 for first fix, 2 for high frequency and 3 for low frequency

GNSSLocation myLocation;
GNSSSatellites mySatellites;

volatile bool isTracking = true;

TimerMillis myTimerSuspend;
TimerMillis myTimerResume;
//TimerMillis startHighTide;

uint32_t UID[3] = {0, 0, 0}; 
char buffer[32];


// MAX M8Q GNSS configuration ????
//#define GNSS_en      5     // enable for GNSS 3.0 V LDO
#define pps          4     // 1 Hz fix pulse
//#define GNSS_backup A0     // RTC backup for MAX M8Q

uint8_t freshGPS = 0;

uint16_t Hour = 1, Minute = 1, Second = 1, Millisec, Year = 1, Month = 1, Day = 1;
uint8_t hours = 12, minutes = 0, seconds = 0, years = 1, months = 1, days = 1;
uint32_t subSeconds, milliseconds;
bool ppsFlag = false, firstSync = false, alarmFlag = true;
uint16_t count = 0, fixType = 0, fixQuality;
int32_t latOut, longOut;
uint32_t startTimeToFix;
int16_t timeToFix = 0.0;

float Long, Lat, Alt, EPE;

  static const char *fixTypeString[] = {
      "NONE",
      "TIME",
      "2D",
      "3D",
  };

  static const char *fixQualityString[] = {
      "",
      "",
      "/DIFFERENTIAL",
      "/PRECISE",
      "/RTK_FIXED",
      "/RTK_FLOAT",
      "/ESTIMATED",
      "/MANUAL",
      "/SIMULATION",
  };

void setup()
{
  refHighTide = dateToSeconds(highTideRefMonth, highTideRefDay, highTideRefHour, highTideRefMinute, highTideRefSecond);
  
  /* Enable USB UART */
  Serial.begin(115200);
  delay(4000);
  Serial.println("Serial enabled!");

  /* Get LoRa/LoRaWAN ID for SX1276 */
  STM32L0.getUID(UID);
  Serial.print("STM32L0 MCU UID = 0x"); Serial.print(UID[0], HEX); Serial.print(UID[1], HEX); Serial.println(UID[2], HEX); 

  LoRaWAN.getDevEui(buffer, 18);
  Serial.print("STM32L0 Device EUI = "); Serial.println(buffer); 
  
  /* configure IO pins */
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);  // start with blue led off, since active LOW

  pinMode(pps, INPUT); // select pps as input from MAX M8Q

//  pinMode(GNSS_backup, OUTPUT);   // power for CAM M8Q RTC backup  // This and next line isn't in original sketch
//  digitalWrite(GNSS_backup, HIGH);

   /* Initialize and configure GNSS */
  GNSS.begin(Serial1, GNSS.MODE_UBLOX, GNSS.RATE_1HZ); // Start GNSS
  while (GNSS.busy()) { } // wait for begin to complete

  GNSS.setConstellation(GNSS.CONSTELLATION_GPS_AND_GLONASS); // choose satellites
  while (GNSS.busy()) { } // wait for begin to complete

  GNSS.setAntenna(GNSS.ANTENNA_EXTERNAL);  

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
  RTC.enableAlarm(RTC.MATCH_SS);  // alarm once per minute
//    RTC.enableAlarm(RTC.MATCH_ANY); // alarm once a second

  RTC.attachInterrupt(alarmMatch);

  attachInterrupt(pps, CAMM8QintHandler, RISING);


  // Configuree LoRaWAN connection
  /*
    - Asia       AS923
    - Australia  AU915
    - Europe     EU868
    - India      IN865
    - Korea      KR920
    - US         US915 (64 + 8 channels)
   */
    LoRaWAN.begin(US915);
    LoRaWAN.setADR(false);
    LoRaWAN.setDataRate(1);
    LoRaWAN.setTxPower(10);
    LoRaWAN.setSubBand(2); // 1 for MTCAP, 2 for TT gateways

    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    
    /* end of setup */
}

void loop()
{
 
  /*GNSS*/
//  Serial.print("GNSS location = "); 
//  Serial.println(GNSS.location(myLocation));
  Serial.print("Fix Type = "); 
  Serial.println(myLocation.fixType());
  Serial.print("isTracking = "); 
  Serial.println(isTracking);
  if(GNSS.location(myLocation))
  {
    Serial.println("Starting GNSS loop");
  if (myLocation.fixType() != GNSSLocation::TYPE_NONE)
  {
      Hour   = myLocation.hours();
      Minute = myLocation.minutes();
      Second = myLocation.seconds();
      Year   = myLocation.year();
      Month  = myLocation.month();
      Day    = myLocation.day();
      
      Serial.print(fixQualityString[myLocation.fixQuality()]);
      Serial.print(" ");
      Serial.print(myLocation.year());
      Serial.print("/");
      Serial.print(myLocation.month());
      Serial.print("/");
      Serial.print(myLocation.day());
      Serial.print(" ");
      if (myLocation.hours() <= 9) {Serial.print("0");}
      Serial.print(myLocation.hours());
      Serial.print(":");
      if (myLocation.minutes() <= 9) {Serial.print("0");}
      Serial.print(myLocation.minutes());
      Serial.print(":");
      if (myLocation.seconds() <= 9) {Serial.print("0");}
      Serial.print(myLocation.seconds());
      Serial.print(".");
      if (myLocation.millis() <= 9) {Serial.print("0");}
      if (myLocation.millis() <= 99) {Serial.print("0");}
      Serial.print(myLocation.millis());


      if (myLocation.leapSeconds() != GNSSLocation::LEAP_SECONDS_UNDEFINED) {
                Serial.print(" ");
                Serial.print(myLocation.leapSeconds());
                if (!myLocation.fullyResolved()) {
                    Serial.print("D");
                }
       }


      if (myLocation.fixType() != GNSSLocation::TYPE_TIME)
      {
      Lat = myLocation.latitude();
      myLocation.latitude(latOut);
      Long = myLocation.longitude();
      myLocation.longitude(longOut);
      Alt = myLocation.altitude();
      EPE = myLocation.ehpe(); // use this as accuracy figure of merit
      Serial.print(" LLA=");
      Serial.print(Lat, 7);
      Serial.print(",");
      Serial.print(Long, 7);
      Serial.print(",");
      Serial.print(Alt, 3);
      Serial.print(" EPE=");
      Serial.print(EPE, 3);
      Serial.print(",");
      Serial.print(myLocation.evpe(), 3);
      Serial.print(" SATELLITES=");
      Serial.print(myLocation.satellites());
      Serial.print(" DOP=");
      Serial.print(myLocation.hdop(), 2);
      Serial.print(",");
      Serial.print(myLocation.vdop(), 2);
      Serial.println();
      Serial.print("Location Resolved = ");
      Serial.println(myLocation.fullyResolved());
      Serial.print("isHighTide = ");
      Serial.println(isHighTide);
      
      // Wait until a good fix is acquired then initialize
      if( (myLocation.fixType() != GNSSLocation::TYPE_2D) && (EPE <= 50.0f) && myLocation.fullyResolved()) // 10 is about as low as one should go, 50 is acceptable
//      if( (myLocation.fixType() != GNSSLocation::TYPE_2D) && (EPE <= 100.0f))  // 10 is about as low as one should go, 50 is acceptable
      {
          if (!initialized) 
          {
              Serial.println("Initialize");
              isTracking = false;
              initialized = true;
              freshGPS = 1;
              Serial.println("callbackSuspend from initialization");
              callbackSuspend();
     //         syncRTC()
      //        delay(5000); // Wait five seconds for callbackSuspend to finish
              myTimerResume.start(callbackResume, 1000, myPeriodLT * 1000);
    //          timeNow = dateToSeconds(months, days, hours, minutes, seconds);
              timeNow = dateToSeconds(Month, Day, Hour, Minute, Second);
              Serial.print("refHighTide = "); // Delay
              Serial.println(refHighTide); 
              Serial.print("timeNow = ");  // Period
              Serial.println(timeNow);
              Serial.print("minutesBeforeHighTide = ");  // Period
              Serial.println(minutesBeforeHighTide);
              startHighFrequency = (highTideCycle - ((timeNow - refHighTide) % highTideCycle)) - (minutesBeforeHighTide * 60);  
              if (startHighFrequency < 0)
              {
                  startHighFrequency = startHighFrequency + highTideCycle; // If in a high tide cycle wait until the next one
              }
              //startHighTide.start(beginHighTideCycle, startHighFrequency * 1000, highTideCycle * 1000);
              millisToHighTide = millis() + (startHighFrequency * 1000);
              Serial.print("Millis = "); // Delay
              Serial.println(millis());
              Serial.print("millisToHighTide = "); // Delay
              Serial.println(millisToHighTide); 
              Serial.print("startHighFrequency = "); // Delay
              Serial.println(startHighFrequency); 
              Serial.print("highTideCycle = ");  // Period
              Serial.println(highTideCycle);
         //     callbackSuspend();
          } else
          {
             if (isTracking)
             {
                 isTracking = false;
                 freshGPS = 1;
                 Serial.print("Inside else loop = ");  // Period
                 timeToFix = round((millis() - startTimeToFix)/1000);  
                 myTimerSuspend.stop();
                 callbackSuspend();
             }
          }
       }
    }
  } 

  Serial.println();

  } /* end of GNSS Location handling */

    if (GNSS.satellites(mySatellites))
    {

    Serial.print("SATELLITES: ");
    Serial.print(mySatellites.count());
  
    Serial.println();

    for (unsigned int index = 0; index < mySatellites.count(); index++)
    {
  unsigned int svid = mySatellites.svid(index);

  if ((svid >= 1) && (svid <= 32))
  {
      Serial.print("    ");

      if (svid <= 9)
      {
    Serial.print("  G");
    Serial.print(svid);
      }
      else
      {
    Serial.print(" G");
    Serial.print(svid);
      }
  }
  else if ((svid >= 65) && (svid <= 96))
  {
      Serial.print("    ");

      if ((svid - 64) <= 9)
      {
    Serial.print("  R");
    Serial.print(svid - 64);
      }
      else
      {
    Serial.print(" R");
    Serial.print(svid - 64);
      }
  }
  else if ((svid >= 120) && (svid <= 158))
  {
      Serial.print("    ");
      Serial.print("S");
      Serial.print(svid);
  }
  else if ((svid >= 173) && (svid <= 182))
  {
      Serial.print("    ");
      Serial.print("  I");
      Serial.print(svid - 172);
  }
  else if ((svid >= 193) && (svid <= 197))
  {
      Serial.print("    ");
      Serial.print("  Q");
      Serial.print(svid - 192);
  }
  else if ((svid >= 211) && (svid <= 246))
  {
      Serial.print("    ");

      if ((svid - 210) <= 9)
      {
    Serial.print("  E");
    Serial.print(svid - 210);
      }
      else
      {
    Serial.print(" E");
    Serial.print(svid - 210);
      }
  }
  else if (svid == 255)
  {
      Serial.print("    ");
      Serial.print("R???");
  }
  else
  {
      continue;
  }

  Serial.print(": SNR=");
  Serial.print(mySatellites.snr(index));
  Serial.print(", ELEVATION=");
  Serial.print(mySatellites.elevation(index));
  Serial.print(", AZIMUTH=");
  Serial.print(mySatellites.azimuth(index));

  if (mySatellites.unhealthy(index)) {
      Serial.print(", UNHEALTHY");
  }

  if (mySatellites.almanac(index)) {
      Serial.print(", ALMANAC");
  }

  if (mySatellites.ephemeris(index)) {
      Serial.print(", EPHEMERIS");
  }

  if (mySatellites.autonomous(index)) {
      Serial.print(", AUTONOMOUS");
  }

  if (mySatellites.correction(index)) {
      Serial.print(", CORRECTION");
  }

  if (mySatellites.acquired(index)) {
      Serial.print(", ACQUIRED");
  }

  if (mySatellites.locked(index)) {
      Serial.print(", LOCKED");
  }

  if (mySatellites.navigating(index)) {
      Serial.print(", NAVIGATING");
  }

  Serial.println();
    }
 }/* end of GNSS Satellites handling */


  /*RTC*/
  if (alarmFlag) { // update RTC output whenever there is a GNSS pulse
    alarmFlag = false;

    // Read RTC
    Serial.println("RTC:");
    RTC.getDate(days, months, years);
    RTC.getTime(hours, minutes, seconds, subSeconds);

    milliseconds = ((subSeconds >> 17) * 1000 + 16384) / 32768;

    Serial.print("RTC Time = ");
    if (hours < 10)   {Serial.print("0");Serial.print(hours); } else Serial.print(hours);
    Serial.print(":");
    if (minutes < 10) {Serial.print("0"); Serial.print(minutes); } else Serial.print(minutes);
    Serial.print(":");
    if (seconds < 10) {Serial.print("0"); Serial.print(seconds); } else Serial.print(seconds);
    Serial.print(".");
        if (milliseconds <= 9) {
            Serial.print("0");
        }
        if (milliseconds <= 99) {
            Serial.print("0");
        }
    Serial.print(milliseconds);
    Serial.println(" ");

    Serial.print("RTC Date = ");
    Serial.print(years); Serial.print(":"); Serial.print(months); Serial.print(":"); Serial.println(days);
    Serial.println();

    digitalWrite(myLed, !digitalRead(myLed)); delay(1); digitalWrite(myLed, !digitalRead(myLed));
        
    } // end of alarm section
    
    if (initialized)
    {
       if (isHighTide)
       {
           Serial.print("millis = ");  // Period
           Serial.println(millis());
           Serial.print("startHT_Time = ");  // Period
           Serial.println(startHT_Time);
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


/* Useful functions */
void callbackLoRaTx(void)
{     
    Serial.print("LoRaWAN busy = ");
    Serial.println(LoRaWAN.busy());
    Serial.print("LoRaWAN joined = ");
    Serial.println(LoRaWAN.joined());
    Serial.println(millis());
    if (!LoRaWAN.busy() && LoRaWAN.joined())
     {
        myLPP.reset();
        myLPP.addGPS(1, Lat, Long, Alt);
        myLPP.addAnalogInput(2, EPE);
        myLPP.addAnalogInput(3, timeToFix);
        myLPP.addDigitalInput(4, freshGPS);
        myLPP.addDigitalInput(5, isHighTide);
        
        LoRaWAN.sendPacket(myLPP.getBuffer(), myLPP.getSize());
        Serial.println(millis());
        //while (LoRaWAN.busy())
        //{
        //}
        Serial.println(millis());
        Serial.print("Package sent with: ");
        Serial.println(myLPP.getSize());
     }    
}

void callbackSuspend(void)
{
    GNSS.suspend();
    Serial.println("Suspend Callback");
    callbackLoRaTx();  // update dashboard/backend via LoRaWAN
    freshGPS = 0;
    //isTracking = false;
}

void callbackResume(void)
{
    Serial.println("Resume Callback");
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
    Serial.print("totalSeconds = "); 
    Serial.println(totalSeconds);
    Serial.print("totalDays = "); 
    Serial.println(totalDays);
    Serial.print("ho = "); 
    Serial.println(ho);
    Serial.print("mi = "); 
    Serial.println(mi);
    Serial.print("se = "); 
    Serial.println(se);
    return totalSeconds;
}

void beginHighTideCycle(void) // Start the high tide cycle
{
    Serial.println("Start beginHighTideCycle");  
       myTimerResume.restart(1000, (myPeriodHT * 1000));
       startHT_Time = millis();
//       millisToHighTide = millis() + (highTideCycle * 1000);
       timeNow = dateToSeconds(months, days, hours, minutes, seconds);
       startHighFrequency = highTideCycle + ((highTideCycle - ((timeNow - refHighTide) % highTideCycle)) - (minutesBeforeHighTide * 60));
       millisToHighTide = millis() + (startHighFrequency * 1000);
       Serial.print("millisToHighTide = "); 
       Serial.println(millisToHighTide);
       Serial.print("timeNow = "); 
       Serial.println(timeNow);
       Serial.print("startHighFrequency = "); 
       Serial.println(startHighFrequency);
}

void beginLowTideCycle(void) // Start the low tide cycle
{
    Serial.println("Start beginLowTideCycle");
    
    myTimerResume.restart(1000, (myPeriodLT * 1000));
//    millisToHighTide = millis() + ((highTideCycle * 1000)) - (durationHighFrequency * 1000);
}
/*
bool isHT_StartOk()
{
   timeNow = dateToSeconds(Month, Day, Hour, Minute, Second);
   int32_t startTime = ((timeNow - refHighTide) % highTideCycle) - (minutesBeforeHighTide * 60);
   bool timingOK = abs(startTime) < 300;  // High tide start monitoring time is within 5 minutes of expected time
   Serial.print("startTime = ");  
   Serial.println(startTime);
   Serial.print("timeNow = ");  
   Serial.println(timeNow);
   return timingOK;
}
*/
void CAMM8QintHandler()
{
  ppsFlag = true;
  Serial.println("In CAMM8QintHandler = ");
  STM32L0.wakeup();
}


void alarmMatch()
{
  alarmFlag = true;
  Serial.println("In alarmMatch = ");
  STM32L0.wakeup();
}

void syncRTC()
{
  // Set the time
  RTC.setSeconds(Second);
  RTC.setMinutes(Minute);
  if (Hour < 7) {
    RTC.setHours(Hour + 17);
  } else RTC.setHours(Hour - 7);
  RTC.setMinutes(Minute);

  // Set the date
  if (Hour < 7) {
    RTC.setDay(Day - 1);
  } else RTC.setDay(Day);
  RTC.setMonth(Month);
  RTC.setYear(Year - 2000);
}
