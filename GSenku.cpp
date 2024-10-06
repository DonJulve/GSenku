// Autores: Jorge Jaime y Javier Julve
// NIP:     845067              840710
#include "GSenku.hpp"
#include <time.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

// Pre: true
// Post: lee la configuración y el estado del tablero del fichero de configuración que se le pasa como argumento
//      inicializando tablero y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarTablero(const string nombreFichero, tpTablero &tablero){
	ifstream f;
    // Abrimos el fichero que representa la estructura del tablero
    f.open(nombreFichero.c_str());
	if(f.is_open()){
        // Leemos las dimensiones del tablero
        f >> tablero.nfils >> tablero.ncols;
        // Si las dimensiones son mayores que MAXDIM devolvemos falso
		if(tablero.nfils > MAXDIM || tablero.ncols > MAXDIM){
			cerr << "Las dimensiones del tablero se han superado" << endl;
			f.close();
			return false;
		}
		else{
            string linea;
            //Quitamos la linea de las dimensiones
			getline(f,linea);
            for(int i = 0; i < tablero.nfils; i++){
                //Leemos la nueva linea con la posicion de las fichas
                getline(f,linea);
                int indice = 0;
                for(int j = 0; j < tablero.ncols; j++){
                    // '-' representa una casilla NO_USADA
                    if(linea[indice] == '-'){
					    tablero.matriz[i][j] = NO_USADA;
				    }
                    // 'o' representa una casilla OCUPADA
				    else if(linea[indice] == 'o'){
					    tablero.matriz[i][j] = OCUPADA;
				    }
                    // 'x' representa una casilla VACIA
				    else if(linea[indice] == 'x'){
				    	tablero.matriz[i][j] = VACIA;
				    }
                    // En caso de caracter vacio, aumentaremos el indice al final avanzando de la linea pero restandole 1 a j para que el bucle
                    // no avance y se quede en la misma columna
                    else if(linea[indice] == ' '){
                        j--;
                    }
                    else {
                        //En caso de caracteres incorrectos abortaremos el programa
                        cerr << "Las posiciones de las fichas no son validas" << endl;
                        f.close();
                        return false;
                    }
                    //Incremento el indice para ver el siguiente caracter
                    indice++;
                }
            }
		}
		f.close();
		return true;
	}
	else{
		cerr << "No se pudo abrir el fichero del tablero" << endl;
		return false;
	}
}

// Pre: true 
// Post: lee los movimientos válidos del fichero que se le pasa como argumento 
//      inicializando la estructura y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarMovimientosValidos (const string nombreFichero, tpMovimientosValidos &movimientos){
    ifstream f;
    // Abrimos el fichero que contiene los movimientos válidos
    f.open(nombreFichero.c_str());
    if(f.is_open()){
        string linea;
        int pieza = 0;
        bool esta_o = false;
        //Los dos bucles son para recorrer el archivo como matriz de 3x3, ignarndo el resto de caracteres que se pasen del limite
        for(int i = 0; i < 3;i++){
            // Leemos la primera linea del fichero
            getline(f,linea);
            int indice = 0;
            for(int j = 0; j < 3;j++){
                //Con esto comprobaremos que cuando se llegue a la pieza 5 (la o central) se vera si esta y en el caso que no este, 
                //o cualquier otra pieza la sustituya de las de - o +, el programa abortara la ejecucion para evitar problemas.  
                if(!esta_o && pieza==5){
                    cerr << "La configuracion de los movimientos no es valida" << endl;
                    f.close();
                    return false;
                }
                // Si leemos '-' el movimiento no es valido
           	    if(linea[indice] == '-'){
			    	movimientos.validos[pieza] = false;
                    //Como metemos pieza, avanzamos en el vector
                    pieza++;
			    }
                // Si leemos '+' el movimiento es valido
			    else if(linea[indice] == '+'){
				    movimientos.validos[pieza] = true;
                    //Como metemos pieza, avanzamos en el vector
                    pieza++;
			    }
                // Si leemos 'o' significa que la o central esta y podemos despreocuparnos de este error
                else if(linea[indice] == 'o'){
                    esta_o = true;
                }
                // En caso de caracter vacio, aumentaremos el indice al final avanzando de la linea pero restandole 1 a j para que el bucle
                // no avance y se quede en la misma columna
                else if(linea[indice] == ' '){
                    j--;
                }
                else{
                    //Si cualquier caracter es erroneo, abortamos mision
                    cerr << "La configuracion de los movimientos no es valida" << endl;
                    f.close();
                    return false;
                }
                indice++;
            }
        }
        f.close();
        return true;
    }
    cout << "No se pudo abrir el fichero de los movimientos" << endl;
    return false;
}

// Pre: tablero contiene el estado actual de la ejecución de la búsqueda de la solución
// Post: Se ha mostrado el tablero por pantalla
void mostrarTablero(const tpTablero & tablero){
    //Iteramos filas
    for (int i = 0; i < tablero.nfils; i++){
        //Iteramos columnas
        for (int j = 0; j < tablero.ncols; j++){
            // Escribimos "-" en rojo con fondo negro
            if (tablero.matriz[i][j] == NO_USADA){
                cout << "\033[31;40m" << "-" << "\033[0m";
            }
            // Escribimos "o" en azul con fondo negro
            else if (tablero.matriz[i][j] == OCUPADA){
                cout << "\033[34;40m" << "o" << "\033[0m";
            }
            // Escribimos "x" en negro con fondo negro
            else if (tablero.matriz[i][j] == VACIA){
                cout << "\033[30;40m" << "x" << "\033[0m";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Pre: listaMovimientos contiene la lista de movimientos con la solucion 
// Post: escribe la lista de movimientos en el fichero que se le pasa como argumento siguiendo el 
//      formato especificado en el guión (si está vacía, se escribe un -1 en el fichero)
void escribeListaMovimientos (string nombreFichero, const tpListaMovimientos &solucion){
    ofstream f;
    // Abre el fichero solución
    f.open(nombreFichero.c_str());
    if (f.is_open()){
        for (int i = 0; i < solucion.numMovs; i++){
            // Escribimos en el fichero: Nºfila de origen,Nºcolumna de origen:Nºfila de destino,Nºcolumna de destino
            f << solucion.movs[i].origen.x << "," << solucion.movs[i].origen.y << ":" << solucion.movs[i].destino.x << "," << solucion.movs[i].destino.y << endl;
        }
        f.close();
    }
    else{
        cout << "No se pudo abrir el fichero" << endl;
    }
}

//Pre: true
//Post: Realiza el movimiento de una bola desplazándola y eliminando sobre la que se ha saltado.
bool realizarMovimiento(tpTablero &tablero, tpPosicion &pos_pieza, tpPosicion &pos_aux, tpPosicion &pos_intermedia, tpListaMovimientos &solucion) {
    // Comprobar que la posición de origen está ocupada y la posición de destino está vacía
    if (tablero.matriz[pos_pieza.x][pos_pieza.y] != OCUPADA || tablero.matriz[pos_aux.x][pos_aux.y] != VACIA) {
        return false;
    }
    // Calcular la posición intermedia entre la posición actual y la de destino
    pos_intermedia.x = (pos_pieza.x + pos_aux.x) / 2;
    pos_intermedia.y = (pos_pieza.y + pos_aux.y) / 2;

    // Comprobar que la posición intermedia está ocupada
    if (tablero.matriz[pos_intermedia.x][pos_intermedia.y] != OCUPADA) {
        return false;
    }

    // Actualizar la matriz del tablero para reflejar el movimiento
    tablero.matriz[pos_pieza.x][pos_pieza.y] = VACIA;
    tablero.matriz[pos_intermedia.x][pos_intermedia.y] = VACIA;
    tablero.matriz[pos_aux.x][pos_aux.y] = OCUPADA;
    solucion.movs[solucion.numMovs].destino.x = pos_aux.x;
    solucion.movs[solucion.numMovs].destino.y = pos_aux.y;
    solucion.movs[solucion.numMovs].origen.x = pos_pieza.x;
    solucion.movs[solucion.numMovs].origen.y = pos_pieza.y;
    solucion.numMovs++;

    return true;
}





//Pre: true
//Post: Deshace un movimiento hecho previamente devolviendo la bala a su lugar y volviendo a colocar la recien eliminada
bool deshacerMovimiento(tpTablero& tablero, tpPosicion& pos_pieza, tpPosicion& pos_aux, tpPosicion& pos_intermedia, tpListaMovimientos &solucion) {
    // Comprobar que la posición de origen está vacía y la posición de destino está ocupada
    if (tablero.matriz[pos_pieza.x][pos_pieza.y] != VACIA || tablero.matriz[pos_aux.x][pos_aux.y] != OCUPADA) {
        return false;    
    }
    
    // Comprobar que la posición intermedia está vacía
    if (tablero.matriz[pos_intermedia.x][pos_intermedia.y] != VACIA) {
        return false;
    }

    // Actualizar la matriz del tablero para reflejar el movimiento
    tablero.matriz[pos_pieza.x][pos_pieza.y] = OCUPADA;
    tablero.matriz[pos_intermedia.x][pos_intermedia.y] = OCUPADA;
    tablero.matriz[pos_aux.x][pos_aux.y] = VACIA;
    solucion.numMovs--;

    return true;
}

//Pre: Retardo > 0
//Post: Muestra el tablero y despues hace una pausa de "retardo" milisegundos
void mostrarTableroDelay(const tpTablero &tablero, const int retardo){
    // Si el retardo es positivo
    if(retardo > 0){
        // Mostramos tablero
        mostrarTablero(tablero);
        // Esperamos retardo milisegundos
        usleep(retardo*1000);
    }
}


//Pre: true
//Post: Lee de la terminal los parámetros introducidos por el usuario para la ejecución del programa
void introducirDatos(int argc, char* argv[], string& FicheroTablero, string& FicheroMovimientos, int& retardo, string& FicheroSolucion){
    // Leemos los datos de la terminal y se los asignamos a las variables
    FicheroTablero=argv[1];
    FicheroMovimientos=argv[2];
    retardo=atoi(argv[3]);
    // Si hay 5 argumentos en la terminal leeremos el nombre deseado para el fichero solución
    if (argc == 5){
        FicheroSolucion=argv[4];
    }
}

// Pre: tablero contiene el estado inicial del que se parte para la búsqueda, 
//      movValidos contiene los movimientos que están permitidos, 
//      solucion contiene la solución actual como lista de movimientos,
//      retardo es un entero que indica el retardo en milisegundos del que se va a mostrar una tablero de otro
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucion, const int retardo){
    int retorno = -1;
    if (EsSolucion(tablero)) {
        // Si solo queda una pieza, el tablero está resuelto
        cout << "Tablero solucionado"<< endl;
        return 0;
    }

    tpPosicion pos_pieza;
    tpPosicion pos_aux = {0,0};
    tpPosicion pos_intermedia = {0,0};

    mostrarTableroDelay(tablero, retardo);
    // Llamamos al tablero aun sin modificar
    
    // Recorremos todas las celdas del tablero buscando una pieza
    for (int i = 0; i < tablero.nfils; i++) {
        for (int j = 0; j < tablero.ncols; j++) {
            if (retorno==-1){
                if (tablero.matriz[i][j] == OCUPADA) {
                    pos_pieza.x = i; 
                    pos_pieza.y = j;
                    for (int k = 0; k < 8; k++) {
                        if (movValidos.validos[k]) {
                            // Si el movimiento es válido, lo realizamos y llamamos a backtracking
                            if(calcularPosicion(tablero, pos_pieza, pos_aux, k)){
                                if(realizarMovimiento(tablero, pos_pieza, pos_aux, pos_intermedia, solucion)){
                                    // Llamamos a backtracking con el tablero modificado
                                    retorno = (buscaSolucion(tablero, movValidos, solucion, retardo));
                                }
                            }
                            if (retorno==-1){
                                // mostrarTableroDelay(tablero,retardo);
                                // Si no encontramos solución, deshacemos el movimiento y volvemos a la situación anterior
                                if(deshacerMovimiento(tablero, pos_pieza, pos_aux, pos_intermedia, solucion)){
                                    mostrarTableroDelay(tablero,retardo);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Si llegamos aquí, no se ha encontrado ninguna solución
    return retorno;
}

//Pre: true
//Post: Devuelve true en caso de que una bola se pueda se pueda mover a su posición destino dependiendo del indice de su movimiento a realizar,
// además en ese caso devuelve en la estructura destino sus coordenadas en caso de que esta se mueva. 
bool calcularPosicion(tpTablero tablero, tpPosicion origen, tpPosicion& destino, const int indice){
    // Mover superior izquierda
    if (indice == superiorIzquierda && origen.x >= 2 && origen.y >= 2){
        destino.x=origen.x - 2;
        destino.y=origen.y - 2;
    }
    // Mover superior
    else if(indice == superior && origen.y >= 2){
        destino.x = origen.x;
        destino.y = origen.y - 2;
    }
    // Mover superior derecha
    else if(indice == superiorDerecha && origen.x < tablero.ncols-2  && origen.y >= 2){
        destino.x = origen.x + 2;
        destino.y = origen.y - 2;
    }
    //Mover izquierda
    else if(indice == izquierda && origen.x >= 2){
        destino.x = origen.x - 2;
        destino.y = origen.y;
    }
    // Mover derecha
    else if(indice == derecha && origen.x < tablero.ncols-2){
        destino.x = origen.x + 2;
        destino.y = origen.y;
    }
    // Mover inferior izquierda
    else if(indice == inferiorIzquierda && origen.x >=2 && origen.y < tablero.nfils-2){
        destino.x = origen.x - 2;
        destino.y = origen.y + 2;
    }
    // Mover inferior
    else if(indice == inferior && origen.y < tablero.nfils-2){
        destino.x = origen.x;
        destino.y = origen.y + 2;
    }
    // Mover inferior derecha
    else if(indice == inferiorDerecha && origen.x < tablero.ncols-2 && origen.y < tablero.nfils-2){
        destino.x = origen.x + 2;
        destino.y = origen.y + 2;
    }
    else {
        return false;
    }
    return true;
}

// Pre: true
// Post: Devuelve true si el tablero es solución, es decir, que en él solo queda una ficha
bool EsSolucion(tpTablero tablero) {
    // Contamos el número de fichas del Gsenku restantes
    int numFichas = 0;
    for (int i = 0; i < tablero.nfils; i++) {
        for (int j = 0; j < tablero.ncols; j++) {
            if (tablero.matriz[i][j] == OCUPADA) {
                numFichas++;
            }
        }
    }
    // Si solo queda una ficha del senku, el tablero está resuelto
    return numFichas == 1;
}