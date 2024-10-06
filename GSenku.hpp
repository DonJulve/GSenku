// Autores: Jorge Jaime y Javier Julve
// NIP:     845067              840710
#include <iostream>
#include <fstream> 

using namespace std;

// CONSTANTES Y TIPOS

enum tpEstadoCelda {
    NO_USADA, 
    VACIA, 
    OCUPADA
}; 

enum tpDireccion {
    superiorIzquierda=0, 
    superior, 
    superiorDerecha, 
    izquierda, 
    derecha, 
    inferiorIzquierda, 
    inferior, 
    inferiorDerecha
}; 

// Máximo múmero de filas y columnas del tablero
const int MAXDIM = 25;

// Tipo tablero
struct tpTablero{
    int nfils; // Número de filas del tablero
    int ncols; // Número de columnas del tablero
    tpEstadoCelda matriz[MAXDIM][MAXDIM];   // Una celda puede estar vacía, ocupada o no formar parte 
                                            // del tablero
                                            // Las coordenadas de la casilla de arriba a la izquierda son (0,0)
};

// Tipo posición
struct tpPosicion {     // una posición dentro del tablero
    int x; 
    int y; 
};

// Tipo movimientoPieza 
struct tpMovimientoPieza {   // especificamos un movimiento con la celda origen la celda destino del mismo
    tpPosicion origen; 
    tpPosicion destino; 
};

struct tpListaPosiciones {
    tpPosicion posiciones[MAXDIM*MAXDIM]; 
    int numPos = 0; 
}; 

// Tipo tpListaMovimientos 
// Nota: esta lista es para los movimientos de la solución 
//      (dependiendo de cómo abordéis el diseño, puede ser necesaria
//      una lista de mayor tamaño)
struct tpListaMovimientos {
    tpMovimientoPieza movs[MAXDIM*MAXDIM]; // como cada movimiento elimina una pieza, el número de movimientos de la solución está acotado
    int numMovs = 0; 
};

struct tpMovimientosValidos {
    // cada valor de tpDireccion se corresponderá con una posición de este vector
    bool validos[8]; 
};


// Códigos numéricos de colores - auxiliares para utilizar al mostrar el tablero por pantalla
const int NEGRO    = 40;
const int ROJO     = 41;
const int VERDE    = 42;
const int AMARILLO = 43;
const int AZUL     = 44;
const int MAGENTA  = 45;
const int CIAN     = 46;
const int BLANCO   = 47;


// FUNCIONES / PROCEDIMIENTOS 

// Pre: true
// Post: lee la configuración y el estado del tablero del fichero de configuración que se le pasa como argumento
//      inicializando tablero y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarTablero(const string nombreFichero, tpTablero &tablero);

// Pre: true 
// Post: lee los movimientos válidos del fichero que se le pasa como argumento 
//      inicializando la estructura y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarMovimientosValidos (const string nombreFichero, tpMovimientosValidos &movimientos);

// Pre: tablero contiene el estado actual de la ejecución de la búsqueda de la solución
// Post: Se ha mostrado el tablero por pantalla
void mostrarTablero(const tpTablero & tablero);

// Pre: tablero contiene el estado inicial del que se parte para la búsqueda, 
//      movValidos contiene los movimientos que están permitidos, 
//      solucion contiene la solución actual como lista de movimientos,
//      retardo es un entero que indica el retardo en milisegundos del que se va a mostrar una tablero de otro
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucion, const int retardo=0);


// Pre: listaMovimientos contiene la lista de movimientos con la solucion 
// Post: escribe la lista de movimientos en el fichero que se le pasa como argumento siguiendo el 
//      formato especificado en el guión (si está vacía, se escribe un -1 en el fichero)
void escribeListaMovimientos (string nombreFichero, const tpListaMovimientos &solucion); 

//Pre: true
//Post: Realiza el movimiento de una bola desplazándola y eliminando sobre la que se ha saltado.
bool realizarMovimiento(tpTablero& tablero, tpPosicion& pos_pieza, tpPosicion& pos_aux, tpPosicion& pos_intermedia, tpListaMovimientos& solucion);

//Pre: true
//Post: Deshace un movimiento hecho previamente devolviendo la bala a su lugar y volviendo a colocar la reciñen eliminada
bool deshacerMovimiento(tpTablero& tablero, tpPosicion& pos_pieza, tpPosicion& pos_aux, tpPosicion& pos_intermedia, tpListaMovimientos& solucion);

//Pre: Retardo > 0
//Post: Muestra el tablero y despues hace una pausa de "retardo" milisegundos
void mostrarTableroDelay(const tpTablero &tablero, const int retardo);

//Pre: true
//Post: Lee de la terminal los parámetros introducidos por el usuario para la ejecución del programa
void introducirDatos(int argc, char* argv[], string& FicheroTablero, string& FicheroMovimientos, int& retardo, string& FicheroSolucion);

//Pre: true
//Post: Devuelve true en caso de que una bola se pueda se pueda mover a su posición destino dependiendo del indice de su movimiento a realizar,
// además en ese caso devuelve en la estructura destino sus coordenadas en caso de que esta se mueva. 
bool calcularPosicion(tpTablero tablero, tpPosicion origen, tpPosicion& destino, const int indice);

// Pre: true
// Post: Devuelve true si el tablero es solución, es decir, que en él solo queda una ficha
bool EsSolucion (tpTablero tablero);

//////////////////////////////////////////////////////////////////////////////
// Salida con atributos
//
// La cadena "<ESC>[{attr1};...;{attrn}m" sirve para asignar atributos a la salida
//
// Por ejemplo: 
//    cout << "\033[34;42m" << "Hola" << "\033[0m";
// escribe en azul con fondo verde, y después resetea los atributos

// Lista de atributos estándar:
// 0 Resetea todos los atributos
// 1 brillante
// 2 tenue
// 4 subrayado
// 5 parpadeo
// 7 invertido
// 8 oculto

// Colores de primer plano
// 30 negro
// 31 rojo
// 32 verde
// 33 amarillo
// 34 azul
// 35 magenta
// 36 cian
// 37 blanco

// Colores de fondo
// 40 negro
// 41 rojo
// 42 verde
// 43 amarillo
// 44 azul
// 45 magenta
// 46 cian
// 47 blanco
