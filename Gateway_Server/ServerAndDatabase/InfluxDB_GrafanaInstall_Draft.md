07/10/20

**Install InfluxDB and Grafana**
It can be useful to store data received by a RPi gateway on the RPi itself in addition to sending it to a server. One easy way to do this is to set up an InfluxDB database on the RPi and configure the ChirpStack Application server to automatically send data to an InfluxDB database. With InfluxDB you can query the data and do tihngs like write it out to a CSV file for further processing. 

InfluxDB is a time series database which means it it optimized for storing and serving time series data. One could argue that it is not ideally suited for storing tracker data but one could also argue that there is value in using a simple approach. 

If you would like to easily display data stored in an InfluxDB database in a web browser you can use Grafana. The InfluxDB and Grafana combination is ideal for displaying sensor data but probably less interesting for most tracking applications so working with Grafana is entirely optional.

Installation steps are from Simon Hearne’s webpage: Installing InfluxDB & Grafana on Raspberry Pi: https://simonhearne.com/2020/pi-influx-grafana/ 

ChirpStack integration information is from the ChirpStack website and videos by Anh Quân Tống:
Setup Loraserver - Get data with InfluxDB Integration: https://www.youtube.com/watch?v=aliHXCR564k and Setup Loraserver - Display data on Grafana with InfluxDB Integration: https://www.youtube.com/watch?v=qOAkZMhe_9Y

**Install InfluxDB** 

``` bash
Make sure software is up to date
sudo apt update
sudo apt upgrade -y
```

**Install InfluxDB**

``` bash
wget -qO- https://repos.influxdata.com/influxdb.key | sudo apt-key add -
source /etc/os-release
echo "deb https://repos.influxdata.com/debian $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
sudo apt update && sudo apt install -y influxdb
```

**Start InfluxDB service and set it to start when booting
**
``` bash
sudo systemctl unmask influxdb.service
sudo systemctl start influxdb
sudo systemctl enable influxdb.service
```

Create an InfluxDB database and “grafana” as a user with admin privledges
influx
create database chirpstack
use chirpstack
create user grafana with password 'admin' with all privileges  [You can change ‘admin’ to another password]
grant all privileges on chirpstack to grafana
show users   [This should display grafana as a user and true for admin]
exit

**Install Grafana**
``` bash
wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -
echo "deb https://packages.grafana.com/oss/deb stable main" | sudo tee /etc/apt/sources.list.d/grafana.list


sudo apt update && sudo apt install -y grafana
```

Start Grafana service and set it to start when booting

``` bash
sudo systemctl unmask grafana-server.service
sudo systemctl start grafana-server
sudo systemctl enable grafana-server.service
```

**Add Influx as a Grafana data source**
 - Log into Grafana using a web browser   [If using a browser on your RPi  use http://localhost:3000. If using a browser on a computer connected to the RPi use the IP address for the RPi (e.g., http://10.0.0.44:3000/). 
 - Your initial username and password are both “admin”. You will need to change your password when you log in.
 - Click on “DATA SOURCES” then click on “InfluxDB” under “add data source”.
 - Under “HTTP” set URL to http://localhost:8086
 - Under “InfluxDB Details” enter the Database name “chirpstack” and the username (“grafana”) and password (“admin”). Set “HTTP Method” to “GET”.
 - Click “Save & Test” and if you get a message that the Data source is working the connection between Grafana and InfluxDB is set up

**Configure ChirpStack to send data to InfluxDB database**
 - Make sure you are able to receive data from a LoRa node using your gateway and display those data in the ChirpStack Application web interface. 
 - Open the ChirpStack Application web interface (e.g., http://10.0.0.44:8080/) and then click on “Applications” then select the application you want to configure. Next, click on the “INTEGRATIONS” tab then ”+CREATE”. Under “Integration kind” select “InfluxDB integration”. Fill in the following fields:
   - API endpoint (write):  http://localhost:8086/write
   - Username:  grafana
   - Password:   admin
   - Database name:   chirpstack
   - Retention policy name:  leave blank unless you have a specific retention policy you want to use
   - Select timestamp precision: You can choose the precision you want. I used Second 
   - Click “CREATE INTEGRATION” to save the settings

**Verify data is being stored in the InfluxDB database**
You might need to reboot the RPi before continuing. Log into InfluxDB by typing “influx” in a terminal. Once in InfluxDB type the following commands:
 - use chirpstack
 - show measurements  [This will display the tables for each of the data fields received by the gateway. Note that ChirpStack combines latitude and longitude data into a single “location” object to store in a InfluxDB table]
 - select value from table name   [For example I used: select value from device_frmpayload_data_epe]
 - exit
 - For more detailed information about using InfluxDB you can read the documentation on the InfluxDB website: https://www.influxdata.com/. 

**View data in Grafana**
 - Log into Grafana using a web browser using the information from “Add Influx as a Grafana data source” above
 - Once inside the Grafana web interface select “Create your first dashboard” then “+Add new panel”
 - Click on the dropdown menu labled “default” and select “InfluxDB” at the source for the data. If you do not see “InfluxDB” you will need to redo the step above “Add Influx as a Grafana data source”.
 - For a quick test you can click on the pencil icon to the right of the “A” query and then type in a query. For example I typed “select value from device_frmpayload_data_time” and you will see a graph for that value appear in the panel above. 
 - For more detailed information about presenting data in Grafana you can read the documentation on the Grafana website: https://grafana.com/. 
