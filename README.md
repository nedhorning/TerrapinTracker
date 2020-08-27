# TerrapinTracker
The goal of this project (https://conservationx.com/project/id/491/terrapintracker) is to develop a lightweight, low power consumption tracker that can be used for relatively small (>200 g) animals. The primary components of the tracker are a GNSS receiver for location data and a LoRa radio transceiver to transmit the data in real-time. The initial species we are targeting for tracking is the diamondback terrapin (DBT, Malaclemys terrapin) but the tracker can be adapted to other taxa. This system is best adapted to animals with a range of a few kilometers square where real-time and relatively precise (+/- a few meters) location data is needed.

This project was motivated by a biologist interested in logging movements of several (~15) Diamondback Terrapin over a two-month period. That interest was connected with someone with animal movement data processing experience as well as hobby-level skills in electronics. The result is the TerrapinTracker project. A growing interest in the project as well as technical feedback from the community is helping us achieve and even exceed our goals. Our hope is that this project will develop into a community interested in open hardware and software development and solutions for tracking animals.

The information in this repository is currently focused on the tracking the DBT but the longer term goal is to develop a workflow that will make it easier to design and create purpose-built GNSS/radio trackers for a wide range or animals.

This repository includes information about the hardware and software components required for a functioning monitoring system. We have a working prototype but are still in the experimental stage with many design options being tested. Please reach out if you have an interest in contributing, commenting or simply want to follow along.

Below is a figure illustrating the following core components of the tracker system:
## GNSS receiver on the LoRa node:
Provides periodic turtle location information

## LoRaWAN node (on the turtle) and stationary gateway radio(s): 
The node sends location and other data from a turtle to the gateway and the gateway send the data to the Internet. The gateway antenna should be mounted as high as is practical.

## Server(s) for managing LoRaWAN communication:
Decode data sent from turtles then process and forward data to a database.

## Database (we are using MongoDB) for storing data and making it accessible:
Add data to a database that can be queried so data can be downloaded for analysis


![TrackerSystem](Images/TrackerSystem.png)



