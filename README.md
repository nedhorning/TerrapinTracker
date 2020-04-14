# TerrapinTracker
## Introduction
The goal of this project is to develop a lightweight, low power consumption tracker that can be used for relatively light (>200 g) animals. The primary components of the tracker are a GNSS receiver and a LoRa radio transceiver. The initial species we are targeting for tracking is the diamondback terrapin (DBT, Malaclemys terrapin) but the tracker can be adapted to other taxa. 

The information in this repository is currently focused on the tracking the DBT but the longer term goal is to develop a workflow that will make it easier to design and create purpose-built GNSS/radio trackers for a wide range or animals. 

## Current thinking:
    • Power with a battery using tide timing to dictate GNSS sampling frequency (e.g., high tide/low tide). 

## Near-term experiments: 
    • Test of different node antennas
    • Store GPS readings when LoRa communication is not working
    • Research way to reduce current

## Future:
    • Add solar charging
    • Add additional logic (GNSS frequency based on movement or stationary) and hardware (water sensor to detect when tracker is underwater)
    • Test alternatives to the gateway – node architecture such as a LoRa mesh network, low cost and low power relays and multiple Gateways. 
    • Develop a workflow to more easily create purpose-built boards with updated components
