**How to set up a DigitalOcean Droplet to support a LoRaWAN network with multiple Raspberry Pi gateways**

Working with a single Raspberry Pi gateway I was able to install the entire ChirpStack installation on the Raspberry Pi. When I decided to add multiple gateways to my LoRaWAN network I had to set up a server to host the ChirpStack Network Server and Application Server on another computer. An easy solution for me was to create a server (Droplet) on DigitalOcean. This guide provides information on how to do this setup. 

The basic steps are:
   1. Create a Droplet on DigitalOcean
   2. Install ChirpStack Gateway-Bridge on one or more Raspberry Pi
   3. Install ChirpStack Network Server and Application Server on the DigitalOcean Droplet
   4. Configure DigitalOcean firewall
   5. Add MQTT security


**Create a Droplet on DigitalOcean**
To create a DigitalOcean account and a Droplet (server) go to https://www.digitalocean.com. I signed up for the lowest cost Standard Droplets plan (1 GB Memory / 25 GB Disk) for $5 per month. I installed an Ubuntu 18.04.3 x64 image for the server. Make a note of the Droplet hostname and password since you will need those to access the Droplet.

**Install ChirpStack Gateway-Bridge on one or more Raspberry Pi**
Instructions to install the ChirpStack Gateway-Bridge on a Raspberry Pi can be found in my other guides. During the installation process you will need to edit the chirpstack-gateway-bridge.toml file (located in /etc/chirpstack-gateway-bridge) to point to your DigitalOcean MQTT server. You might need to run “sudo su” first to have permission to edit the file. 

Search for “[integration.mqtt.auth.generic]” and replace: 
>	server="tcp://127.0.0.1:1883" 
with: 
>	server="tcp://206.117.190.51:1883"

Make sure you use your DigtalOcean IP address. After saving the file restart the gateway: 

	``` bash
	sudo systemctl restart chirpstack-gateway-bridge on the RPi
	```

**Install ChirpStack Network Server and Application Server on the DigitalOcean Droplet**
Log in to your DigitalOcean Ubuntu server using ssh. For example:

	``` bash	
ssh root@ 206.117.190.51
	```

To install the Network Server and Application Serve I followed the ChirpStack Quickstart Debian or Ubuntu guide (https://www.chirpstack.io/project/guides/debian-ubuntu/). You can skip the “Install ChirpStack Gateway Bridge” step since that will be installed on the Raspberry Pi. 

For the “Installing ChirpStack Application Server” step you need to change one additional configuration in the /etc/chirpstack-application-server/chirpstack-application-server.toml file. Under the [application_server.integration] tag change: 
	marshaler="json_v3" 
to: 
	marshaler="json". 

Remember to restart the application server: 

	``` bash
	sudo systemctl restart chirpstack-application-server
	```

Configure DigitalOcean firewall
In order to allow connections to port 8883 on the DigitalOcean server update the firewall settings:
	sudo ufw allow 8883

Add MQTT security
Although ChirpStack will work without adding MQTT security it is recommended that you update the Mosquitto (MQTT server used by ChirpStack) settings to make the MQTT server (broker) more secure. More information about securing Mosquitto on DigitalOcean can be found here: https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-ubuntu-18-04
