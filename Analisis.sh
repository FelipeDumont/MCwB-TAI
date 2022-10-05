#!/bin/bash

# Create a file for python to read
tmpInstances="tmpTest.txt"
pythonCode="Analisis.py"
[ -e $tmpInstances ] &&rm -r $tmpInstances
# obtain the instances
nIns=0
cd Results
for x in *; do
  echo $x >> $tmpInstances
  nIns=$(($nIns+1))
done
mv $tmpInstances ../
cd ..

# now run the python code
python3 $pythonCode
[ -e $tmpInstances ] &&rm -r $tmpInstances

echo "Analisis all done, have a nice day!"
