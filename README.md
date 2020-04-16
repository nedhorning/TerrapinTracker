# TerrapinTracker
## Introduction
The goal of this project is to develop a lightweight, low power consumption tracker that can be used for relatively light (>200 g) animals. The primary components of the tracker are a GNSS receiver and a LoRa radio transceiver. The initial species we are targeting for tracking is the diamondback terrapin (DBT, Malaclemys terrapin) but the tracker can be adapted to other taxa. 

The information in this repository is currently focused on the tracking the DBT but the longer term goal is to develop a workflow that will make it easier to design and create purpose-built GNSS/radio trackers for a wide range or animals. 

### Current thinking:
    • Power with a battery using tide timing to dictate GNSS sampling frequency (e.g., high tide/low tide). 

### Near-term experiments: 
    • Test of different node antennas
    • Store GPS readings when LoRa communication is not working
    • Research way to reduce current

### Future:
    • Add solar charging
    • Add additional logic (GNSS frequency based on movement or stationary) and hardware (water sensor to detect when tracker is underwater)
    • Test alternatives to the gateway – node architecture such as a LoRa mesh network, low cost and low power relays and multiple Gateways. 
    • Develop a workflow to more easily create purpose-built boards with updated components
    
## Software
### Arduino sketch
These sketches are largely based on the demo sketch written by the Gant’s developer:  https://github.com/kriswiner/CMWX1ZZABZ/tree/master/Gnat. 

There are two versions of the sketch. The one provides serial output primarily for debugging and the other (with “NoPrints” in the file name) does not have serial output or LED lights and is intended to be used for the low power implementation running at a CPU speed of 4.2 MHz. 

### LoRa Server 
Don can provide details or link to one of his blogs for the ChirpStack server setup


## Hardware
### GNSS/LoRa tracker
We are using the Gnat LoRa+GNSS Asset Tracker developed by Kris Winer at Tlera Corp. and available for purchase on Tindie: https://www.tindie.com/products/tleracorp/gnat-loragnss-asset-tracker/. 

We selected this unit because of it’s small footprint and low power requirements. The board is programmed using Arduino with a library dedicated to support for STM32L0 boards: https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0.

### Tracker peripherals (LoRa and GNSS antennas)
The Gnat requires external GNSS and LoRa antennas. We are using this LoRa antenna: https://www.mouser.com/ProductDetail/anaren/66089-0930/?qs=pH7abCSN9NM0tHwbfikfEQ%3D%3D&countrycode=US&currencycode=USD and these two GNSS patch antennas: https://www.mouser.com/ProductDetail/inventek-systems/actpat184-01-ip/?qs=MLItCLRbWszGAzoW2JcPkw%3D%3D&countrycode=US&currencycode=USD [This used about 10mA more current then the one from AliExpress for some reason] and https://www.aliexpress.com/item/32796566081.html?spm=a2g0s.9042311.0.0.51524c4dziIOL7.

The Gnat can be powered by a micro-USB cable or a battery connection can be added by either directly soldering battery leads to the board or soldering a batter cable with a connector. We have been using a variety of 3.7V lithium ion polymer batteries [https://www.adafruit.com/product/1570] for testing. These batteries require a JST PH 2-pin cable with a female connector [https://www.adafruit.com/product/261] soldered to the Gnat. 

### LoRa gateway and outdoor enclosure
The gateway is RAK Wireless LoRa Raspberry Pi Gateway with Enclosure: https://www.sparkfun.com/products/15336. A RAK Wireless outdoor enclosure was also purchased:  https://store.rakwireless.com/products/outdoor-gateway-enclosure. This antenna was purchased for the outdoor gateway enclosure: LoRa Fiberglass Antenna Type N - 5.8dBi (860-930MHz) [https://www.sparkfun.com/products/15597] and Interface Cable N to RP-SMA Cable – 1m [https://www.sparkfun.com/products/14911]. 

We also had to purchase a USB external WiFi antenna for the Pi Gateway [https://www.ebay.com/itm/123990900283] to attach to the USB port on the Pi and [https://www.data-alliance.net/antenna-5dbi-2-4ghz-5ghz-options-outdoor-omnidirectional-w-n-male-right-angle/] for the external antenna. We also needed a U.FL-Male To RP-SMA-Male Adapter [https://www.data-alliance.net/u-fl-male-to-rp-sma-male-adapter-u-fl-connector-snaps-for-permanent-fit/].

The outdoor enclosure is packaged with a cable gland for the power-over-ethernet adapter that comes with the enclosure but it does not have a gland for a power cable if the Pi will be powered using other means. We used this M25 x 1.5 metric thread gland: https://www.ebay.com/itm/352874323191. 

In order to attach the Pi Gateway to the outdoor enclosure we had to drill four holes in the mounting aluminum sheet that came with the outdoor enclosure. 

### Gnat enclosure
The enclose needs to be watertight. Ideally it would be nice to be able to reprogram the unit, remove the electronics and change batteries without having to destroy the enclosure. The current design can be opened with a saw before it is mounted on a DBT. 

The enclosure is sculpted from 1/2” inside diameter schedule  40 PVC pipe. To make this a 4.25” section of pipe was cut off and then submerged in boiling water. After short while (~15 seconds) the PVC becomes malleable and the front 1 3/4” of the pipe is stretched to an oblong shape using a piece of 7/8” x 1/8” steel with a slightly rounded end inserted into the PVC pipe. The other, round end of the pipe is drilled with a 5/8” drill to increase the size of the hole to accommodate a SAFT LS14500 AA 3.6V 2600mAh Li-SOCl2 battery with wire leads. The diameter of the batteries is slightly larger than a typical AA battery due to the wires and a plastic sheath that envelopes the battery and leads. A hole has to be drilled in the PVC pipe near the oval end for the LoRa antenna.

Just prior to deploying the ends of the PVC pipe need to be sealed. I used JB WaterWeld epoxy putty and it seems to work well. A mound of the same epoxy can be mounded up around the antenna to reinforce the antenna while at the same time sealing the hole. 

### Attaching the Gnat enclosure to a DBT
We will likely apply an epoxy using methods developed by biologists with DBT tracking experience. Care must be used to select an epoxy that cures relatively quickly keeping in mind that the shorter the cure time the greater the heat that is generated. If the heat is too high the DBT can be injured.  

The DBT sheds its scutes so it is expected that the tracking unit will fall off at some point. 
