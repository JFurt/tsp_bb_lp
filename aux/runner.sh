#!/bin/bash

LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

make clean && make

if [ ! -d "resultados" ]
then
	mkdir resultados
fi

./aux/timer.sh 25 50 75 100 125
