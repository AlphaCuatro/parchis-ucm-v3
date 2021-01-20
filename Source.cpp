#include <cctype>
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
    bool pasaTurno;
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
void abrirPuente(tJuego& juego, int casilla);
bool procesa5(tJuego& juego);
bool procesa6(tJuego& juego);
bool jugar(tJuego& juego);
bool puedeMover(tJuego& juego, int tirada);
void mover(tJuego& juego, int tirada);
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

    int casilla, tirada, i;
	tJuego juego;


}

//FUNCIONES V1
bool esSeguro(int casilla) 
{
    bool esSeguro = false;
    switch (casilla) {

    case 0: esSeguro = true; break;
    case 5: esSeguro = true; break;
    case 12: esSeguro = true; break;
    case 17: esSeguro = true; break;
    case 22: esSeguro = true; break;
    case 29: esSeguro = true; break;
    case 34: esSeguro = true; break;
    case 39: esSeguro = true; break;
    case 46: esSeguro = true; break;
    case 51: esSeguro = true; break;
    case 56: esSeguro = true; break;
    case 63: esSeguro = true; break;

    }
    
    return esSeguro;
}

int zanataJugador(tJuego& juego) 
{
    int zanata = -1;
    switch (juego.jugadorTurno) {

    case Amarillo: zanata = 0; break;
    case Azul: zanata = 17; break;
    case Rojo: zanata = 34; break;
    case Verde: zanata = 51; break;

    }
    return zanata;
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
    bool hayPuente = false;

    if (juego.casillas[i].calle1 == juego.casillas[i].calle2){hayPuente = true;}
    
    return hayPuente;

}
int cuantasEn(tJuego& juego, int casilla) 
{
    int cuantas = 0;
    if (juego.casillas[casilla].calle1 == Ninguno && juego.casillas[casilla].calle2 == Ninguno)
    {
        for (int i = 0; i < NUM_JUGADORES; i++)
        {
            if (juego.jugadores[juego.jugadorTurno].fichas[i] == casilla) { cuantas++; }
        }
    }
    else if (juego.casillas[casilla].calle1 != Ninguno) { cuantas = 1; }
    else if (juego.casillas[casilla].calle2 != Ninguno) { cuantas = 2; }

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
    int ficha = segundaEn(juego.jugadores[juego.casillas[casilla].calle2], casilla);
   
    juego.jugadores[juego.casillas[casilla].calle2].fichas[ficha] = -1;

    juego.casillas[casilla].calle2 = Ninguno;

}
void abrirPuente(tJuego& juego, int casilla){}
bool procesa5(tJuego& juego)
{   
    bool puedeAbrir = false;
    int zanata = zanataJugador(juego.jugadorTurno);
    
    if (juego.casillas[zanata].calle2 == Ninguno)
    {
        puedeAbrir = true; 
        saleFicha(juego.jugadores[juego.jugadorTurno]);
    }
    else if (juego.casillas[zanata].calle2 != Ninguno)
    {
        if (juego.casillas[zanata].calle1 ==  juego.jugadorTurno)
        {
            puedeAbrir = true;
            saleFicha(juego.jugadores[juego.jugadorTurno]);
        }
        else
        {
            aCasita(juego.jugadores[juego.casillas[zanata].calle2].fichas, zanata);
            juego.premio = 20;
            saleFicha(juego.jugadores[juego.jugadorTurno]);
            puedeAbrir = true;
        }
    }
    return puedeAbrir;
}
bool procesa6(tJuego& juego)
{
    bool procesa6 = false, hayPuente = false;
    int numPuentes = 0, puente1 = -2, puente2 = -2;
    //puente1 y puente2 estan inicializados a -2 por si se diese el caso de que un puente esta en la casilla 0
    juego.seisesConsecutivos++;
  
    if (juego.seisesConsecutivos == 3)
    {
        if (juego.jugadores[juego.jugadorTurno].fichas[juego.ultimaFichaMovida] > 100)
        {
            cout << "La ultima ficha movida esta en la subida a meta, se salva" << endl;
        }
        else if (juego.jugadores[juego.jugadorTurno].fichas[juego.ultimaFichaMovida] < 100)
        {
            aCasita(juego.jugadores[juego.jugadorTurno].fichas[juego.ultimaFichaMovida], juego.ultimaFichaMovida);
            cout << "Han salido 6 seises seguidos, la ultima ficha movida se va a casa" << endl;
           
        }
        procesa6 = true;
    }
    else
    {
        if (cuantasEn(juego.jugadores[juego.jugadorTurno].fichas, -1) == 0)
        {
            juego.tiradaActual = 7;
            cout << "Como no tiene fichas en casa, cuenta 7!" << endl;
        }

        for (int i = 0; i < NUM_CASILLAS; i++)
        {
            if (puente(juego.casillas[i], i))
            {
                numPuentes++;
                if (puente1 == -2) { puente1 = i; }
                else{puente2 == i; i = NUM_CASILLAS;}
            }
        }
        if (numPuentes != 0)
        {
            int puente = 0;
            if (numPuentes == 1) { abrirPuente(juego.jugadores[juego.jugadorTurno], puente1);}
            else
            {
                cout << "El jugador tiene dos puentes" << endl;
                cout << "[1] EL puente de la casilla " << puente1 << endl;
                cout << "[2] El puente de la casilla " << puente2 << endl;
                cout << "Escribe 1 para abrir el primer puente, escribe 2 para abrir el segundo puente : ";
                while (puente != 1 || puente != 2)
                {
                    cin >> puente;
                    cout << endl;
                    if (puente == 1) { abrirPuente(juego.jugadores[juego.jugadorTurno].fichas, puente1);}
                    else if (puente == 2) { abrirPuente(juego.jugadores[juego.jugadorTurno].fichas, puente2); }
                    else if(puente != 1 && puente != 2){cout << "Numero introducido incorrecto, escribe otro numero : "}

                }
                
            }
        }
        procesa6 = false;

    }
    juego.pasaTurno = procesa6;
    return procesa6;
}
bool jugar(tJuego& juego) {}
bool puedeMover(tJuego& juego, int tirada)
{
    bool puedeMover = false;
    int casillaDestino = casillaDestino + tirada;

    if(juego.jugadores[jugadorTurno].fichas){}



}
void mover(tJuego& juego, int tirada) {}
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