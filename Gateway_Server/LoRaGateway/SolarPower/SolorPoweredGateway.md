Solar power for a LoRaWAN gateway

The attempt to develop a low cost solar powered LoRaWAN gateway got off to a rough start. The intent was to provide solar charging to a Raspberry Pi (RPi) Zero gateway with a current load of ~450 mA. The first attempt followed the directions at: <https://www.activecountermeasures.com/making-a-solar-powered-raspberry-pi/>. This seemed like a reasonable match for our purpose. Unfortunately the Anker PowerCore 26800mAh battery that was recommended did not perform as expected. This battery does not have pass-through charging capabilities so when the battery was charging it was not possible to power the RPi Zero. Next, I looked for a battery with pass-through charging and came across the RAVPower 26800 mAh dual port battery pack. When I tested that battery I was able to run the RPi Zero as the battery was charging but when the battery became full and the charging circuitry  stopped charging the battery the power to the RPi Zero was interrupted which resulted in a restart of the RPi Zero. Shortly after a restart the battery would start charging from the solar panel again but would quickly stop charging when the battery was full forcing a restart of the RPi Zero. This was clearly not going to work. 

It is quite possible that this problem could be solved by placing a capacitor in the charge circuit so there wouldn’t be an interruption of current flow when the solar charging is turned off but it can be risky tearing apart batteries. 

We designed a LoRaWAN gateway that can be deployed in areas without WiFi access and no access to the electrical grid. These are the components we’re using:

  - Battery [$132.99] - Might Max 12V, 55AH GEL Deep Cycle Battery 
  - Solar panel and controller [$69.99] - Renogy 22.4-in x 13.4-in 30-Watt Solar Panel 
  - Buck converter [$9.99] - DROK Voltage Regulator DC 9V-36V Step Down to DC 5V-5.3V
  - LoRaWAN Gateway [$99] - WisGate Developer D0 (formerly RAK7246)
  - Cellular modem [$74.95] - NOVA-R410 Global IoT Cellular USB Modem
  - SIM card [Free] – Hologram Global IoT SIM card

The power comes from a large battery that is charged with a 30-Watt solar panel. We connect the buck converter directly to the battery to avoid any possible breaks in current coming from the low-budget solar charger that comes with the solar panel. We plug one end of a USB/A-MicroB cable into the buck converter and the other into the power port of the Raspberry Pi Zero in the gateway. We store the buck converter, gateway and cellular model in a waterproof enclosure.

We are using the Raspberry Pi Zero based WisGate Developer D0 gateway since is uses less power that the larger gateways. RAK Wireless sells a Raspberry Pi 3B+ and 4 based gateway that comes with a built-in modem. The Hologram Global IoT SIM card also works with that setup. 

Information about setting up a cellular modem is in the Hardware => LoRaGateway => CellularModem directory and information about setting up the gateway is in the Software => ChirpStackLoRaWAN_Software => Rak7246ChirpStackInstallation document. Details on the 
