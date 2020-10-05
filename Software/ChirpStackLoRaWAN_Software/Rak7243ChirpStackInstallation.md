**Install ChirpStack on a RAK7243(C) Raspberry Pi gateway**

There are several different ways to configure a RAK7243 or RAK7243C Raspberry Pi (RPi) gateway. The following three options are the ones I tried. Details on these methods follows after this brief overview.

 1. Download and install the desktop version of the Raspberry Pi OS (<https://www.raspberrypi.org/downloads/raspberry-pi-os/>) and then follow the instructions to install ChirpStack on a Debian (such as Ubuntu) system: <https://www.chirpstack.io/guides/debian-ubuntu/>. This option provides a familiar desktop and is helpful for people who prefer to interact via a desktop instead of a command line. With this approach you need to install firmware for the RAK2245 concentrator: <https://github.com/RAKWireless/rak_common_for_gateway>. There are more steps to accomplish this setup compared to the other two options but the benefit of having a desktop could be advantageous. This is currently my preferred method. 
 2. Download an RPi image from RAK (<https://downloads.rakwireless.com/LoRa/Pilot-Gateway-Pro-RAK7243/Firmware/>) that is preconfigured with Raspberry Pi OS lite (formally Raspbian lite) and ChirpStack. This is likely the easiest way to install and operating system for the RPi and ChirpStack. The downside is that Raspberry Pi OS lite does not have a desktop so all configuration and other interactions with the RPi must be done using a command line via SSH or a terminal connected to the RPi. The RAK website has a helpful online guide (<https://doc.rakwireless.com/rak2245-pi-hat-edition-lorawan-gateway-concentrator-module/device-firmware-setup>) to install the RPi operating system and ChirpStack software. Since the RAK image has ChirpStack preconfigured you can quickly get up and running and have a LoRa node communicating with Gateway and viewing data on the web-based graphical interface. 
 3. Download and install the RPi image provided by ChirpStack: (<https://www.chirpstack.io/gateway-os/install/raspberrypi/>). This uses Yocto instead of the Raspberry Pi OS and as such is, for the most part, dedicated to ChirpStack and it’s not easy to add other applications. This is compatible with the RAK RPi gateway. The lack of a familiar operating system is a drawback for me but it is well suited if you just want your RPi dedicated to be a ChirpStack gateway and don’t need to add other software.

Notes for installing on a desktop version of the Raspberry Pi OS (Option 1 from above):
Download Raspberry Pi OS from here: <https://www.raspberrypi.org/downloads/raspberry-pi-os/>. Any of the install options should work although I selected the “desktop and recommended software” option. 

Use Balena Etcher to create bootable SD card. Boot the RPi with the new SD card and follow the onscreen guide to configure and update software. 

When you insert the SD card and boot the RPi for the first time, follow the instructions to configure the settings. Updating software at the end of the configuration can take several minutes. When complete you will need to restart the RPi. 

To enable the ability to ssh into the RPi you need to use the RPi configuration tool. Also, to enable the RakWireless LoRa concentrator you need to enable the SPI,  I2C and serial interface using that same configuration tool. To enable those features, enter “sudo raspi-config” in a terminal window. Select “Interfacing Options” from the interface. Select “SSH” then select ‘Yes”, then “Ok”. Do the same to turn on “SPI” and “I2C”. For “Serial” you need to disable the login shell over serial and enable serial port hardware. Close the configuration window by selecting “Finish”. When complete you should restart the RPi. 

Follow the Rak GitHub instructions: <https://github.com/RAKWireless/rak_common_for_gateway>. Those instructions suggest using the Lite version of RPi OS but they installation also works for the desktop version. In summary, the instructions to install the gateway software from the Rak Wireless GitHub repository are:
    • git clone https://github.com/RAKWireless/rak_common_for_gateway.git ~/rak_common_for_gateway   [Note that if git isn’t installed you will need to install it using: sudo apt update; sudo apt install git -y]
    • cd ~/rak_common_for_gateway
    • sudo ./install.sh  [You will need to select the gateway model. In my case I used “3”.]

After installation is done you can configure the LoRa gateway concentrator using “sudo gateway-config”. You can follow the RakWireless instructions for Cconfiguring the Gateway”: https://doc.rakwireless.com/rak2245-pi-hat-edition-lorawan-gateway-concentrator-module/configuring-the-gateway. When configuring the Gateway under the second option, “Setup RAK Gateway LoRa concentrator” select “2 Server is ChirpStack”. For channel plan select “ChirsStack Channel-plan configuration” and if you are in the US the channel plan should be US_902_928. For other countries you’ll need to check for the channel plan that is designated (legal) for your region. The “SERVIR_IP” can be set to the default “127.0.0.1” (IP address for localhost which is the address of your own server). Now your RPi should be set up to connect to the ChirpStack Application Server which you can use to configure ChirpStack to communicate with your LoRa nodes. See the “Connect to the ChirpStack Application Gateway and configure it to listen to your LoRa nodes” section below to start running your LoRaWAN.

Once that is set up you can connect to the ChirpStack Application Gateway.

Notes for using the RAK RPi image (Option 2 from above):
The RAK website has a helpful online guide (<https://doc.rakwireless.com/rak2245-pi-hat-edition-lorawan-gateway-concentrator-module/device-firmware-setup>) to install the RPi operating system and ChirpStack software. Follow that guide but note that when booting the RPi for the first time with the new image you need to access the RPi gateway. The online RAK instructions provide information about how to do that using another computer. If you have your RPi connected to a terminal and keyboard you can skip that “Accessing your Gateway” page of the instructions since you are connecting directly to the RPi.

Once the WiFi settings are completed under “Accessing the Internet” you can restart the RPi and then ssh into the RPi as long as your computer is on the WiFi router with the SSID that you specified in the configuration. To do that you’ll need to find the RPi IP address. You can do that by logging into your router or looking for a Raspberry Pi device on your network using a mobile app such as Network Scanner. If you have a terminal connected to the RPi you can type “ifconfig” in a terminal window and look for the “inet” number under WLAN0 or something similar. For example for me to SSH into my RPi I type ssh pi@10.0.0.50 and then enter my RPi password. Sometimes it can take a while for the wireless IP address to be established so it might take a few minutes before the RPi can be seen on the network.

When configuring the Gateway under the second option, “Setup RAK Gateway LoRa concentrator” select “2 Server is ChirpStack”. For channel plan select ChirsStack Channel-plan configuration” and if you are in the US the channel plan should be US_902_928. For other countries you’ll need to check for the channel plan that is designated (legal) for your region. The “SERVIR_IP” can be set to the default “127.0.0.1” (IP address for localhost which is the address of your own server). Now your RPi should be set up to connect to the ChirpStack Application Server to configure ChirpStack to listen to yur LoRa nodes. See the “Connect to the ChirpStack Application Gateway and configure it to listen to your LoRa nodes” section below to start running your LoRaWAN.

Once that is set up you can connect to the ChirpStack Application Gateway.

Notes for using the ChirpStack Gateway OS image Option 3 from above):
Follow the directions on the ChirpStack Gateway OS web page (https://www.chirpstack.io/gateway-os/overview/) for the raspberry Pi. If you want to install all of the software on the Raspberry Pi you should use the “full” version. This is the file I downloaded: chirpstack-gateway-os-full-raspberrypi3-20200408112223.rootfs.wic.gz. After you uncompress the file you can use balenaEtcher to burn the image to an SD card. 

When you boot the RPi for the first time (see the guide for options to log in) you can run “sudo gateway-config” to set up the LoRa concentrator:

> Select your shield.  I used “RAK2245”
> Select channel plan. I used “US915”
> Select the channel-block. I used “Channels 8 – 15 + 65”
> Respond “Yes” and “OK” to the messages to appear

Once that is set up you can connect to the ChirpStack Application Gateway.
