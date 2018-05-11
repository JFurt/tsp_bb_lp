#!/bin/bash

MEAN_TIMES=10
INCR=1
rm -f resultados/*_tempo.dat
for (( i=1;$i<=$#;i=$i+1 ))
do
   echo ${!i}

   let pi=$INCR
   while [ $pi -le $MEAN_TIMES ]
   do
      ./main benchmark/${!i}_R1_S${pi} FIFO
      ./main benchmark/${!i}_R1_S${pi} PILHA
      let pi=$pi+$INCR
   done
   #./time_mean ${!i}
done


