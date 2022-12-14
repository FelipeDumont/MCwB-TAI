#!/bin/bash

# example call
# bash Test.sh 5 true 0

# Constants
mainFolder="Results"
programFolder="EA"
instanceFolder="InstancesDePrueba"
instanceFolder2="InstanciasReales"
finalFile="EA"
paramFile="params.txt"
seedsFile="seeds.txt"
tempTime="tempTime"
tempTest="tempTest"
# clean the remaining files
[ -e $mainFolder ] &&rm -r $mainFolder
[ -e $tempTime ] && rm $tempTime
[ -e $tempTime ] && rm $tempTime
[ -e "LatexTable.txt" ] && rm "LatexTable.txt"


# obtain the instances
nIns=0
ins=()
insIds=()

# input for the bash will be
# $1 : number of subprocesses, default 1 -> $subP
# $2 : make clean, default false

if(( $3 == 0 )); then
  cd $instanceFolder
  for x in *; do
    ins[$nIns]="new $instanceFolder/$x"
    insIds[$nIns]=$x
    nIns=$(($nIns+1))
  done
  cd ..
fi

if(( $3 != 0 )); then
  cd $instanceFolder2
  for x in *; do
    ins[$nIns]="new $instanceFolder2/$x"
    insIds[$nIns]=$x
    nIns=$(($nIns+1))
  done
  cd ..
fi

#cd $instanceFolderO
#for x in *.*; do
#  ins[$nIns]="$instanceFolderO/$x"
#  insIds[$nIns]=$x
#  nIns=$(($nIns+1))
#done
#cd ..

# obtain params
nparam=0
params=()
while read line; do 
  params[$nparam]=$line
  nparam=$((nparam+1))
done < $paramFile

# obtain seeds
nseed=0
seeds=()
while read line; do 
  seeds[$nseed]=$line
  nseed=$((nseed+1))
done < $seedsFile

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
  # echo "read and org"
  while read line; do
    arrLine=(${line//;/ })
    data=${arrLine[1]}
    for ((i=2; i<${#arrLine[@]};i++)); do
    	data=$data" "${arrLine[$i]}
    done
    results[${arrLine[0]}]=$data
  done < $2
  
  for (( si=0; si <$1; si++));do
    echo "${results[si]}" >> $3
  done
}

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
  echo ${params[$cparam]}
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
        echo "${ins[$cins]}" ${seeds[$si]} ${params[$cparam]} &&
        now=$(date +%s.%N) &&
       	result=$(./$programFolder/$finalFile ${ins[$cins]} ${seeds[$si]} ${params[$cparam]}) &&
        rt=$( echo "$(date +%s.%N)-$now" | bc -l) &&
        echo "$si;$rt">>$tempTime &&
        echo $si";"$result>>$tempTest
        # echo "FINISHED ${ins[$cins]}" ${seeds[$si]} ${params[$cparam]}
      ) &
    done
    wait # wait for the rest of subprocesses
    readAndOrganize $nseed "$tempTime" "$mainFolder/${insIds[$cins]}/$tempTime"
    readAndOrganize $nseed "$tempTest" "$mainFolder/${insIds[$cins]}/$tempTest"
    # echo "Reorganized ---"
    #break;
    rm "$tempTime"
    rm "$tempTest"
    rtComplete=$( echo "$(date +%s.%N)-$nowComplete" | bc -l)
    
    # echo "Finished [$mainFolder/$innerFolder$cins => ${insIds[$cins]}] total: $rtComplete"
  done
  wait -n
done

# echo "Testing all done, Time for the Analisis"
bash Analisis.sh




