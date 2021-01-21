Connect to the ChirpStack Application Gateway and configure it to receive data from your LoRa nodes
Follow the last page of the RAK website guide (<https://doc.rakwireless.com/rak2245-pi-hat-edition-lorawan-gateway-concentrator-module/connect-the-lora-gateway-with-chirpstack>) to connect to the gateway ChirpStack Application server. In my case I open an Internet browser (I used Chrome) and then type in the URL: <http://10.0.0.50:8080> which opens a login page for the Application Server. The user name is “admin” and the password is also “admin”. This will open the ChirpStack Application Server web interface. You can use this to configure the gateway to receive data from your LoRa nodes. In the settings below I use the settings for my gateway. I noted cases where you need to enter data unique to your setup. 

If you used the Rak Wireless image much of the configuration has already been filled in so you can skip down to the Service-profiles step.

Network server

``` bash
General
Network server name = nedRakGateway1  [Choose a name relevant to your project] 
Network-server server = 127.0.0.1:8000
Gateway Discovery
“Enable gateway discovery” not checked
TLS Certificates
Do not add anything on this page
```

Gateway-profiles

``` bash
Name = Rak7246G  [Choose a name relevant to your project] 
Enabled channels = 0,1,2
Networ-server =  nedRakGateway1
```

Organizations
Organization name = chirpstack
Display name = ChirpStack
Gateways = check “Organization can have gateways

All users

``` bash
Username = admin
Permissions: check both “Is active” and “Is global admin”
```

Service-profiles

``` bashE
ven if most of the service profile is filled in you need to make sure you check the “Add gateway meta-data” box.
Service-profile name = rakServiceProfile     [Choose a name relevant to your project] 
Network-server = nedRakGateway1     [Should match the name you entered above]
Check “Add gateway meta-data” 
Do not check “Enable network geolocation
use “0” for all three fields at the bottom
```

Add a new device profile that works with OTAA and Cayenne
For the turtle tracker we are using “over the air activation (OTTA) and a custom payload codec.To add a new profile, click on “Device-profiles” then click on “+ create” to create a new profile. Enter the following settings. After making the changes click on “CREATE DEVICE PROFILE”.
General

``` bash
Device-profile name = otaa_cayenne
Network-server = ns
LoRwWAN MAC version = 1.0.2
LoRaWAN Regional Parameters revision = A
Next three fiels are “0”
Uplink interval (seconds) = This depends on the application but you can type a number that can be used to indicate if your node is active or not
Join (OTAA/ABP) tab
Device supports OTAA is checked
CLASS-B tab
Device support Class-B not checked
CLASS-C tab
Device support Class-B not checked
CODEC tab
Payload codec = Select either “Cayenne LPP” or “Custom JavaScript codec functions” (see the “EncodingDecodingLoRaData_Draft.odt” document for more details)
```

Configure the gateway

``` javascript
The gateway should be configured already if you used the Rak Wireless image. Otherwise you can follow these settings to configure the gateway:
Gateway details
Gateway name =  ned_rak1_gateway    [Choose a name relevant to your project] 
Description = Ned’s first RAK gateway     [Choose a description relevant to your project] 
Gateway ID = b827ebfffeade849     [Choose ID for your gateway] 
Network server = nedRakGateway1        [Should match the name you entered above]
“Gateway discovery enabled” not checked
```

This is optional but you can follow these instructions to set the gateway location.
Click on the Gateways link on the left then click on the “GATEWAY CONFIGURATION” tab near  the top of the page. If you want you can enter the “Gateway altitude” in the appropriate text box. Next, use the map to move the icon to your location. To do that you need to first zoom out by clicking on the “-” button several times until you see you location on the map then move the icon to roughly the correct location. To more accurately place the icon zoom in and move the marker again. Repeat this zoom and move step until you are satisfied with the location then click “UPDATE GATEWAY”. 

Add a device
To add a device (Gnat in my case) go to “Applications” then click on the name of an application – the default is “app”. Click on “+ CREATE” and enter the following information.  Below is what I used for my device.
Configuration

``` javascript
Device name = neds_gnat2     [Choose a name relevant to your project] 
Device description = Neds second Gnat     [Choose a description relevant to your project] 
Device EUI = 383434306a379301     [Choose EUI for your LoRa device] 
Device-profile = otaa_cayenne   [Should match the name you entered above]
Disable frame-counter validation not checked
```

After clicking Create Device you need to enter the Application key

``` javascript
Keys (OTAA)
Application key = 9C5E64056A46C2872DED008E20A6F95A    [Choose or generate an application key that you use for your application – this has to be the same number that is used in the Arduino sketch loaded into the tracker]
Gen Application key – leave blank
Click on “SET DEVICE-KEYS”
```

Viewing data from a LoRa node
To see if the Gateway is communicating with the LoRa node you just configured click on “Applications” then clcik on the application that you use to create a device (“app” is the pre-configured application”). Click on the link for the device you just created then click on the “DEVICE DATA” link near the top of the page. Any incoming data packets should be displayed. If you see the data you expect the gateway is working.

Set up the Application Server to PUSH data to an endpoint (another server)
To set up the Application Server to use HTTP PUSH to forward data to another server (see section below “Set up an Internet server and database to store and access data”) you need to configure an Integration. To do that click on “Applications” in the ChirpStack web browser interface then click on the “INTEGRATIONS” tab near the top of the interface. Next, clcik on the “HTTP” link to open the form. For the “Payload marshaler” enter “JSON”. The only other line you need to fill in is “Endpoint  data URL”. This URL specifies where the LoRa packet will be sent. When testing with a server on a local computer you need to specify the IP address of the computer with the MongoDB database and server. Since data is being sent from the RPi it is important that they are on the same network so they can communicate. For example, if you install MongoDB and the server on the computer that you use to SSH into the RPi then you could use the IP address of that computer. You also need to add the port (3000 as defined in the Node.js code) and the “data” directory. For example, I put the following text on the “Uplink data URL” line: “<http://10.0.0.61:3000/data>”. If you are connecting to the Heroku server you would need that URL. For example for Heroku: <https://turtle-server.herokuapp.com/data>. You can get that address from Heroku by logging into your account and via the terminal as described below in the “Deploy code to Heroku server” section. 
