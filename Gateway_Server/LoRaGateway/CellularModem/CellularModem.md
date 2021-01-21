Cellular modem

We are experimenting with different cellular modems to make it possible to upload data from a LoRaWAN gateway to the Internet if there is no WiFi or cable connection available. This approach requires cellular reception but the same workflow could be used with a satellite modem which would work just about anywhere there is a clear view of the sky.

The easiest of the two methods to integrate a cellular modem is to purchase the RAK Wireless LoRa Raspberry Pi 3 B+ Gateway with cellular modem inside (RAK7243C). The RAK Wireless website has good instructions to set that up. In our case we used a Nova Hologram SIM card since that provide an option to use any cellular network instead of being tied to a single network. 

These are abbreviated installation instructions and should be used to suppliment the instructions on the RakWireless website.

``` bash
Setup LTE on RPi

APN = hologram

sudo gateway-config
Configure LTE Module
Disable LTE Automatic Dial-up
Quit
Reboot
sudo minicom -D /dev/ttyAMA0 -b 115200
at    // Verify you get “OK”
at+cops=?     // Scan for cell networks
at+cops=1,0,XXX,YYY  where XXX is name of carrier (e.g., AT&T) and YYY is the number  

at+cops=0   // I used this when the above two commands gave me CME ERROR: 30

before closing parenthesis for that carrier For example: at+cops=1,0,Verizon,7

Enable LTE Automatic Dial-up

Exit minicom Ctrl a then x the select “yes”

```


The other setup was on the RAK Wireless LoRa Raspberry Pi Zero Gateway (RAK7246). This RPi Zero gateway does not have an option for a cellular modem so we purchased a NOVA-R410 Global IoT Cellular USB Modem and again, used the Hologram SIM card. A good set of instructions by Hologram staff can be found on Hackster: <https://www.hackster.io/hologram/add-cellular-to-a-raspberry-pi-with-hologram-nova-ea5926>.

The USB modem requires typing in some Python commands or running a python script to activate it. This can be set up to run automatically when the RPi boots but we did not do that. 

These brief instruction can be used with the website noted above:

APN = hologram

Install Hologram
  - sudo apt-get update
  - curl -L hologram.io/python-install | bash
  - sudo hologram version   // Use this to verify things are working
  - sudo hologram send "Hello Worlds"  // Check if this message made it to the console: https://dashboard.hologram.io/org/37620/?drawer=full

To activate the modem to send all data to the Internet via Hologram start Python3 using: sudo python3
  - from Hologram.HologramCloud import HologramCloud
  - hologram = HologramCloud(dict(), network='cellular')
  - result = hologram.network.connect()
or use:  sudo python3 startCell.py     [Note that this python script in on the 

More information on the Hologram Python SDK: <https://www.hologram.io/references/python-sdk>
