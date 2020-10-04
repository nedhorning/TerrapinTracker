Reed switch

We are adding a normally closed (NC) reed switch to the positive power wire from the battery before we place the tracker inside its enclosure. 

The switch we are using is from Standex-Meder Electronics with these specifications: Switch Reed N.O./N.C. 0.2A 30VAC 30VDC 3VA 1ms 100mOhm. The switch has three wires so it can be used as either a normally open or normally closed switch.
 
![](../../images/switch.png)

We cut off the straight wire next to the bent wire to have a normally closed switch that opens when a magnetic field is applied. This allows us to turn off the tracker when it is not in use and also allows us to restart the Arduino sketch if necessary by placing a magnet near the switch. 
