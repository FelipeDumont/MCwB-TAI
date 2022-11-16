#!/bin/bash

clear
if(($1 == 0));then
	make clean
	make
fi

if(($2 == 0));then
 #echo "./GA new ../InstancesDePrueba/instancia3.mcsb 548792 1 [=1254=]"
 #./GA new ../InstancesDePrueba/instancia3.mcsb 548792 1
# echo "./GA new ../InstancesDePrueba/instancia6.mcsb 548792 1 $3 $4 [=604=] "
# ./GA new ../InstancesDePrueba/instancia6.mcsb 548792 $3 $4
 echo "./GA new ../InstancesDePrueba/instancia4.mcsb 548792 1 $3 $4  "
 ./GA new ../InstancesDePrueba/instancia4.mcsb 548792 $3 $4
fi
if(($2 == 1));then

 echo "./GA new ../InstanciasReales/5clusters.dat.4 548792 1 $3 $4 [>1307>]"
 ./GA new ../InstanciasReales/5clusters.dat.4 548792 $3 $4
 
 #echo "./GA new ../InstanciasReales/5clusters.dat.5 548792 1 $3 $4 [>3090>]"
 #./GA new ../InstanciasReales/5clusters.dat.5 548792 $3 $4
 
# echo "./GA new ../InstanciasReales/5clusters.dat.4 548792 1 $3 $4 [>1488>]"
# ./GA new ../InstanciasReales/5clusters.dat.4 548792 $3 $4

# echo "./GA new ../InstanciasReales/5clusters.dat.1 548792 1 $3 $4 [>5018>]"
#./GA new ../InstanciasReales/5clusters.dat.1 548792 $3 $4

# echo "./GA new ../InstanciasReales/ALL.dat.1 548792 1 $3 $4 [>16.531>]"
# ./GA new ../InstanciasReales/ALL.dat.1 548792 $3 $4

fi
