# Autores: Jorge Jaime y Javier Julve
# NIP:     845067              840710
# Definición de macros
CC = g++
CPPFLAGS = -std=c++11      #opciones de compilación
LDFLAGS =                      #opciones de linkado
RM = rm -f                     #comando para borrar ficheros

all: main
#-----------------------------------------------------------
# Compilacion
mainGSenku.o: mainGSenku.cpp
	${CC} mainGSenku.cpp -c ${CPPFLAGS}
GSenku.o: GSenku.cpp GSenku.hpp
	${CC} GSenku.cpp -c ${CPPFLAGS}
# Linkeado
main: GSenku.o mainGSenku.o
	${CC} mainGSenku.o GSenku.o -o mainGSenku ${CPPFLAGS}
#-----------------------------------------------------------	
# LIMPIEZA
clean:
	${RM} GSenku.o mainGSenku.o mainGSenku
