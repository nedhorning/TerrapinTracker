/* this file is documented using doxygen*/
#include <STM32L0.h>
#include "LoRaWAN.h"
#include "TimerMillis.h"
#include "GNSS.h"
#include <RTC.h>

/// lora/chirp stack
const char *appEui = "70B3D57ED0029388";
const char *appKey = "9C5E64056A46C2872DED008E20A6F95A";  // This is set using the ChirpStack Application Server 
const char *devEui = "383434306f379301";  
uint32_t UID[3] = {0, 0, 0};

bool initialized = false;  // Flag set when GNSS get initial fix and sets RTC

/// GNSS/GPS variables
GNSSLocation myLocation;  // create object
GNSSSatellites mySatellites;  // create object
uint8_t withSignalCount = 0;
uint8_t minWithSignalCount = 1; // Minimum number of satellites to receive signal within maxSignalSearchTime 
uint8_t minSNR = 10; // Minimum signal to noise ratio to qualify as a received signal
uint32_t maxSignalSearchTime = 5000; // Milliseconds to receive a signal from minWithSignalCount number of satellites 
volatile bool recvSats = false;
volatile bool isTracking = true;
uint8_t freshGPS = 0;
uint16_t fixType = 0;
uint32_t startTimeToFix = 0;  
int16_t timeToFix = 0;
int32_t latOut, longOut;
float Long, Lat, Alt, EPE;
int32_t latInt, longInt, altInt;
int16_t EPE_Int, vbatInt; 
byte latArray[4], longArray[4], altArray[4], EPE_Array[2], timeToFixArray[2], timeArray[4], vbatArray[2];

/// Define the tide arraay in program memory
const PROGMEM uint32_t HT_Times[] = {1577856000,1577899740,1577945520,1577989080,1578035100,1578078720,1578124740,1578168480,1578214260,1578258180,1578303600,1578347760,1578392760,1578437160,1578481860,1578526440,1578570900,1578615600,1578659940,1578704820,1578749040,1578793980,1578838200,1578883320,1578927420,1578972780,1579016820,1579062420,1579106400,1579152300,1579196220,1579242420,1579286400,1579332720,1579376760,1579422960,1579467240,1579512960,1579557480,1579602720,1579647300,1579692180,1579736820,1579781400,1579826040,1579870380,1579915020,1579959240,1580003880,1580047980,1580092620,1580136660,1580181360,1580225280,1580270040,1580313960,1580358840,1580402700,1580447760,1580491620,1580536920,1580580840,1580626500,1580670600,1580716320,1580760780,1580806260,1580850960,1580896020,1580940900,1580985600,1581030540,1581075060,1581119940,1581164340,1581209220,1581253620,1581298560,1581342840,1581387840,1581432120,1581477240,1581521460,1581566760,1581610980,1581656460,1581700680,1581746460,1581790740,1581836700,1581881280,1581927120,1581971940,1582017420,1582062360,1582107360,1582152300,1582196880,1582241700,1582286040,1582330680,1582374900,1582419480,1582463580,1582508100,1582552140,1582596600,1582640640,1582685040,1582729140,1582773480,1582817580,1582861920,1582906080,1582950480,1582994700,1583039280,1583083680,1583128620,1583173260,1583218560,1583263680,1583308980,1583354460,1583399400,1583444820,1583489460,1583534700,1583579160,1583624220,1583668620,1583713620,1583758020,1583802900,1583847300,1583892240,1583936640,1583981520,1584025980,1584070980,1584115440,1584160560,1584205140,1584250440,1584295260,1584340680,1584385800,1584431160,1584476580,1584521580,1584567060,1584611580,1584656820,1584701100,1584746100,1584790200,1584834900,1584879000,1584923520,1584967620,1585012020,1585056120,1585100400,1585144620,1585188780,1585233000,1585277040,1585321440,1585365360,1585409820,1585453800,1585498440,1585542540,1585587360,1585631820,1585677060,1585721760,1585767540,1585812300,1585858440,1585903020,1585948920,1585993320,1586038860,1586083200,1586128500,1586172840,1586217900,1586262300,1586307240,1586351700,1586396520,1586441040,1586485800,1586530440,1586575200,1586619960,1586664720,1586709720,1586754480,1586799780,1586844540,1586890260,1586934900,1586980860,1587025200,1587071100,1587115200,1587160800,1587204720,1587249900,1587293820,1587338700,1587382680,1587427260,1587471360,1587515760,1587559980,1587604140,1587648480,1587692520,1587737040,1587780900,1587825540,1587869280,1587914040,1587957780,1588002780,1588046640,1588091940,1588135980,1588181760,1588225860,1588272180,1588316280,1588362840,1588406880,1588453200,1588497240,1588543080,1588587300,1588632720,1588677000,1588722180,1588766640,1588811520,1588856100,1588900860,1588945560,1588990200,1589035020,1589079540,1589124540,1589168940,1589214240,1589258520,1589304120,1589348340,1589394300,1589438280,1589484480,1589528340,1589574480,1589618160,1589664000,1589707740,1589753160,1589796960,1589842020,1589886000,1589930760,1589974860,1590019320,1590063660,1590107880,1590152400,1590196380,1590241080,1590284940,1590329820,1590373500,1590418620,1590462300,1590507600,1590551280,1590597000,1590640680,1590686760,1590730500,1590777000,1590820620,1590867300,1590911040,1590957480,1591001340,1591047360,1591091460,1591137060,1591181340,1591226580,1591271040,1591315980,1591360680,1591405380,1591450140,1591494660,1591539600,1591583940,1591629060,1591673220,1591718520,1591762560,1591808100,1591851960,1591897800,1591941480,1591987560,1592031060,1592077260,1592120760,1592166720,1592210340,1592256060,1592299800,1592345100,1592389140,1592434020,1592478300,1592522880,1592567400,1592611620,1592656320,1592700420,1592745240,1592789160,1592834220,1592878020,1592923260,1592967000,1593012420,1593056160,1593101880,1593145500,1593191580,1593235200,1593281520,1593325140,1593371640,1593415380,1593461760,1593505680,1593551760,1593595920,1593641580,1593685980,1593731220,1593775800,1593820680,1593865440,1593910020,1593954840,1593999240,1594044120,1594088340,1594133340,1594177380,1594222500,1594266420,1594311660,1594355460,1594400880,1594444500,1594490220,1594533780,1594579620,1594623180,1594669080,1594712820,1594758600,1594802580,1594848000,1594892280,1594937280,1594981800,1595026380,1595071140,1595115480,1595160300,1595204520,1595249400,1595293500,1595338560,1595382540,1595427660,1595471640,1595516940,1595560860,1595606400,1595650200,1595695980,1595739780,1595785860,1595829720,1595875980,1595919960,1595966160,1596010380,1596056340,1596100860,1596146280,1596190980,1596236040,1596280800,1596325500,1596370260,1596414720,1596459480,1596503760,1596548520,1596592620,1596637440,1596681420,1596726240,1596770160,1596814980,1596858840,1596903780,1596947640,1596992700,1597036620,1597081800,1597125780,1597171200,1597215420,1597260900,1597305420,1597350720,1597395540,1597440480,1597485420,1597530000,1597574940,1597619340,1597664340,1597708560,1597753560,1597797720,1597842720,1597886880,1597931940,1597976100,1598021220,1598065380,1598110620,1598154780,1598200200,1598244360,1598290080,1598334360,1598380200,1598424780,1598470560,1598515440,1598560980,1598605980,1598651040,1598696160,1598740800,1598785800,1598830140,1598875020,1598919180,1598963940,1599007980,1599052620,1599096660,1599141240,1599185280,1599229800,1599273840,1599318300,1599362400,1599406800,1599450960,1599495420,1599539700,1599584280,1599628740,1599673500,1599718320,1599763320,1599808560,1599853500,1599899100,1599943740,1599989220,1600033620,1600078920,1600123200,1600168380,1600212600,1600257660,1600301880,1600346880,1600391160,1600436100,1600480440,1600525380,1600569780,1600614780,1600659240,1600704420,1600749000,1600794240,1600839060,1600884420,1600929660,1600974960,1601020500,1601065440,1601111040,1601155560,1601200980,1601245200,1601290380,1601334420,1601379360,1601423340,1601468040,1601512020,1601556540,1601600580,1601644980,1601689080,1601733360,1601777580,1601821740,1601866080,1601910120,1601954640,1601998620,1602043260,1602087300,1602132240,1602176460,1602221880,1602266280,1602312240,1602356640,1602402900,1602447120,1602493200,1602537240,1602582960,1602627060,1602672480,1602716580,1602761760,1602806040,1602851040,1602895380,1602940320,1602984780,1603029660,1603074240,1603119060,1603163760,1603208640,1603253580,1603298460,1603343760,1603388640,1603434360,1603479000,1603525080,1603569420,1603615440,1603659480,1603705200,1603749060,1603794420,1603838220,1603883280,1603927140,1603971900,1604015820,1604060340,1604104440,1604148780,1604193000,1604237160,1604281560,1604325540,1604370060,1604413920,1604458680,1604502420,1604547360,1604591160,1604636400,1604680320,1604726040,1604769960,1604816340,1604860200,1604906880,1604950680,1604997180,1605040920,1605087000,1605130920,1605176580,1605220680,1605266040,1605310320,1605355380,1605399780,1605444720,1605489300,1605534060,1605578820,1605623520,1605668400,1605713040,1605758160,1605802740,1605848160,1605892620,1605938460,1605982680,1606028880,1606072800,1606119000,1606162680,1606208700,1606252320,1606297920,1606341600,1606386840,1606430640,1606475520,1606519500,1606564080,1606608300,1606652640,1606697040,1606741140,1606785720,1606829640,1606874400,1606918200,1606963080,1607006820,1607051940,1607095620,1607141040,1607184720,1607230560,1607274240,1607320620,1607364180,1607410860,1607454420,1607501100,1607544780,1607591100,1607635020,1607680860,1607725020,1607770440,1607814840,1607859960,1607904480,1607949360,1607994000,1608038760,1608083520,1608128100,1608173040,1608217500,1608262560,1608306900,1608352200,1608396360,1608442020,1608485940,1608531840,1608575580,1608621720,1608665340,1608711420,1608755040,1608800880,1608844620,1608890040,1608934020,1608979020,1609023180,1609067880,1609112220,1609156620,1609201140,1609245300,1609290000,1609333980,1609378800,1609422660};

// Variables for when to start high freq GPS
uint8_t maxEPE = 5;   ///< Maximum EPE value before starting initialization
uint8_t longTermEPE = 20;  ///< EPE after GPS is initialized
uint16_t myAcqTime =  60;  ///< Seconds to acquire a GPS fix
uint16_t myPeriodHT  = 120;   ///< Repeat period in seconds for GPS fixes during high tide
uint16_t myPeriodLT  = 120; ///< Repeat period in seconds for GPS fixes during low tide
uint16_t minutesBeforeHighTide = 120; ///< Number of minutes before high tide to start high frequency GPS
uint16_t minutesAfterHighTide = 120; ///< Number of minutes after high tide to stop high frequency GPS
bool ppsFlag = false;  ///< flag for pps pin GNSS module

/// Assume the date is 01/01/2020 (MM/DD/YYYY) at 00:20:00 (hh:mm:ss) to inititialize RTC clock 
uint16_t highTideRefYear = 2020;  ///< Year
uint8_t highTideRefMonth = 1; // January = 1 and December = 12
uint8_t highTideRefDay = 1;  // Day
uint8_t highTideRefHour = 0;  // Hour in 24 hour clock
uint8_t highTideRefMinute = 20;  // Minutes
uint8_t highTideRefSecond = 0;  // Seconds
uint32_t timeRTC; 

/// Define high tide timing variables
TimerMillis myTimerResume;  ///< timer object for alarm
bool isHighTide = false;
uint8_t isHighTideByte;
uint32_t refHighTide;  ///< Number of second from beginning of year to reference high tide 
uint32_t millisNextHighTide;
int16_t secBeforeHTStart; ///< Seconds before starting high tide cycles
uint32_t durationHighFrequency = (minutesBeforeHighTide + minutesAfterHighTide) * 60 * 1000; ///< Duration of high frequency GPS looging in milliseconds
uint32_t startHT_Time = 0; ///< Time high tide monitoring cycle starts
 
/// Pin Assignments
#define pps          4     ///< 1 Hz  MAX M8Q GNSS fix pulse
#define myBat    A1 ///< LiPo battery ADC
#define myBat_en  2 ///< LiPo battery monitor enable

/// initalise battery monitoring variables
float VDDA, VBAT;

void setup()
{  
  /// setup coms
  Serial.begin(115200);
  delay(4000);

  /// Configuree LoRaWAN connection
  STM32L0.getUID(UID);  ///< Get LoRa/LoRaWAN ID for SX1276
  LoRaWAN.begin(US915);  ///< select frequency plan
  LoRaWAN.setADR(false);  ///< automatic data rate
  LoRaWAN.setDataRate(1);  ///< select data rate
  LoRaWAN.setTxPower(20);   ///< transmit power
  LoRaWAN.setSubBand(2); ///< 1 for MTCAP, 2 for TT gateways
  LoRaWAN.joinOTAA(appEui, appKey, devEui);  ///< use Over The Air Authorization (OTAA)
  LoRaWAN.onReceive(receiveCallback);   ///< enable downstream communications to modify variables

  /// configure IO pins 
  pinMode(myBat_en, OUTPUT);  ///< set voltage probe pin as output
  pinMode(myBat, INPUT);    ///< set up ADC battery voltage monitor pin
  digitalWrite(myBat_en, LOW);
  analogReadResolution(12); ///< 12 = 12-bit ADC resolution
  pinMode(pps, INPUT); ///< select pps as input from MAX M8Q

  /* Set RTC time and date based on reference tide */
  RTC.setHours(highTideRefHour);
  RTC.setMinutes(highTideRefMinute);
  RTC.setSeconds(highTideRefSecond);
  RTC.setDay(highTideRefDay);
  RTC.setMonth(highTideRefMonth);
  RTC.setYear(highTideRefYear-2000);
  refHighTide = RTC.getEpoch();

   /* Initialize and configure GNSS */
  GNSS.begin(Serial1, GNSS.MODE_UBLOX, GNSS.RATE_1HZ); // Start GNSS
  while (GNSS.busy()) { } 

  GNSS.setConstellation(GNSS.CONSTELLATION_GPS_AND_GLONASS); // choose satellite constellations
  GNSS.setConstellation((GNSSClass::GNSSconstellation)11); 
  // 11 = GPS + GLONASS + Galileo  For GPS + Beidou + Galileo use 13
  while (GNSS.busy()) { } 

  GNSS.setAntenna(GNSS.ANTENNA_EXTERNAL);  
  while (GNSS.busy()) { }

  GNSS.enableWakeup();

  // set alarm to update the RTC periodically
  RTC.setAlarmTime(0, 0, 0);
  RTC.enableAlarm(RTC.MATCH_SS); // alarm once a minute
  RTC.attachInterrupt(alarmMatch);


}

void loop()
{
  /*GNSS*/

  if(GNSS.location(myLocation))
 
  {
    if (recvSats == false && GNSS.satellites(mySatellites))
    {
      withSignalCount = 0;
      for (unsigned int index = 0; index < mySatellites.count(); index++)
      {
        if (mySatellites.snr(index) > minSNR)
        {
          withSignalCount++;
        }
      }
      if (withSignalCount >= minWithSignalCount) 
      {
        recvSats = true;
      }
    }
    if ((((millis() - startTimeToFix) > (myAcqTime * 1000)) || (recvSats == false && (millis() - startTimeToFix) > maxSignalSearchTime)) && initialized && isTracking)
    {
      isTracking = false;
      callbackSuspend();
    }
  
    if (myLocation.fixType() != GNSSLocation::TYPE_NONE)
    {      
      if (myLocation.fixType() != GNSSLocation::TYPE_TIME)
      {
        Lat = myLocation.latitude();
        myLocation.latitude(latOut);
        Long = myLocation.longitude();
        myLocation.longitude(longOut);
        Alt = myLocation.altitude();
        EPE = myLocation.ehpe(); // use this as accuracy figure of merit
      
        // Wait until a good fix is acquired 
        if( (myLocation.fixType() != GNSSLocation::TYPE_2D) && (EPE <= maxEPE) && myLocation.fullyResolved()) 
        {  
          /* Reset RTC time and date based on GPS */
          RTC.setHours(myLocation.hours());
          RTC.setMinutes(myLocation.minutes());
          RTC.setSeconds(myLocation.seconds());
          RTC.setDay(myLocation.day());
          RTC.setMonth(myLocation.month());
          RTC.setYear(myLocation.year()- 2000);
           
          if (!initialized) 
          {
            maxEPE = longTermEPE;
            isTracking = false;
            initialized = true;
            freshGPS = 1;
            timeRTC = RTC.getEpoch();
            callbackSuspend();
            myTimerResume.start(callbackResume, 1000, myPeriodLT * 1000);
            secBeforeHTStart = secondsToNextHT(timeRTC, false) - (minutesBeforeHighTide * 60);
            if (secBeforeHTStart <= 0) 
            {
              // If in a HT cycle wait for the next one
              secBeforeHTStart = secondsToNextHT(timeRTC, true) - (minutesBeforeHighTide * 60); 
            }
            millisNextHighTide = millis() + (secBeforeHTStart * 1000);
          } else
          {
            if (isTracking)
            {
              isTracking = false;
              freshGPS = 1;
              timeToFix = round((millis() - startTimeToFix)/1000);  
              timeRTC = RTC.getEpoch();
              callbackSuspend();
            }
          }
        }
      } 
    }   
  } /* end of GNSS Location handling */
   
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
       if (millis() > millisNextHighTide)
       {
         isHighTide = true;
         beginHighTideCycle();
       }
    }
  }
  STM32L0.stop();        // Enter STOP mode and wait for an interrupt
}  

void callbackLoRaTx(void)
{     
  if (!LoRaWAN.joined()) 
  {
     LoRaWAN.rejoinOTAA();
  }
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
    digitalWrite(myBat_en, HIGH);
    VDDA = STM32L0.getVDDA();
    VBAT = 1.27f * VDDA * ((float) analogRead(myBat)) / 4096.0f;
    digitalWrite(myBat_en, LOW);
    vbatInt = round(VBAT * 1000);
    vbatArray[0] = (vbatInt & 0xFF00) >> 8;
    vbatArray[1] = (vbatInt & 0x00FF);
     
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
    LoRaWAN.write(vbatArray[0]);
    LoRaWAN.write(vbatArray[1]);
    LoRaWAN.endPacket();
  }    
}

void callbackSuspend(void)
{
  GNSS.suspend();
  timeToFix = round((millis() - startTimeToFix)/1000);  // Also time to give up getting a fix when freshGPS = 0
  callbackLoRaTx();  
  freshGPS = 0;
  recvSats = false;
}

void callbackResume(void)
{
  GNSS.resume();
  recvSats = false;
  startTimeToFix = millis();
  //mySatellites = GNSSSatellites(); //ResetGNSSSatellites class
  isTracking = true; 
}

void beginHighTideCycle(void) // Start the high tide cycle
{
  myTimerResume.restart(1000, (myPeriodHT * 1000));
  startHT_Time = millis();
  timeRTC = RTC.getEpoch();
  secBeforeHTStart = secondsToNextHT((timeRTC), true) - (minutesBeforeHighTide * 60);
  millisNextHighTide = millis() + (secBeforeHTStart * 1000);
}

void beginLowTideCycle(void) // Start the low tide cycle
{
  myTimerResume.restart(1000, myPeriodLT * 1000);
}

void alarmMatch()
{
  STM32L0.wakeup();
}

uint32_t secondsToNextHT(uint32_t time, boolean getNextIndex)
{
  int i = 0;
  uint32_t seconds;
  while (HT_Times[i] < time) 
  {
      i++;
  }
  if (getNextIndex) i++;
  seconds = HT_Times[i] - time; 
  return seconds;
}

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
