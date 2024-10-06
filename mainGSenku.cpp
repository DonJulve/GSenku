// Autores: Jorge Jaime y Javier Julve
// NIP:     845067              840710
#include <iostream>
#include <unistd.h>
#include <string>
#include "GSenku.hpp"

using namespace std;

int main(int argc, char* argv[]){
    // Variables usadas
    string FicheroTablero;
    string FicheroMovimientos;
    int retardo;
    // El fichero solución en caso de no recibir el nombre deseado por terminal sera "resultado.txt"
    string FicheroSolucion = "resultado.txt";
    tpTablero tablero;
    tpMovimientosValidos movimientos;
    tpListaMovimientos solucion;
    ofstream f;
    // Número válido de argumentos 4 o 5 depende si se incluye el nombre del fichero solución
    if (argc < 4 || argc > 5){
        cerr << "Numero de parametros invalido" << endl;
        return -1;
    }
    // Damos valor a las variables según los datos introducidos por terminal
    introducirDatos(argc, argv, FicheroTablero, FicheroMovimientos, retardo, FicheroSolucion);
    // Si conseguimos leer correctamente el fichero del tablero y el de los movimientos válidos buscamos una solución
    if(inicializarTablero(FicheroTablero, tablero) && inicializarMovimientosValidos (FicheroMovimientos, movimientos)){
        //Si encontramos solución
        if ((buscaSolucion(tablero, movimientos, solucion ,retardo)) == 0){
            // Solo si el retardo es positivo mostraremos el tablero final
            if (retardo > 0){
                mostrarTablero(tablero);
            }
            // En cualquier caso creamos el fichero de movimientos para llegar al resultado final
            escribeListaMovimientos(FicheroSolucion, solucion);
            return 0;
        }
        // Si no encontramos solución
        else{
            // Se abre el fichero solución y se escribe "-1"
            f.open(FicheroSolucion.c_str());
            if(f.is_open()){
                f << "-1" << endl;
                f.close();
            }
            // Si no se puede abrir el fichero se muestra un error
            else{
                cerr << "No se ha podido abrir el fichero de salida" << endl;
                return -1;
            }
            // Mostramos por pantalla que no se ha encontrado una solución
            cerr << "No se ha encontrado solucion" << endl;
            return -1;
        }
    }
    return 0;
}