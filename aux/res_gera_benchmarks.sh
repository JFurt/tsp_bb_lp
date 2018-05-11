#!/bin/bash

GEN_NUMBER=10
INCR=1
make completo
rm -f benchmark/*
for (( i=1;$i<=$#;i=$i+1 ))
do
   echo ${!i}  #imprime a discretizacao sendo executada

   let pi=$INCR
   while [ $pi -le $GEN_NUMBER ]
   do
      ./completo benchmark/100_R${!i}_S${pi} ${pi}$ 100 ${!i}$
      let pi=$pi+$INCR
   done
done


