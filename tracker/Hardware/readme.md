# Tracker Bill of Materials

Note that most of the items can be purchased at other vendors Some additional items for the tracker and gateways will be noted in the documents describing those components.   

GNSS/LoRa tracker \$69.95
We are using the Gnat LoRa+GNSS Asset Tracker developed by Kris Winer at Tlera Corp. and available for purchase on Tindie: <https://www.tindie.com/products/tleracorp/gnat-loragnss-asset-tracker/>

LoRa node antenna  \$7.07
We selected the Aneren 66089-0930 915 MHz whip antenna

GNSS antenna  \$7.12
We selected the Inventek Systems ACTPAT184-01-IP active ceramic patch antenna. It is labeled as a GPS antenna but it does receive other satellite constellations

RAK Wireless LoRa Raspberry Pi 3 B+ Gateway  \$199 - \$278
There are two versions of this 8-channel gateway. The RAK7243 (\$199) and the RAK7243C (\$278) which includes the option of adding cellular connectivity.

RAK Wireless LoRa Raspberry Pi Zero Gateway  \$99 - \$114
There are two versions of this 8-channel gateway. The RAK7246 (\$99) and the RAK7246C (\$114) which includes a GPS receiver.

External fiberglass antenna for the gateway 5.8dBi (860-930MHz) \$49.95 + \$7.95
	This antenna from Sparkfun (<https://www.sparkfun.com/products/15597>) provided a significant improvement with LoRa reception. This antenna requires an interface cable N to RP-SMA Cable – 1m  (<https://www.sparkfun.com/products/14911>).

All weather enclosure for the RAK Raspberry Pi gateways  \$89
This can be used to deploy a Raspberry Pi gateway that will be set outside. This required some additional parts:
- USB external WiFi antenna [\$9.99] for the RPi Gateway (<https://www.ebay.com/itm/123990900283>) to attach to the USB port on the RPi and this WiFi antenna [\$4.99] to attached to the gateway enclosure (<https://www.data-alliance.net/antenna-5dbi-2-4ghz-5ghz-options-outdoor-omnidirectional-w-n-male-right-angle/>) .
- We also needed a U.FL-Male To RP-SMA-Male Adapter [$4.70] (<https://www.data-alliance.net/u-fl-male-to-rp-sma-male-adapter-u-fl-connector-snaps-for-permanent-fit>/).
- The outdoor enclosure is packaged with a cable gland for the power-over-ethernet adapter that comes with the enclosure but it does not have a gland for a power cable if the Pi will be powered using other means. We used this M25 x 1.5 metric thread gland [$4.25] (<https://www.ebay.com/itm/352874323191>).

Battery for a 2-month deployment  \$25.95 for 2
We were planning to use a SAFT LS14500 Size AA 3.6V 2600mAh primary lithium battery for the initial deployment but we discoved that the battery can react violently when exposed to water so we are considering alturnatives. 

# Component Weights
* Gnat 2.8g
* GNSS Antenna 8.0g
* LoRa antenna 0.1g
* Lithium ion 400 mAh battery 8.7g
* Lithium ion 110 mAh battery 2.8g
* Lithium ion 40 mAh battery 1.5g
* PVC enclosure 27g
* Heat-shrink tube double-walled with adhesive 10 cm 7g
* Thin coat of silicone ~3g

## Assembled Mass of Tracker
* with 110mAh battery in heat-shrink tube 19g
* with AA battery in heat-shrink tubing 35g
* with PVC and epoxy ~56g
* with 400mAh battery and 30mm x 25mm solar panel in heat-shrink ~30g
