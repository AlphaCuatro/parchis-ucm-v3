#include <iostream>
using namespace std;
const int NUM_JUGADORES = 4;
const int NUM_FICHAS = 4;
const int NUM_CASILLAS = 68;

enum tColor {	//enumeracion de colores
	Amarillo,
	Azul,
	Rojo,
	Verde,
	Ninguno
};
struct tCasilla {	// Casilla compuesta por dos mitades tColor
	tColor calle1;
	tColor calle2;
};
struct tFichas {	// Array 4 enteros, siendo cada uno el lugar donde esta la ficha
	int numFicha[4];
};
struct tJugador {	// Color jugador y array tipo tFichas
	tColor color;
	tFichas fichas;
};
typedef tJugador tJugadores[NUM_JUGADORES]; //Array de 4 jugadores, ni idea de por qué
typedef tCasilla tCasillas[NUM_CASILLAS]; //Array de 2x68 tCasilla, de nuevo ni idea de por qué no es una matriz bidimensional, pero bueno, enunciado de mierda.
struct tJuego { //Toda esta mierda de codigo mal estructurado puesta en la misma struct
	tCasillas casillas;
	tJugadores jugadores;
	tColor jugadorTurno;
	int tiradaActual;
	int premio;
	int seisesConsecutivos;
	int ultimaFichaMovida;
};



void mostrar(const tJuego& juego);
void modoDepuracion();

//OPCIONAL: DESARROLLAR IA
int puedeComer(tJuego& juego, int& casilla);
int aMeta(tJuego& juego, int& casilla);
int aSeguro(tJuego& juego, int& casilla);
int huir(tJuego& juego, int& casilla);
int primeraPosible(tJuego& juego, int& casilla);

int main() {

	tJugadores jugador;


}

//OPCIONAL: DESARROLLAR IA
int puedeComer(tJuego& juego, int& casilla){}
int aMeta(tJuego& juego, int& casilla){}
int aSeguro(tJuego& juego, int& casilla){}
int huir(tJuego& juego, int& casilla){}
int primeraPosible(tJuego& juego, int& casilla){}