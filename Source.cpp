﻿#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <limits>
#include <iomanip>
#include <iostream>
#include <string>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING 
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#ifdef _WIN32
#include <windows.h>
#undef max 
#endif
using namespace std;

const int NUM_JUGADORES = 4;
const int NUM_FICHAS = 4;
const int NUM_CASILLAS = 68;

enum tColor {	// Enumeracion de colores
    Amarillo,
    Azul,
    Rojo,
    Verde,
    Gris,
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
typedef tJugador tJugadores[NUM_JUGADORES]; //Array de 4 jugadores
typedef tCasilla tCasillas[NUM_CASILLAS]; //Array de 2x68 tCasilla
struct tJuego { 
	tCasillas casillas;
	tJugadores jugadores;
	tColor jugadorTurno;
	int tiradaActual;
	int premio;
	int seisesConsecutivos;
	int ultimaFichaMovida;
};


//FUNCIONES V1
bool esSeguro(int casilla);
int salidaJugador(int jugador);
int zanataJugador(int jugador);
string colorACadena(tColor color);
char colorALetra(tColor color);

//FUNCIONES V2
void iniciar(tJuego& juego);
bool puente(tJuego& juego, int i);
int cuantasEn(tJuego& juego, int casilla);
int primeraEn(tJuego& juego, int casilla);
int segundaEn(tJuego& juego, int casilla);
void saleFicha(tJuego& juego);
void aCasita(tJuego& juego, int casilla);
void abrirPuente(tJuego& juego);
bool procesa5(tJuego& juego);
bool procesa6(tJuego& juego);
bool jugar(tJuego& juego);
bool puedeMover(tJuego& juego);
void mover(tJuego& juego);
void setColor(tColor color);
void iniciaColores();
void pausa();

//FUNCIONES V3
void mostrar(const tJuego& juego);
void modoDepuracion();

//OPCIONAL: DESARROLLAR IA
int puedeComer(tJuego& juego, int& casilla);
int aMeta(tJuego& juego, int& casilla);
int aSeguro(tJuego& juego, int& casilla);
int huir(tJuego& juego, int& casilla);
int primeraPosible(tJuego& juego, int& casilla);


int main() {

    int casilla;
	tJuego juego;


}

//FUNCIONES V1
bool esSeguro(int casilla) 
{
    switch (casilla) {

    case 0: return true;
    case 5: return true;
    case 12: return true;
    case 17: return true;
    case 22: return true;
    case 29: return true;
    case 34: return true;
    case 39: return true;
    case 46: return true;
    case 51: return true;
    case 56: return true;
    case 63: return true;

    default: return false;
    }
}
int salidaJugador(int jugador) 
{
    switch (jugador) {

    case Amarillo: return 5;
    case Azul: return 22;
    case Rojo: return 39;
    case Verde: return 56;

    }
}
int zanataJugador(int jugador) 
{
    switch (jugador) {

    case Amarillo: return 0;
    case Azul: return 17;
    case Rojo: return 34;
    case Verde: return 51;

    }
}
string colorACadena(tColor color) 
{
    switch (color) {

    case Rojo: return "Rojo";
    case Verde: return "Verde";
    case Amarillo: return "Amarillo";
    case Azul: return "Azul";

    }
}
char colorALetra(tColor color) {}

//FUNCIONES V2
void iniciar(tJuego& juego) {}
bool puente(tJuego& juego, int i) 
{   
    if (juego.casillas[i].calle1 == juego.casillas[i].calle2){return true;}
    else { return false; }

}
int cuantasEn(tJuego& juego, int casilla) 
{
    int cuantas = 0;
    
    if (juego.casillas[casilla].calle1 != Ninguno;) { cuantas++; }
    else if (juego.casillas[casilla].calle2 != Ninguno;) { cuantas = 2; }

    return cuantas;
}
int primeraEn(tJuego& juego, int casilla)
{
    int primera = -1;

    for (int i = 0; i < NUM_FICHAS; i++)
    {
        if (juego.jugadorTurno.fichas[i] == casilla) { primera = i; break; }
 
    }
    return primera;
}
int segundaEn(tJuego& juego, int casilla) 
{
    int segunda = -1;

    for (int i = 4; i > 0; i--)
    {
        if (juego.jugadorTurno.fichas[i] == casilla) { segunda = i; break; }
    }
    return segunda;
}
void saleFicha(tJuego& juego) 
{
    int zanata = zanataJugador(juego.jugadorTurno);
    
    if (juego.casillas[zanata].calle1 == Ninguno)
    {
        juego.casillas[zanata].calle2 = Ninguno;
    }
    else if (juego.casillas[zanata].calle1 != Ninguno)
    {
        juego.casillas[zanata].calle2 = juego.casillas[zanata].calle1;
    }

    juego.jugadorTurno = juego.casillas[zanata].calle1;
}
void aCasita(tJuego& juego, int casilla) 
{
    int ficha = segundaEn(juego.jugadores[juego.casillas[casilla].calle1], casilla);

    juego.jugadores[juego.casillas[casilla].calle1].fichas[ficha] = -1;

    juego.casillas[casilla].calle2 = Ninguno;

}
void abrirPuente(tJuego& juego){}
bool procesa5(tJuego& juego) {}
bool procesa6(tJuego& juego) {}
bool jugar(tJuego& juego) {}
bool puedeMover(tJuego& juego) {}
void mover(tJuego& juego) {}
void setColor(tColor color) {
    switch (color) {
    case Azul:
        cout << "\x1b[34;107m";
        break;
    case Verde:
        cout << "\x1b[32;107m";
        break;
    case Rojo:
        cout << "\x1b[31;107m";
        break;
    case Amarillo:
        cout << "\x1b[33;107m";
        break;
    case Gris:
    case Ninguno:
        cout << "\x1b[90;107m";
        break;
    }
}
void pausa() {
    cout << "Pulsa Intro para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void iniciaColores() {
#ifdef _WIN32
    for (DWORD stream : {STD_OUTPUT_HANDLE, STD_ERROR_HANDLE}) {
        DWORD mode;
        HANDLE handle = GetStdHandle(stream);

        if (GetConsoleMode(handle, &mode)) {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(handle, mode);
        }
    }
#endif
}

//FUNCIONES V3
void mostrar(const tJuego& juego) {
   int casilla, ficha;
   tColor jug;

   cout << "\x1b[2J\x1b[H"; // Se situa en la esquina superior izquierda
   setColor(Gris);
   cout << endl;

   // Filas con la numeraci�n de las casillas...
   for (int i = 0; i < NUM_CASILLAS; i++)
      cout << i / 10;
   cout << endl;
   for (int i = 0; i < NUM_CASILLAS; i++)
      cout << i % 10;
   cout << endl;

   // Borde superior...
   for (int i = 0; i < NUM_CASILLAS; i++)
      cout << '>';
   cout << endl;

   // Primera fila de posiciones de fichas...
   for (int i = 0; i < NUM_CASILLAS; i++) {
      setColor(juego.casillas[i].calle2);
      if (juego.casillas[i].calle2 != Ninguno)
         cout << primeraEn(juego.jugadores[juego.casillas[i].calle2].fichas, i) + 1;
      else
         cout << ' ';
      setColor(Gris);
   }
   cout << endl;

   // "Mediana"   
   for (int i = 0; i < NUM_CASILLAS; i++)
      if (esSeguro(i))
         cout << 'o';
      else
         cout << '-';
   cout << endl;


   // Segunda fila de posiciones de fichas...
   for (int i = 0; i < NUM_CASILLAS; i++) {
      setColor(juego.casillas[i].calle1);
      if (juego.casillas[i].calle1 != Ninguno)
         cout << segundaEn(juego.jugadores[juego.casillas[i].calle1].fichas, i) + 1;
      else
         cout << ' ';
      setColor(Gris);
   }
   cout << endl;

   jug = Amarillo;
   // Borde inferior...
   for (int i = 0; i < NUM_CASILLAS; i++)
      if (i == zanataJugador(jug)) {
         setColor(jug);
         cout << "V";
         setColor(Gris);
      }
      else if (i == salidaJugador(jug)) {
         setColor(jug);
         cout << "^";
         setColor(Gris);
         jug = tColor(int(jug) + 1);
      }
      else
         cout << '>';
   cout << endl;

   // Metas y casas...
   for (int i = 0; i < NUM_FICHAS; i++) {
      casilla = 0;
      jug = Amarillo;
      setColor(jug);
      while (casilla < NUM_CASILLAS) {
         if (casilla == zanataJugador(jug)) {
            ficha = primeraEn(juego.jugadores[jug].fichas, 101 + i);
            if (ficha != -1) {
               cout << ficha + 1;
               if (cuantasEn(juego.jugadores[jug].fichas, 101 + i) > 1) {
                  ficha = segundaEn(juego.jugadores[jug].fichas, 101 + i);
                  if (ficha != -1) {
                     cout << ficha + 1;
                  }
                  else
                     cout << "V";
               }
               else
                  cout << "V";
            }
            else
               cout << "VV";
            casilla++;
         }
         else if (casilla == salidaJugador(jug)) {
            if (juego.jugadores[jug].fichas[i] == -1) // En casa
               cout << i + 1;
            else
               cout << "^";
            jug = tColor(int(jug) + 1);
            setColor(jug);
         }
         else
            cout << ' ';
         casilla++;
      }
      cout << endl;
   }

   // Resto de metas...
   for (int i = 105; i <= 107; i++) {
      casilla = 0;
      jug = Amarillo;
      setColor(jug);
      while (casilla < NUM_CASILLAS) {
         if (casilla == zanataJugador(jug)) {
            ficha = primeraEn(juego.jugadores[jug].fichas, i);
            if (ficha != -1) {
               cout << ficha + 1;
               if (cuantasEn(juego.jugadores[jug].fichas, i) > 1) {
                  ficha = segundaEn(juego.jugadores[jug].fichas, i);
                  if (ficha != -1) {
                     cout << ficha + 1;
                  }
                  else
                     cout << "V";
               }
               else
                  cout << "V";
            }
            else
               cout << "VV";
            casilla++;
            jug = tColor(int(jug) + 1);
            setColor(jug);
         }
         else
            cout << ' ';
         casilla++;
      }
      cout << endl;
   }

   casilla = 0;
   jug = Amarillo;
   setColor(jug);
   while (casilla < NUM_CASILLAS) {
      cout << ((juego.jugadores[jug].fichas[0] == 108) ? '1' : '.');
      cout << ((juego.jugadores[jug].fichas[1] == 108) ? '2' : '.');
      jug = tColor(int(jug) + 1);
      setColor(jug);
      cout << "               ";
      casilla += 17;
   }
   cout << endl;
   casilla = 0;
   jug = Amarillo;
   setColor(jug);
   while (casilla < NUM_CASILLAS) {
      cout << ((juego.jugadores[jug].fichas[2] == 108) ? '3' : '.');
      cout << ((juego.jugadores[jug].fichas[2] == 108) ? '4' : '.');
      jug = tColor(int(jug) + 1);
      setColor(jug);
      cout << "               ";
      casilla += 17;
   }
   cout << endl << endl;
   setColor(Gris);
}
void modoDepuracion() {}

//OPCIONAL: DESARROLLAR IA
int puedeComer(tJuego& juego, int& casilla) {}
int aMeta(tJuego& juego, int& casilla) {}
int aSeguro(tJuego& juego, int& casilla) {}
int huir(tJuego& juego, int& casilla) {}
int primeraPosible(tJuego& juego, int& casilla) {}