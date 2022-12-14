#!/bin/bash

#clear
if(($1 == 0));then
	make clean
	make
fi

if(($2 == 0));then

 echo "./EA new ../InstancesDePrueba/instancia3.mcsb 548792 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} "
 ./EA new ../InstancesDePrueba/instancia4.mcsb 548792 $3 $4 $5
fi
if(($2 == 1));then

 echo "./EA new ../InstanciasReales/ALL.dat.1 189438  $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12}"
 ./EA new ../InstanciasReales/ALL.dat.1 29529010 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12}

fi
