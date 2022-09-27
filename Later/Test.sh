#!/bin/bash

# Constants
mainFolder="Results"
programFolder="Greedy"
instanceFolder="Instancias"
tempTime="tmpTime"
tempTest="tmpTest"
finalFile="Greed"
# clean the remaining files
[ -e $mainFolder ] &&rm -r $mainFolder
[ -e $tempTime ] && rm $tempTime
[ -e $tempTime ] && rm $tempTime
[ -e "LatexTable.txt" ] && rm "LatexTable.txt"


# obtain the instances
nIns=0
ins=()
insIds=()
cd $instanceFolder
for x in *.dat.*; do
  ins[$nIns]=$x
  insIds[$nIns]=$x #"${x%.*}"
  nIns=$(($nIns+1))
done
for x in *.mcsb; do
  ins[$nIns]=$x
  insIds[$nIns]=$x #"${x%.*}"
  nIns=$(($nIns+1))
done
cd ..

# obtain params
nparam=0
params=()
while read line; do 
  params[$nparam]=$line
  nparam=$((nparam+1))
done < params.txt

# obtain seeds
nseed=0
seeds=()
while read line; do 
  seeds[$nseed]=$line
  nseed=$((nseed+1))
done < seeds.txt

# Create folder if it does not exist
function createIfNotExist()
{
  if ! [ -d $1 ]; then 
    # echo "creating" $1 "Folder"
    mkdir $1
  fi
}


function readAndOrganize(){
  results=()
  while read line; do
    arrLine=(${line//;/ })
    #echo "${arrLine[0]} . ${arrLine[1]}"
    results[${arrLine[0]}]=${arrLine[1]}
  done < $2
  
  for (( si=0; si <$1; si++));do
    echo "${results[si]}" >> $3
  done
}
# input for the bash will be
# $1 : number of subprocesses, default 1 -> $subP
# $2 : make clean, default false
subP=1
if ! [ -z $1 ]; then
  # echo "set $1"
  subP=$1
fi
clean=false
if ! [ -z $2 ]; then
  clean=$2
fi

if $clean; then
  echo "Clean With Makefile"
  cd $programFolder
  make clean
  make
  cd ..
fi

# run, not the same files !
createIfNotExist $mainFolder

for (( cparam=0; cparam <$nparam; cparam++));do
  # echo ${params[$cparam]}
  for (( cins=0; cins <nIns; cins++));do
    createIfNotExist "$mainFolder/${insIds[$cins]}"
    result=()
    nowComplete=$(date +%s.%N)
    for (( si=0; si < $nseed; si++));do
      # echo "wait for processes ... $si > $subP"
      if  (( $si >= $subP )); then    
        wait -n
      fi
      (        
        now=$(date +%s.%N) &&
        result=$(./$programFolder/$finalFile "$instanceFolder/${ins[$cins]}" ${seeds[$si]} ${params[$cparam]}) &&
        rt=$( echo "$(date +%s.%N)-$now" | bc -l) &&
        echo "$si;$rt" >> $tempTime &&
        echo "$si;$result" >> $tempTest
        # echo "finished $si"
      ) &
    done
    wait # wait for the rest of subprocesses
    readAndOrganize $nseed "$tempTime" "$mainFolder/${insIds[$cins]}/time.txt"
    readAndOrganize $nseed "$tempTest" "$mainFolder/${insIds[$cins]}/result.txt"
    
    rm "$tempTime"
    rm "$tempTest"
    rtComplete=$( echo "$(date +%s.%N)-$nowComplete" | bc -l)
    echo "Finished [$mainFolder/$innerFolder$cins => ${ins[$cins]}] total: $rtComplete"
  done
done

echo "Testing all done, Time for the Analisis"

bash Analisis.sh




