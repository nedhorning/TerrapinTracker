Integrating tide tables to the Arduino script

We implemented two methods for determining high tide times. The first uses an average value for a high tide to high tide cycle and the second approach, described here, uses published tide tables. Using tide tables is superior since the high tide cycle is not regular and using an average value for the cycle’s period can have errors of well over one hour. 

To add a tide table to the Arduno sketch you need to copy an array of high tide times, using Unix epoch time format, that will cover your monitoring period. The steps described here assume monitoring will take place within a single calendar year and if that is not the case you will need to manually concatenate multiple arrays into a single array. This can be easily done using text editing software. 

To start, you need to download a suitable NOAA tide table. If you can’t find one for your precise location, data from a nearby station should work fine. I use the NOAA Tide Predictions interactive web site (https://tidesandcurrents.noaa.gov/tide_predictions.html) to download tide tables. If you need data for another country you will need to find another source for tide data and, unless the tide table uses the same format as the NOAA tide table, you will need to convert the high tide times to Unix epoch time and create the array on your own. Feel free to contact me if you need help with that. 

When you select a specific station location click on the “Click Here for Annual Published Tide Tables” button. In the interface you need to select the following values for each field and when you are done click the “Download” button:

> Year: Select a year
> Datum: You can select any datum but in many cases the only choice is “MLLW”
> Threshold Direction: Either choice is fine 
> Format: Select “TXT” 
> Timezone: Select “GMT” 
> 12 Hour/24 Hour Clock: Select “24 Hour” 
> Threshold Value: Leave this field blank

![ ](./tide-table.png)

The file will be downloaded to your default downloads directory. You can open the file using any text editor. The first several lines should look like this:

> NOAA/NOS/CO-OPS
> Disclaimer: These data are based upon the latest information available as of the date of your request, and may differ from the published tide tables.
> Annual Tide Predictions
> StationName: SWAIN CHANNEL 
> State: NJ 
> Stationid: 8535805
> ReferencedToStationName: Atlantic City
> ReferencedToStationId: 8534720
> HeightOffsetLow: * 1.06
> HeightOffsetHigh: * 1.09
> TimeOffsetLow: 40
> TimeOffsetHigh: 55
> Prediction Type: Subordinate
> From: 20200101 05:20 - 20201231 20:11
> Units: Feet and Centimeters
> Time Zone: GMT
> Datum: MLLW
> Interval Type: High/Low
>
> Date 		Day	Time	Pred(Ft)	Pred(cm)	High/Low
> 2020/01/01	Wed	05:20	3.35		102			H
> 2020/01/01	Wed	11:02	0.71		22			L
> 2020/01/01	Wed	17:29	4.01		122			H
> 2020/01/01	Wed	23:46	0.36		11			L
> 2020/01/02	Thu	06:12	3.35		102			H
> 2020/01/02	Thu	11:57	0.86		26			L

A python 3 script was written to automate the date and time conversion and creation of an array in a text file that can easily be copied and pasted into the Arduino script. The script, “tideTableParser.py” is available from the TerrapinTracker GitHub repository in the “Tides” directory. To run the script you need to provide the filename of the text file that was downloaded from the NOAA portal. If you are running this from the same directory as the script an example command would be:
python3 tideTableParser.py /media/ned//8535805_annual.txt

The output from the script is an ASCII text file with all of the high tide times in Unix epoch time format extracted from the NOAA tide table. The output file name will be the same as the input name except “_linuxTime” will be appended to the file name. For my exampe above the output filename was:
>    /media/ned//8535805_annual_linuxTime.txt

If you open the file in a text editor you will see one very long line of 10-digit numbers bounded by curly braces “{  }”. This is an example of the first several high tide times:
>	{1577856000,1577899740,1577945520,1577989080,1578035100,1578078720,1578124740,
The end of the array should look something like this:
>	1609201140,1609245300,1609290000,1609333980,1609378800,1609422660};

To add the high tide times to the Arduino sketch open the sketch and look for the line starting with “const PROGMEM uint32_t HT_Times[]”. Copy the entire array (using Select All is a good way to ensure you copy the entire array) from the file you created with the python script and paste it after the “=” for the “HT_Times[]” variable. Verify the code compiles and then load it into your tracker.


