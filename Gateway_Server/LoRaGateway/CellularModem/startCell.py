# -*- coding: utf-8 -*-
from Hologram.HologramCloud import HologramCloud
hologram = HologramCloud(dict(), network='cellular')
result = hologram.network.connect()
if (result):
    print ("Connected to Hologram")
else:
    print("Not connected")

