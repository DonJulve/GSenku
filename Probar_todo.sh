#!/bin/bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mv "${DIR}/Movimientos"/movimientos* "${DIR}/../GSenku"
mv "${DIR}/Tableros"/tablero* "${DIR}/../GSenku"
mv "${DIR}/Soluciones"/solucion* "${DIR}/../GSenku"
rm -r Movimientos
rm -r Tableros
rm -r Soluciones