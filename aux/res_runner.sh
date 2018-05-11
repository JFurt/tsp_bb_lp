#!/bin/bash

LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

make clean && make

if [ ! -d "resultados" ]
then
	mkdir resultados
fi

./aux/res_timer.sh 1 2 4 8 16
