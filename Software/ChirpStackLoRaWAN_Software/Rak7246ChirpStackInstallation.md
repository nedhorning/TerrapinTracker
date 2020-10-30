**Install ChirpStack on a Raspberry Pi Zero W Rak7246(G) gateway**

These instructions guide you through the process of installing and configuring ChirpStack on a Raspberry Pi Zero W Rak7246(G) gateway. The guide is based on the ChirpStack OS Getting Started document at <https://www.chirpstack.io/gateway-os/use/getting-started/> with input from Jason (garenglic on ChirpStack forum). ChirpStack OS is a Linux-based embedded OS (using Yocto). 

To start you need to download the ChirpStack OS image from <https://artifacts.chirpstack.io/downloads/chirpstack-gateway-os/raspberrypi/raspberrypi0-wifi/3.3.2/>. There are two versions of  ChirpStack OS the full version which has all of the ChirpStack components and the base which only has the ChirpStack Concentratord and ChirpStack Gateway Bridge. We will be installing the full version since we want to have all of the components installed on the RPi Zero.

After downloading the ChirpStack OS image you need to use software such as Balena Etcher to flash a micro-SD card. Once the flashing is done you can insert the micro-SD card into the Rak7246 gateway and then power it up. Inserting the micro-SD card can be difficult. The writing on the micro-SD card should be facing up and the card should be inserted horizontally at the top of the slot in the plastic case. If the card falls into the case you will need to unscrew and remove the case to access the card.

When the Rak7246 gateway is powered up after a minute or so you should see a WiFi access point named “ChirpStackAP” on your WiFi-enabled computer. Connect to that WiFi access point and then use ssh to log into the Rak7246 gateway. The IP address is 192.168.0.1 and the user name and password are both admin. This ssh command should work in a terminal: ssh admin@192.168.0.1 . Use admin for the password and then you should be logged into the Rak7246 gateway.

Once you are connected to the gateway you need to edit two files:
1.  sudo nano /etc/chirpstack-application-server/chirpstack-application-server.toml
add these lines:

``` bash
    [application_server]
    [application_server.integration]
    [application_server.api]
    bind="0.0.0.0:8001"
    public_host="127.0.0.1:8001"
```

2. sudo nano /etc/chirpstack-gateway-bridge/chirpstack-gateway-bridge.toml
change marshaler="protobuf" to marshaler="json"

Type sudo gateway-config in the terminal to configure the gateway. Again, the password is admin. The first task is to setup the concentrator. Select: 1 Setup LoRa concentrator shield. When you  select the shield use either: 6 RAK2246 or 7 RAK2246G (with GNSS) depending on which gateway you are using. The RAK7246 has the  RAK2246 shield and the The RAK7246G has the  RAK2246G shield. Next, select the channel plan for your area (I selected 4 US915). Lastly select the channel-blocks if you are using the US915 plan. I selected 2 Channels 8 - 15 + 65. Keep selecting OK until you are back at the main screen.

The default gateway ID is 0101010101010101 and if you want to change that to another unique ID you can go to: 2 Edit ChirpStack Conectratord config then select 1 General configuration. In the editor that opens scroll down to the line with “gateway_id=”0101010101010101” and enter a new ID with the same number (16) of hexidecimal characters (0-9 and a-f). For example, I used B827EBFFFE8DD8D5 for my ID.

Next you need to set up the MQTT broker. Select 3 Edit ChirpStack Gateway Bridge config. In the next screen select 2 MQTT connection wizard. In the text box for the MQTT broker address type tcp://127.0.0.1:1883. Unless you have a reason not to, select “No” for the following prompts.

If you want to set up the gateway so you can access it on your local WiFi network you will need to select: 6 Configure WIFI. Otherwise, you can continue to access the gateway using the ChirpStackAP access point. Note that once you specify WiFi settings you cannot use the gateway-config GUI to switch back to using  the ChirpStackAP access point. If you do want to switch back to using the ChirpStackAP access point you need to :
1. Delete the /var/lib/connman/wifi.config file
2. Change Tethering=false back to Tethering=true in /var/lib/connman/settings

At this point the gateway should be set up. To confirm this open a web browser using a computer on the same network as the gateway and go to the Application Server GUI using this address: http://IP address:8080 where “IP address” is the IP address you use to access the gateway. For example since I switched to using WiFi I access the Application Server GUI using http://10.0.0.19:8080 since that is the IP address on my local network. This should open a login screen. The default username and password is admin. Once you log in you can start to configure the gateway. You can use the ConnectToChirpStackApplicationGateway document to setup up the configuration.

If you don’t know the IP address of your gateway after you configure it for WiFi you can use a phone app (I use Network Scanner) to see the IP address of all the devices on a network. You need to ensure that your phone (or computer if you’re using that) are on the same network as the gateway.

This setup assumes you want the full ChirpStack installation on the gateway. If you want to put ChirpStack components on other servers you will need to edit the configuration files as explained in the SetupDigitalOceanServer document. 
