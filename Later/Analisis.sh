#!/bin/bash

# Create a file for python to read
instanceFolder="Instancias"
tmpInstances="tmpTest.txt"
pythonCode="Analisis.py"

[ -e $tmpInstances ] &&rm -r $tmpInstances
# obtain the instances
nIns=0
cd $instanceFolder
for x in *.dat.*; do
  echo "${x}" >> $tmpInstances
  nIns=$(($nIns+1))
done
for x in *.mcsb; do
  echo "${x}" >> $tmpInstances
  nIns=$(($nIns+1))
done
mv $tmpInstances ../
cd ..

# now run the python code
# it should return the latex table in a file named LatexTable.txt
python3 $pythonCode
[ -e $tmpInstances ] &&rm -r $tmpInstances

echo "Analisis all done, have a nice day!"
