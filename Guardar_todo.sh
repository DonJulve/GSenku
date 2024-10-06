#!/bin/bash
if [ ! -d Movimientos ]; then
    mkdir Movimientos
fi
mv movimientos* Movimientos

if [ ! -d Tableros ]; then
    mkdir Tableros
fi
mv tablero* Tableros

if [ ! -d Soluciones ]; then
    mkdir Soluciones
fi
mv solucion* Soluciones

if [ -f *.txt ]
then
    rm *.txt
fi