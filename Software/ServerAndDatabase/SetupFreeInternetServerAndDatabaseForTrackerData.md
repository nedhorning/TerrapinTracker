Set up an Internet server and database to store and access data
This example of setting up an Internet server and database uses no-cost versions of Heroku and MongoDB services. This can be suitable for testing but for long term deployments it might be necessary to upgrade the server to another plan. 

The ChirpStack Application Server will make POST requests to the configured endpoints when a node join is complete and when data is received from a node. Although there are other options for sending data using the HTTP POST request is probably the most straightforward. The server side code is written using Node.js. 

The following steps will be outlined in detail below:
    • Download server software from GitHub and setup local test server 
    • Set up a Heroku server account
    • Set up a GitHub account
    • Set up a MongoDB server account and configure MongoDB
    • Deploy code to Heroku server
    • Configure an endpoint integration in the ChirpStack Application Server

Download server software from GitHub and setup local test platform 
I use Visual Studio Code (https://code.visualstudio.com/) as my coding environment for Node.js. There are other options for Node.js and any code editor should work fine although any examples in this guide will be based on Visual Studio Code.

Install Node.js and the Node Package Manager (npm)
You will need to download Node.js and the node package manager (npm). Since npm is distributed with Node.js you only need to install the software for your operating system from this site: https://nodejs.org/en/download/. I used Node.js LTS version 12.2.0. On Ubuntu I used the following commands in the terminal instead of downloading the software from the Node.js website:
sudo apt-get update
sudo apt-get install nodejs-dev node-gyp libssl1.0-dev
sudo apt-get install npm

Download GitHub repo
The code we will be using can be downloaded or cloned from thie GitHub repository: git clone https://github.com/nedhorning/terrapinServer. Once you have the repository you can open it in your code editor. For Visual Studio Code you go to File => Add Foler to Workspace… then select the directory from the GitHub repository “terrapinServer-master”.

Add config/dev.env file
Before running the code locally you need to create a directory named “config” and then add a file named “dev.env” to hold environmental variables for running the server locally. The dev.env file should have the following lines:
PORT=3000
MONGODB_URL=mongodb://127.0.0.1:27017/tracker-data

Run “npm install” from the “terrapinServer-master” directory to install all of the required packages. I run all of my commands using the Terminal in Visual Studio Code (View => Terminal).

Install MongoDB locally
Next we will install MongoDB locally for testing. You can download the MongoDB Community Server (the free version) from the MongoDB website: https://www.mongodb.com/. You can download the most current version and select the correct operating system. On my system I expanded the .tgz file and then renamed the directory name from “mongodb-linux-x86_64-ubuntu1804-4.2.7” to “mongodb” and then moved it to my home directory. I also created an empty directory “mongodb-data” also inside my home directory. To start the database server run this command in the terminal:
	directoryPath/mongodb/bin/mongod --dbpath=directoryPath/mongodb-data

For example, this is the command I use:
	/home/ned/mongodb/bin/mongod --dbpath=/home/ned/mongodb-data
	
After running this command you should see a lot of text scroll by and when that stops the command line will not appear since the MongoDB server is continuing to run. Use Ctrl-c to stop the MongoDB server. 

Install MongoDB Compass (A GUI for MongoDB) to view MongoDB data
Go to the MongoDB Compass download page: https://docs.mongodb.com/compass/master/install/ and follow the installation instructions.

When you start MongoDB Compass for the first time you will need to configure it. You can select any of the privacy setting that you like the click on “Start Using Compass”. Make sure MongoDB is running (see previous step) then at the “New Connection” window click on the “Fill in connection fields individually” link. Use the defaults and click “Connect”.

The next window allows you to create a database but that’s not necessary since the database will be created the first time the local server is run. Once the database is created you can view data by clicking on the connection you just created from the list of connections on the left side of the MongoDB Compass interface. 

Starting the local server to receive LoRa node packets and forward to MongoDB
To start the server to receive LoRa node packets you need to enter this command from the directory that was downloaded from GitHub (“terrapinServer-master”): npm run dev

If you started the MongoDB database server in the Visual Studio Code terminal you will need to open a second terminal using the “+” icon at the top-right portion of the terminal window.

You should see a message “Server is up on port 3000” in the terminal. This means the local server is up and running. If you have the ChirpStack software set up on the RPi and have the Application Server set up properly (see “Set up the Application Server to PUSH data to another server” above) the data coming from the LoRo node should be logged in the MongoDB database. When this first runs the database “tracker-data” and collection “trackerdatas” will be created and it will start to be populated with LoRa packet data. 

Set up a Heroku server account
Once the local setup is working as expected you can work on setting up a database and server system that can be accessed by anyone. I am using Heroku but there are many options out there. First you will need to set up an account with Heroku. No credit card information is necessary and the nice thing about Heroku is that if you need more server resources you can upgrade your account. 

Go to the Heroku site (https://heroku.com) and click on the “Sign up” button. Enter your personal information then click “CREATE FREE ACCOUNT”. You will receive and email and once you click on the link in the email your account should be active. 

Next you should install the Heroku’s command line tools (CLI). Go to https://devcenter.heroku.com/articles/heroku-cli and follow the “Download and install” instructions. For Ubuntu I entered “sudo snap install --classic heroku” in a terminal. You can verify that Heroku was installed properly by typing “heroku -v” in a terminal. That should provide version information. 

Type “heroku login” in a terminal. Open a browser by typing any key except “q” . Next you can login using the email and password you specified when you created the Heroku account. After you are logged in you can close the tab on the browser. You should see a message in the terminal that you are logged in. 

If you don’t already have a GitHub account you need to set one up. To set one up go to https://github.com and follow the instructions to create a free account. 


Set up a GitHub account and creating a Git repository
GitHub is a repository service that uses Git which is a version control system. If you haven’t used Git before it would be good to follow a tutorial to learn about some of the basics of how to use it. We will create a local Git repository and then commit that to GitHub

To start using Git for version control you need to navigate in a terminal to the root project directory which is “terrapinServer-master” in our case. Run the command “git init” to initialize the Git repository. Next enter the following commands (you can look at a tutorial to see what each command does):
git add .
git status
git commit -m “Init commit”

If you get an error “Please tell me who you are” then run the two git config command noted in the message:
git config --global user.email "you@example.com"
git config --global user.name "Your Name"

Once the local Git repository is ready we will need to set up a GitHub repository so the code can be accessed by Heroku. If you don’t already have a GitHub account you need to set one up. To set one up go to https://github.com and follow the instructions to create a free account. 

Once you log into your GitHub account you will need to create a new repository. Log into GitHub and then create a new repository by clicking on the “+” icon in the upper right corner of the GitHub webpage. You need to give is a name and you can decide if you want the repository to be public or private. You can have GitHub create a Readme file which you can edit to provide more information about your project. When the form is filled in click the “Create repository” button. 

The next step is to push the local Git repository to GitHub. To do that you need to copy the repository address and you can get that by clocking on the “Clone or download” button. If the window that appears includes “Clone with SSH” then click on “Use HTTPS” to open the “Clone with HTTPS” window. Copy the address in the text box. In my case the address is: https://github.com/nedhorning/test4Guide.git

Next, use the following two commands inserting the repository address you copied :
git remote add origin addressFromAbove
git push -f origin master


Set up a MongoDB Atlas server account 
Before you can deploy the software to Heroku (or another server) you need to set up a MongoDB database that Heroku can access. For this example I am using MongoDB Atlas which has a free optoin for lot volume use. 

To set up a MongoDB Atlas account go to the Atlas website (https://www.mongodb.com/cloud/atlas) and click “Start free” and fill in the form to create an account. For the cloud provider you can select the Cloud Provider. I selected “AWS”. Then, you need to select a region. Make sure you select one that has “Free Tier Available” . I selected ”N. Virginia”. All other settings can be left at their default values. Click the “Create Cluster” button and after some time (a few minutes) the cluster will be set up. 

To connect to a cluster you need to click on the “CONNECT” button on the “Clusters” page. That opens the “Connect to Cluster0 page. Click on the “Add a Different IP Address” button and then enter 0.0.0.0/0 into the “IP Address text box to whitelist all IPs. Then, click the “Add IP Address” button. 

Next you need to create a MongoDB User. Enter a username and password. The last step is to connect to a database.  Make sure MongoDB Compass is running then click on the “Choose a connection method” button and then click on “Connect with MongDB Compass”. Click on “I have MongoDB Compass” and check the version selection is ok. Then copy the host address portion of the connection string from the link displayed in the text box. That includes everything between the “@” sign and “/admin”. In my case that address is: “cluster0-rvcbb.mongodb.net”. 

In MongoDB Compass click on “New Connection” and then “Fill in connection fields individually” to open the connection form. Paste the host address you just copied to the “Hostname” text box. Leave the port set to the default value of “27017” and click on the “SRV Record to move the toggle to the right. For “Authentication” select “Username / Password” and then type in the username and password you entered when setting up the cluster (not your Atlas username and credentials. The other field(s) can be left as the default. Click “Connect”. There won’t be any data since none has been written to the database. 


Deploy code to Heroku server
to deploy the software to Heroku we will use the  CLI tool installed above. Open Visual Studio Code and then in the terminal type: heroku create uniqueNameForApp. In my case I used “heroku create horning-test-app”.

We will need to create a MONGODB_URL environmental variable on Heroku for the using the Heroku “config” command using a key:value pair. First we need to get the MongoDB Atlas and create another connection. Log into your MongoDB Atlas account then go to the page to create a new connection using the “CONNECT” button. In the “Connect to Cluster0 window click on “Connect your application”. The “DRIVER” should be set to “Node.js” and the “VERSION” set to “3.6 or later”. Copy the text string in the text box for step “2”. You will need to edit that string to replace “<password>” with the password to the cluster that you created above so it might be easiest to past the text string into a text editor. You also need to replace “<dbname>” with a name for the database. If you want to use the same name as above that would be “tracker-data”.

The syntax is: heroku config:set key value. This is the command that needs to be run in the terminal:
heroku config:set MONGODB_URL=yourMongodbConnectionString

In my case the command is:
heroku config:set MONGODB_URL=’mongodb+srv://nedhorning:mypassword@cluster0-rvsbb.mongodb.net/tracker-data?retryWrites=true&w=majority’

Note, if you have special characters in the connection string URLyou will need to add quotes at the beginning and end of the connection string. For Mac or Linux use single quotes and on Windows use double-quotes. 

To verify that the Heroku config worked type “heroku config” and verify the output is correct.

To push the code up to the Heroku server from your GitHub repository type the following in the terminal (it will take a while to run): 
git push heroku master

This is the same command you would run if you update your code. 

The final step is to set the RPi Gateway so it sends data to the Heroku server. To do that we will edit the Application Server Integration as we did above in the section “Set up the Application Server to PUSH data to an endpoint (another server)”. The only change is that you need to set the “Uplink data URL” line to the Heroku URL. The Heroku URL is https://yourAppName.herokuapp.com/data. Another way to get the URL is to type “heroku info” in a terminal and you will see the “Web URL” listed there. 
In my case the URL is: “https://horning-test-app.herokuapp.com/data”
