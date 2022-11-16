import numpy as np

def readAndGetList(fileName, getOther):
  data = []
  f = open(fileName)
  for line in f:
    s = line.strip().split(" ")[0]
    if s != "":
      data.append(float(s))
      #print("Data ", s)
    else:
     data.append(-1)
    if(getOther):
      unfeseable = line.strip().split(" ")[1]
      if unfeseable.isnumeric() :
        data.append("-")
      else:
        data.append(line.strip().split(" ")[1])
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
  # print("Line: " + line[0])
  rresult[line[0]] = line[1]

for x in folders:
  #allDataResult[x] = readAndGetList("Results/" + x + "/tempTest", True)
  allDataResult[x] = readAndGetList("Results/" + x + "/tempTest", False)
  allDataTime[x] = readAndGetList("Results/" + x + "/tempTime", False)
  # print(allDataResult[x])
  # print(allDataTime[x])

# Now we have all the data to from all the files :3, now we can
# start getting the measurments

# measure A
for x in folders:
  #print("time for "+ x +" ["+ min(allDataTime[x]) + "," + max(allDataTime[x]) + "]")
  #print(allDataResult[x][0])
  result[x] = []
  result[x].append(int(round((sum(allDataResult[x])/len(allDataResult[x])), 5)))
  result[x].append(int(round((sum(allDataTime[x])/ len(allDataTime[x])), 5)))
  
  result[x].append(int(round((max(allDataResult[x])), 5)))
  
# With all the measures, we generate the final table to be placed on latex
finalFile = open("LatexTable.txt","w")
finalFile.write("\\hline \n")

for x in folders:
  #Instance0 & item 12 & item 13 \\  
  # \hline
  Kresult = -1
  id = x
  # print(x)
  instanceName = x.replace(".txt","")
  Kresult = rresult[id]
  #newL = "\\verb|{}|  \t & {} \t & {} [{}] \t & {} \\\\ \\hline \n ".format(instanceName, Kresult, result[x][0] ,allDataResult[x][1], result[x][1])
  newL = "\\verb|{}|  \t & {} \t & {} \t & {} \t & {} \\\\ \\hline \n ".format(instanceName, Kresult, result[x][2], result[x][0], result[x][1])
  finalFile.write(newL)
  
	

finalFile.close()


