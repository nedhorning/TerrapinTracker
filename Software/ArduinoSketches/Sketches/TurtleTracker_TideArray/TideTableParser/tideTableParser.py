import numpy as np
import datetime
import time
from pathlib import Path
import sys

isHeaderLine = False

if len(sys.argv) != 2:
    print("ERROR! Command should be: python3 tideTableParser.py filename")
    sys.exit()

inputFilename = Path(sys.argv[1])
#inputFilename = Path("/media/ned/Data1/AMNH/TurtleTracker/Tides/8535805_annual (1).txt")
newName = inputFilename.stem + "_linuxTime" + inputFilename.suffix
outputFilename = inputFilename.parent / newName

with open(inputFilename, "r") as tideFile:
    while (isHeaderLine == False):
        line = tideFile.readline()
        if line != "\n":
            isHeaderLine = line.split()[0] == 'Date'  
    data = tideFile.readlines()
    data_array = np.array([line.split() for line in data])

dateArray = data_array[:,0]
timeArray = data_array[:,2]
tideArray = data_array[:,5]
linuxTime = []

for i in range(dateArray.size):
    if tideArray[i] == 'H':
        dateTime = dateArray[i] + " " + timeArray[i]
        dateFormated = datetime.datetime.strptime(dateTime, '%Y/%m/%d %H:%M').replace(tzinfo=datetime.timezone.utc).timestamp()
        linuxTime.append( int(dateFormated))
linuxTime = np.array(linuxTime)
np.savetxt(outputFilename, [linuxTime], fmt='%d', delimiter=',', newline='', header='{', footer='};', comments='')


