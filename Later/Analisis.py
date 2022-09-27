#!/usr/bin/python

import numpy as np
# Constans
latextxt = "LatexTable.txt"
tmpFilestxt = "tmpTest.txt"
resultFolder = "Results"
resulttxt = "result.txt"
timetxt = "time.txt"
SNTime = 6
SNResult = 4

def readAndGetList(fileName):
  data = []
  f = open(fileName)
  for line in f:
    data.append(float(line.strip()))
  return data
  
def GNumber(n, s = 6):
  significantN = round(n, (s-1))
  significantS = str(significantN)
  noNumber = significantS.replace(".","")
  if len(noNumber) < s:
    significantS += "0" * (s - len(noNumber))
  if len(noNumber) > s:
    significantS = signifincantS.split(".")[0]
  return significantS

# Read the file, normally will be tmpTest.txt
insFile = open(tmpFilestxt)
folders = []
# file data
allDataTime = {}
allDataResult = {}
# measure data
result = {}

for line in insFile:
  line = line.strip()
  folders.append(line)

for x in folders:
  allDataResult[x] = readAndGetList(resultFolder + "/" + x + "/" + resulttxt)
  allDataTime[x] = readAndGetList(resultFolder + "/" + x + "/" + timetxt)

# Now we have all the data to from all the files :3, now we can
# start getting the measurments

# measure A
for x in folders:
  #print("time for "+ x +" ["+ min(allDataTime[x]) + "," + max(allDataTime[x]) + "]")
  result[x] = []
  # result related
  result[x].append(GNumber(min(allDataResult[x]),SNResult))
  result[x].append(GNumber(np.mean(allDataResult[x]),SNResult))
  result[x].append(GNumber(np.std(allDataResult[x]), SNResult))
  # Time related
  result[x].append(GNumber(sum(allDataTime[x])/len(allDataTime[x]),SNTime))
  result[x].append(GNumber(min(allDataTime[x]),SNTime))
  result[x].append(GNumber(max(allDataTime[x]),SNTime))
  
# With all the measures, we generate the final table to be placed on latex
finalFile = open(latextxt,"w")
finalFile.write("\\begin{tabular}{ | m{7.5em} | m{1.2cm}| m{1.2cm} | m{1.2cm} | m{1.4cm} | m{1.4cm} | m{1.4cm} |}  \n")
finalFile.write("\\hline \n")
# First raw, what is what
newL = "Instance & Best Result & Mean & Std. Deviation & Avg. Time & Min. Time & Max. Time \\\\ \n "
finalFile.write(newL)
finalFile.write("\\hline \n")

for x in folders:
  #Instance0 & item 12 & item 13 \\  
  # \hline
  newL = "\\verb|{}| & {} & {} & {} & {} & {} & {} \\\\ \n ".format(x, result[x][0], result[x][1],result[x][2],result[x][3],
  								result[x][4], result[x][5])
  finalFile.write(newL)
  finalFile.write("\\hline \n")

finalFile.write("\\end{tabular} \n")
finalFile.close()

