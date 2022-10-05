import numpy as np

def readAndGetList(fileName):
  data = []
  f = open(fileName)
  for line in f:
    s = line.strip().split(" ")[0]
    if s != "":
      data.append(float(s))
    else:
     data.append(-1)
  return data


# Read the file, normally will be tmpTest.txt
insFile = open("tmpTest.txt")
resultFile = open("ResultsComplete")
folders = []
# file data
allDataTime = {}
allDataResult = {}
# measure data
result = {}
# Real Result data
rresult = {}

for line in insFile:
  line = line.strip()
  folders.append(line)
for line in resultFile:
  line = line.strip().split(" ")
  rresult[line[0]] = line[1]

for x in folders:
  allDataResult[x] = readAndGetList("Results/" + x + "/tempTest")
  allDataTime[x] = readAndGetList("Results/" + x + "/tempTime")

# Now we have all the data to from all the files :3, now we can
# start getting the measurments

# measure A
for x in folders:
  #print("time for "+ x +" ["+ min(allDataTime[x]) + "," + max(allDataTime[x]) + "]")
  result[x] = []
  result[x].append(round((allDataResult[x][0]), 5))
  result[x].append(round((allDataTime[x][0]), 5))

  
# With all the measures, we generate the final table to be placed on latex
finalFile = open("LatexTable.txt","w")
finalFile.write("\\hline \n")

for x in folders:
  #Instance0 & item 12 & item 13 \\  
  # \hline
  Kresult = -1
  id = x[:-4]
  Kresult = rresult[id]
  
  newL = "\\verb|{}|  \t & {} \t & {} \t & {} \\\\ \\hline \n ".format(x, Kresult, result[x][0], result[x][1])
  finalFile.write(newL)
  
	

finalFile.close()


