//  Arturo Soria y Joel Sánchez

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#undef max
#endif

using namespace std;

//  DECLARACIÓN DE CONSTANTES

const int NUM_FICHAS = 4;
const int  NUM_JUGADORES = 4;
const int NUM_CASILLAS = 68;

const bool modoDebug = false;
const string Archivo = "pruebas.txt";

//  DECLARACIÓN DE TIPOS Y ESTRUCTURAS

typedef int tFichas[NUM_FICHAS];
typedef int tCasillas[NUM_CASILLAS];
typedef int tJugadores[NUM_JUGADORES];
typedef enum {

    Amarillo, 
    Azul, 
    Rojo, 
    Verde, 
    Gris, 
    Ninguno 

} tColor;

struct tJugador {

    tColor color;
    tFichas fichas;
};
struct tCasilla {

    tColor calle1;
    tColor calle2;
};
struct tJuego {

    tCasilla casillas[NUM_CASILLAS];
    tJugador jugadores[NUM_JUGADORES];
    tColor turno = Ninguno;
    int tirada = 0;
    int premio = 0;
    int seises = 0;
    int ultimaFichaMovida = 0;
};

//  DECLARACIÓN DE FUNCIONES

int eligeColor();                                                                                           //
int zanataJugador(int jugadorfin);                                                                          //
void mostrar(const tJuego& juego);                                                                          //
void salidaacasa(int casilla, int& casilla2, int jugador, int jugador2, int& i);                            //
void jugar(tJuego& juego, bool& fin);                                                                       //
void saleFicha(tJuego& juego, int ficha);                                                                   //
void setColor(tColor color);                                                                                //
void iniciar(tJuego& juego);                                                                                //
void iniciaColores();                                                                                       //
void usarPremio(tJuego& juego);                                                                             //
void mover(tJuego& juego, int ficha, int casilla);                                                          //
void cargar(tJuego& juego);                                                                                 //
bool esFinJuego(tJuego juego);                                                                              //
bool esSeguro(int seguro);                                                                                  //
bool puedeMover(const tJuego juego, int ficha, int& casilla);                                               //
bool procesa5(tJuego& juego);                                                                               //
bool procesa6(tJuego& juego);                                                                               //
bool puente(const tCasillas calle1, const tCasillas calle2, int casilla);                                   //
int primeraEn(tFichas jugador, int casilla, const tJuego juego);                                            //
int segundaEn(const tFichas jugador, int casilla);                                                          //
int cuantasEn(const tFichas jugador, int casilla);                                                          //
int salidaJugador(int jugador);                                                                             //
int colorAJugador(tColor color);                                                                            //
int tirarDado(bool& fin);                                                                                   //
string colorACadena(tColor color);                                                                          //
char colorALetra(tColor color);                                                                             //

/*
    LAS FUNCIONES OPCIONALES DE LA
    IA DE LA V3 NO ESTÁN IMPLEMENTADAS
*/

//  IMPLEMENTACIÓN DE FUNCIONES

int main() {
    
    time(NULL);
    int firstplayer = rand() % 4;
    bool fin = false;
    tJuego juego;
    tColor jugador = tColor(eligeColor());
    juego.turno = tColor(firstplayer);
    iniciar(juego);
    while (!esFinJuego(juego) && !fin) {
        mostrar(juego);
        jugar(juego, fin);

        if (juego.premio != 0) {
            usarPremio(juego);
        }
        juego.turno = tColor(juego.turno + 1);
        if ((firstplayer + juego.turno) > 3) { juego.turno = tColor(firstplayer + juego.turno - 4); }
        firstplayer = 0;
        system("pause");
        system("CLS");

    }
    cout << "\n" << "FIN DEL JUEGO";
    return 0;
}
void cargar(tJuego& juego) {

    ifstream archivo;
    int jugador, casilla;
    int tiradas[100];

    archivo.open("archivo.txt");

    if (archivo.is_open()) {
        for (int i = 0; i < NUM_JUGADORES; i++)
            for (int f = 0; f < NUM_FICHAS; f++) {
                archivo >> casilla;
                juego.jugadores[i].fichas[f] = casilla;
                if ((casilla >= 0) && (casilla < NUM_CASILLAS))
                    if (juego.casillas[casilla].calle1 == Gris)
                        juego.casillas[casilla].calle1 = tColor(i);
                    else
                        juego.casillas[casilla].calle2 = tColor(i);
            }
        archivo >> jugador;
        juego.turno = tColor(jugador);
        bool breakk = false;
        for (int i = 0; !breakk; i++) {
            archivo >> tiradas[i];
            if (tiradas[i] == -1) { breakk = true; }
        }
        archivo.close();
    }
}
void usarPremio(tJuego& juego) {

    int tirada = juego.tirada;
    int a;
    bool b = false;

    juego.tirada = juego.premio;
    mostrar(juego);

    cout << "Has ganado un premio, cuenta " << juego.premio << " casillas con las siguientes fichas" << endl;
    cout << "Elige una opción: " << endl;
    for (int i = 0; i < NUM_FICHAS; i++) {
        if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
            cout << i + 1 << " ficha puede moverse " << juego.premio << " casillas!" << endl;
            b = true;
        }
    }
    if (b) {
        cin >> a;
        bool breakk = false;
        int error = 0;
        while (a > 4 || a < 1 || !breakk || cin.fail()) {
            if (error > 0) {
                cout << "Elige una:       ";
            }
            error++;
            if (!cin.good()) {
                cin.clear();
                cin.ignore(200, '\n');
            }
            for (int i = 0; i < NUM_FICHAS && !breakk; i++) {
                if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == a) {
                    breakk = true;
                }
            }
        }
        if (a > 0 && a < 5 && puedeMover(juego, a - 1, juego.jugadores[juego.turno].fichas[a - 1])) {
            mover(juego, a - 1, juego.jugadores[juego.turno].fichas[a - 1] + juego.premio);
        }
    }
    else { cout << "No tienes ninguna ficha con la que puedas utilizar el premio" << endl; }
    system("pause");
    juego.premio = 0;
    juego.tirada = tirada;
}
void mover(tJuego& juego, int ficha, int casilla) {

    int casillaN = casilla;
    if (juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2 == Ninguno && juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle1 == juego.turno) { //por si solo hay una ficha de el jugador en esa casilla
        juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle1 = Ninguno;
    }

    if (juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2 != Ninguno && juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2 != juego.turno && juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle1 == juego.turno) {//por si están dos fichas de diferentes colores en un seguro
        juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle1 = juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2;
    }
    juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2 = Ninguno;

    if (juego.jugadores[juego.turno].fichas[ficha] == zanataJugador(juego.turno)) {
        juego.jugadores[juego.turno].fichas[ficha] = 100 + juego.tirada;
        casillaN = juego.tirada + 100;
    }
    else {
        juego.jugadores[juego.turno].fichas[ficha] += juego.tirada;
        if (juego.jugadores[juego.turno].fichas[ficha] > 67 && juego.jugadores[juego.turno].fichas[ficha] < 101) {
            juego.jugadores[juego.turno].fichas[ficha] -= 68;
            casillaN -= 68;
        }
    }


    if (juego.casillas[casillaN].calle1 == Ninguno) {
        juego.casillas[casillaN].calle1 = juego.turno;
    }
    else if (juego.casillas[casillaN].calle1 != Ninguno && juego.casillas[casillaN].calle1 != juego.turno && juego.casillas[casillaN].calle2 == Ninguno && esSeguro(casillaN)) { //si es seguro y la casilla de la calle1 ya está ocupada
        juego.casillas[casillaN].calle2 = juego.turno;
    }
    else if (juego.casillas[casillaN].calle1 != Ninguno && juego.casillas[casillaN].calle1 == juego.turno && juego.casillas[casillaN].calle2 == Ninguno) { //si el propio jugador ya tiene una ficha en esa casilla
        juego.casillas[casillaN].calle2 = juego.turno;
    }
    else if (juego.casillas[casillaN].calle1 != Ninguno && juego.casillas[casillaN].calle1 != juego.turno && juego.casillas[casillaN].calle2 == Ninguno && !esSeguro(casillaN) && juego.jugadores[juego.turno].fichas[ficha] < 101) { //si el jugador se come a otra ficha
        for (int i = 0; i < NUM_FICHAS; i++) {
            if (juego.jugadores[juego.casillas[casillaN].calle1].fichas[i] == casillaN) {
                juego.jugadores[juego.casillas[casillaN].calle1].fichas[i] = -1;
                juego.casillas[casillaN].calle1 = juego.turno;
                juego.premio = 20;
            }
        }
    }
    juego.ultimaFichaMovida = juego.jugadores[juego.turno].fichas[ficha];
    if (juego.ultimaFichaMovida == 108) {
        juego.premio = 10;
    }
}
void saleFicha(tJuego& juego, int ficha) {

    juego.jugadores[juego.turno].fichas[ficha] = salidaJugador(juego.turno);
    if (juego.casillas[salidaJugador(juego.turno)].calle1 == Ninguno) {
        juego.casillas[salidaJugador(juego.turno)].calle1 = juego.turno;
    }
    else if (juego.casillas[salidaJugador(juego.turno)].calle2 == Ninguno) { juego.casillas[salidaJugador(juego.turno)].calle2 = juego.turno; }
    else if (juego.casillas[salidaJugador(juego.turno)].calle1 != juego.turno) { juego.casillas[salidaJugador(juego.turno)].calle1 = juego.turno; }
    else if (juego.casillas[salidaJugador(juego.turno)].calle2 != juego.turno) { juego.casillas[salidaJugador(juego.turno)].calle2 = juego.turno; }


}
void jugar(tJuego& juego, bool& fin) {

    juego.seises = 0;
    int f = 0;
    juego.tirada = 8;
    int siete = 0;
    int c = 0;
    for (int i = 0; i < NUM_FICHAS; i++) {
        if (juego.jugadores[tColor(juego.turno)].fichas[i] != -1) {
            c++;
        }
    }
    if (c == 4) { siete++; }
    while ((juego.tirada == 6 + siete || f == 0) && !fin && juego.seises != 3) {
        int opcion = 6;
        f++;
        cout << "Turno para jugador " << colorACadena(juego.turno) << "." << endl;
        juego.tirada = tirarDado(fin);
        if (juego.tirada == 6) {
            juego.tirada += siete;
        }
        cout << "DADO: " << juego.tirada << "." << endl;
        if (juego.tirada == 6 || juego.tirada == 7) { juego.seises++; }
        if (!procesa6(juego)) {
            if (juego.tirada == 7) { juego.tirada = 6; }
            int c = 0;
            bool b = false;
            int casillapasada = 0;
            switch (juego.tirada)
            {
            case 1:

                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;
                        c++;
                    }
                }
                if (c > 0) {
                    bool breakk = false;
                    int error = 0;
                    while (opcion > 4 || opcion < 1 || !breakk || cin.fail()) {
                        if (error > 0) {
                            cout << "Elige una:       ";
                        }
                        error++;
                        cout << "Elija una opcion: " << endl;
                        cin >> opcion;
                        if (!cin.good()) {
                            cin.clear();
                            cin.ignore(200, '\n');
                        }
                        for (int i = 0; i < NUM_FICHAS && !breakk; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                breakk = true;
                            }
                        }
                    }
                    c = 0;
                }
                break;

            case 2:

                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;
                        c++;
                    }
                }
                if (c > 0) {
                    bool breakk = false;
                    int error = 0;
                    while (opcion > 4 || opcion < 1 || !breakk) {
                        if (error > 0) {
                            cout << "Elige una:       ";
                        }
                        error++;
                        cout << "Elija una opcion: " << endl;
                        cin >> opcion;
                        if (!cin.good()) {
                            cin.clear();
                            cin.ignore(200, '\n');
                        }
                        for (int i = 0; i < NUM_FICHAS && !breakk; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                breakk = true;
                            }
                        }
                    }
                    c = 0;
                }
                break;
            case 3:

                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;
                        c++;
                    }
                }
                if (c > 0) {
                    bool breakk = false;
                    int error = 0;
                    while (opcion > 4 || opcion < 1 || !breakk) {
                        if (error > 0) {
                            cout << "Elige una:       ";
                        }
                        error++;
                        cout << "Elige una opcion: " << endl;
                        cin >> opcion;
                        if (!cin.good()) {
                            cin.clear();
                            cin.ignore(200, '\n');
                        }
                        for (int i = 0; i < NUM_FICHAS && !breakk; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                breakk = true;
                            }
                        }
                    }
                    c = 0;
                }
                break;
            case 4:

                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": ficha puede moverse " << juego.tirada << " casillas." << endl;
                        c++;
                    }
                }
                if (c > 0) {
                    bool breakk = false;
                    int error = 0;
                    while (opcion > 4 || opcion < 1 || !breakk) {
                        if (error > 0) {
                            cout << "Elija dentro de sus posibilidades.       ";
                        }
                        error++;
                        cout << "Elija la opcion que mas le guste: " << endl;
                        cin >> opcion;
                        if (!cin.good()) {
                            cin.clear();
                            cin.ignore(200, '\n');
                        }
                        for (int i = 0; i < NUM_FICHAS && !breakk; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                breakk = true;
                            }
                        }
                    }
                    c = 0;
                }
                break;
            case 5:

                for (int i = 0; i < NUM_FICHAS && !b; i++) {

                    if (juego.jugadores[juego.turno].fichas[i] == -1 && procesa5(juego)) {
                        saleFicha(juego, i);

                        b = true;
                        c++;
                    }
                }

                if (c == 0) {
                    for (int i = 0; i < NUM_FICHAS; i++) {
                        casillapasada = juego.jugadores[juego.turno].fichas[i];
                        if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                            cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;

                        }
                    }
                }


                if (c == 0) {
                    bool breakk = false;
                    int error = 0;
                    while (opcion > 4 || opcion < 1 || !breakk) {
                        if (error > 0) {
                            cout << "Elige una:       ";
                        }
                        error++;
                        cout << "Elige una opcion: " << endl;
                        cin >> opcion;
                        if (!cin.good()) {
                            cin.clear();
                            cin.ignore(200, '\n');
                        }
                        for (int i = 0; i < NUM_FICHAS && !breakk; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                breakk = true;
                            }
                        }
                    }
                }
                break;
            case 6:
                if (siete == 1 && juego.tirada != 7 && juego.tirada == 6) { juego.tirada += siete; }
                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;
                        c++;
                    }
                }
                if (c > 0) {
                    bool breakk = false;
                    int error = 0;
                    while (opcion > 4 || opcion < 1 || !breakk) {
                        if (error > 0) {
                            cout << "Elige una:       ";
                        }
                        error++;
                        cout << "Elige una opcion: " << endl;
                        cin >> opcion;
                        if (!cin.good()) {
                            cin.clear();
                            cin.ignore(200, '\n');
                        }
                        for (int i = 0; i < NUM_FICHAS && !breakk; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                breakk = true;
                            }
                        }
                    }
                    c = 0;
                }
                break;
            }
            if (opcion > 0 && opcion < 5 && puedeMover(juego, opcion - 1, juego.jugadores[juego.turno].fichas[opcion - 1])) {
                mover(juego, opcion - 1, juego.jugadores[juego.turno].fichas[opcion - 1] + juego.tirada);
            }
            if (opcion == 0) {
                fin = true;
            }
            if (juego.seises > 0 && juego.seises != 3 && juego.tirada == 6 + siete) {
                cout << "Has sacado un 6 , tiras de nuevo" << endl;
                system("pause");
                system("CLS");
                mostrar(juego);
            }
            if (juego.premio != 0) {
                usarPremio(juego);
                system("CLS");
                mostrar(juego);
            }
        }
        if (siete == 1 && juego.tirada != 7 && juego.tirada == 6) { juego.tirada += siete; }
    }
}
void iniciar(tJuego& juego) {

    for (int i = 0; i < NUM_CASILLAS; i++) {
        juego.casillas[i].calle1 = Gris;
        juego.casillas[i].calle2 = Gris;
    }

    srand(time(NULL));
    juego.turno = tColor(0 + rand() % (3));

    for (int j = 0; j < NUM_JUGADORES; j++) {
        for (int i = 0; i < NUM_FICHAS; i++) {
            juego.jugadores[j].fichas[i] = -1;
        }
    }

    for (int i = 0; i < NUM_CASILLAS; i++) {

        if (juego.casillas[i].calle1 == Gris) {

            juego.casillas[i].calle1 = Ninguno;
        }
        if (juego.casillas[i].calle2 == Gris) {

            juego.casillas[i].calle2 = Ninguno;
        }
    }
}
bool puente(const tJuego juego, int casilla) { 

    bool a = true;

    if (juego.casillas[casilla].calle1 != Ninguno && juego.casillas[casilla].calle2 == juego.casillas[casilla].calle1 && esSeguro(casilla)) {
        a = false;
    }

    return a;
}
bool procesa6(tJuego& juego) {

    bool a = false;
    int b = 0;
    int c = 0;
    int puente;
    int seguro[4];

    if (juego.seises == 3) {
        if (juego.ultimaFichaMovida < 68) {
            cout << "Has sacado 3 veces seguidas 6, la ultima ficha va a casa" << endl;
            bool breakk = true;
            for (int i = 0; i < NUM_FICHAS && breakk; i++) {
                if (juego.jugadores[juego.turno].fichas[i] == juego.ultimaFichaMovida) {
                    if (juego.casillas[juego.jugadores[juego.turno].fichas[i]].calle1 == juego.turno) {
                        juego.casillas[juego.jugadores[juego.turno].fichas[i]].calle1 = Ninguno;
                    }
                    else { juego.casillas[juego.jugadores[juego.turno].fichas[i]].calle2 = Ninguno; }
                    juego.jugadores[juego.turno].fichas[i] = -1;
                    breakk = false;
                }
            }
        }
        else { cout << "No vas a casa ya que la ultima ficha que ha movido se encontraba en la subida a meta" << endl; }
        a = true;
    }
    for (int i = 0; i < NUM_FICHAS; i++) { 
        if (esSeguro(juego.jugadores[juego.turno].fichas[i])) { seguro[b] = juego.jugadores[juego.turno].fichas[i]; b++; }
    }
    for (int j = 0; j < b; j++) { 
        for (int i = 0; i < b; i++) {
            if (seguro[j] == seguro[i] && j != i) {
                c++;
                puente = seguro[j];
            }
        }
    }
    if (c == 2) { 
        bool breakk2 = false; for (int i = 0; i < NUM_FICHAS && !breakk2; i++) {
            if (juego.jugadores[juego.turno].fichas[i] == puente && (juego.tirada == 6 || juego.tirada == 7)) {
                cout << "Abres un puente" << endl;
                system("pause");;
                int casillafutura = juego.jugadores[juego.turno].fichas[i] + juego.tirada;
                mover(juego, i, casillafutura);
                system("CLS");
                mostrar(juego);
                cout << "Has sacado un 6, tira otra vez" << endl;
                breakk2 = true;
                a = true;
            }
        }
    }

    return a;
}
bool procesa5(tJuego& juego) { 
    bool a = false;
    int c = 0;
    for (int i = 0; i < NUM_FICHAS; i++) {
        if (juego.jugadores[juego.turno].fichas[i] == salidaJugador(juego.turno)) {
            c++;
        }
    } 

    if (c != 2) { a = true; }
    if (a && juego.casillas[salidaJugador(juego.turno)].calle2 != juego.turno && juego.casillas[salidaJugador(juego.turno)].calle2 != Ninguno && juego.casillas[salidaJugador(juego.turno)].calle1 == juego.turno) {
        bool a = false;
        for (int i = 0; i < NUM_FICHAS && !a; i++) {
            if (juego.jugadores[juego.casillas[salidaJugador(juego.turno)].calle2].fichas[i] == salidaJugador(juego.turno)) {
                juego.jugadores[juego.casillas[salidaJugador(juego.turno)].calle2].fichas[i] = -1;
                juego.premio = 20;
                a = true;

            }
        }
    }
    if (a && juego.casillas[salidaJugador(juego.turno)].calle1 != juego.turno && juego.casillas[salidaJugador(juego.turno)].calle1 != Ninguno && juego.casillas[salidaJugador(juego.turno)].calle2 != juego.turno && juego.casillas[salidaJugador(juego.turno)].calle2 != Ninguno) {
        bool a = false;
        for (int i = 0; i < NUM_FICHAS && !a; i++) {
            if (juego.jugadores[juego.casillas[salidaJugador(juego.turno)].calle2].fichas[i] == salidaJugador(juego.turno)) {
                juego.jugadores[juego.casillas[salidaJugador(juego.turno)].calle2].fichas[i] = -1;
                juego.premio = 20;
                a = true;

            }
        }
    }
    return a;
}
bool puedeMover(const tJuego juego, int ficha, int& casilla) { 
    int premio = 0;

    int casillafin = casilla + juego.tirada;
    bool ok = true;
    if (casillafin > 67 && casillafin < 101) {
        casillafin -= 68;
    }
    if (juego.jugadores[juego.turno].fichas[ficha] == -1) {
        ok = false;
    }
    if (juego.jugadores[juego.turno].fichas[ficha] == zanataJugador(juego.turno)) {
        for (int i = 1; i <= juego.tirada && ok; i++) { 
            int casillareal = 100 + i;

            if (casillareal == 109) {
                ok = false;
            }
        }
    }
    if (juego.jugadores[juego.turno].fichas[ficha] > 100) {
        for (int i = 1; i <= juego.tirada && ok; i++) { 
            int casillareal = casilla + i;
            if (casillareal == 109) {
                ok = false;
            }
        }
    }
    for (int i = 1; i <= juego.tirada && ok; i++) { 
        int casillareal = casilla + i;
        if ((casillareal > 67)) { casillareal -= 68; }
        if (!puente(juego, casillareal)) {
            ok = false;
        }
    }
    if (juego.jugadores[juego.turno].fichas[ficha] != zanataJugador(juego.turno) && juego.jugadores[juego.turno].fichas[ficha] < 100) {
        for (int i = 1; i <= juego.tirada && ok; i++) { 
            int casillareal = casilla + i;
            if ((casillareal > 67)) { casillareal -= 68; }
            if (casillareal == zanataJugador(juego.turno) + 1) {
                ok = false;
            }
        }
    }
    if (juego.casillas[casillafin].calle1 != Ninguno && juego.casillas[casillafin].calle2 == juego.casillas[casillafin].calle1 && juego.jugadores[juego.turno].fichas[ficha] < 100) { //por si en la casilla que va a caer ya hay dos fichas
        ok = false;
    }
    return ok;
}
bool esFinJuego(const tJuego juego) {

    bool finJuego = false;
    int a = 0;

    for (int j = 0; j < NUM_JUGADORES && a != 4; j++) {
        a = 0;
        for (int i = 0; i < NUM_FICHAS; i++) {
            if (juego.jugadores[j].fichas[i] == 108) { a++; }
            if (a == 4) { finJuego = true; }
        }
    }
    return finJuego;
}
bool esSeguro(int s) {

    bool ok = false;

    switch (s) {

    case 0: ok = true; break;
    case 5: ok = true; break;
    case 12: ok = true; break;
    case 17: ok = true; break;
    case 22: ok = true; break;
    case 29: ok = true; break;
    case 34: ok = true; break;
    case 39: ok = true; break;
    case 46: ok = true; break;
    case 51: ok = true; break;
    case 56: ok = true; break;
    case 63: ok = true; break;

    default: ok = false; break;
    }

    return ok;
}
int cuantasEn(const tFichas jugador, int casilla) {
    int c = 0;
    for (int i = 0; i < NUM_FICHAS; i++) {
        if (jugador[i] == casilla) {
            c++;

        }
    }
    return c;
}
int segundaEn(const tFichas jugador, int casilla) {
    int s = -1;
    int c = 0;
    int c2 = 0;
    for (int i = 0; i < NUM_FICHAS; i++) {

        if (jugador[i] == casilla) {
            c++;
        }
    }

    if (c == 1) {
        for (int i = 0; i < NUM_FICHAS && s == -1; i++) {

            if (jugador[i] == casilla) {
                s = i;
            }
        }
    }
    else if (c == 2) {
        for (int i = 0; i < NUM_FICHAS && s == -1; i++) {

            if (jugador[i] == casilla) {
                c2++;
            }
            if (c2 == 2) {
                s = i;
            }
        }

    }
    return s;
}
int primeraEn(const tFichas jugador, int casilla, tJuego juego) {

    int f = -1;
    int c = 0;

    for (int i = 0; i < NUM_FICHAS; i++) {
        if (jugador[i] == casilla) {
            c++;
            if (casilla > 100) {
                f = i;
            }
        }
    }

    if (juego.casillas[casilla].calle2 != juego.casillas[casilla].calle1 && juego.casillas[casilla].calle2 != Ninguno) {
        c++;
    }
    if (c == 2) {
        for (int j = 0; j < NUM_FICHAS && f == -1; j++) {
            if (jugador[j] == casilla) {
                f = j;
            }
        }
    }
    return f;

}
int colorAJugador(tColor color) {
    int returned;
    switch (color) {
    case Amarillo:
        returned = 0;
        break;
    case Azul:
        returned = 1;
        break;
    case Rojo:
        returned = 2;
        break;
    case Verde:
        returned = 3;
        break;
    }
    return returned;
}
int zanataJugador(int jugadorfin) { 
    int casillafin = 0;
    switch (jugadorfin)
    {
    case 0:
        casillafin = 0;
        break;
    case 1:
        casillafin = 17;
        break;
    case 2:
        casillafin = 34;
        break;
    case 3:
        casillafin = 51;
        break;
    }
    return casillafin;
}
int salidaJugador(int jugador) { 
    int casillaIni = 0;
    switch (jugador)
    {
    case 0: casillaIni = 5; break;
    case 1: casillaIni = 22; break;
    case 2: casillaIni = 39; break;
    case 3: casillaIni = 56; break;
    }
    return casillaIni;
}
int eligeColor() { 

    int jugador = 5; 

    while (jugador < 1 || jugador > 4) {

        cout << " ------------------------------------ " << endl;
        cout << "|                                    |" << endl;
        cout << "|     Elige el color que quieras:    |" << endl;
        cout << "|                                    |" << endl;
        cout << "|     1. Amarillo                    |" << endl;
        cout << "|     2. Azul                        |" << endl;
        cout << "|     3. Rojo                        |" << endl;
        cout << "|     4. Verde                       |" << endl;
        cout << "|                                    |" << endl;
        cout << " ------------------------------------ " << endl;
        cout << "" << endl;
        cout << "    Opcion: ";
        cin >> jugador;

        if (cin.fail()) {

            cin.clear();
            cin.ignore(200, '\n');
        }
        system("pause");;
        system("CLS");

        if (jugador > 0 && jugador < 5) {

            return jugador;
        } 
    }

}
int tirarDado(bool& fin) {
    int numDado;
    if (modoDebug) {
        cout << "Dado: (Introduce 0 para salir): ";
        cin >> numDado;
        if (numDado == 0) { fin = true; }
        cin.get(); 
                                                    
    }
    else
        numDado = rand() % 6 + 1;

    return numDado;

}
string colorACadena(tColor color) {   
    string strColor;
    if (color == 0) {
        strColor = "Amarillo";
    }
    else if (color == 1) {
        strColor = "Azul";
    }
    else if (color == 2) {
        strColor = "Rojo";
    }
    else if (color == 3) {
        strColor = "Verde";
    }
    return strColor;
}
char colorALetra(tColor color) { 

    char charColor;

    switch (color) {
    case 0:
        charColor = 'M';
    case 1:
        charColor = 'A';
    case 2:
        charColor = 'R';
    case 3:
        charColor = 'V';
    }
    /*
    if (color == 0) {
        charColor = 'M';
    }
    else if (color == 1) {
        charColor = 'A';
    }
    else if (color == 2) {
        charColor = 'R';
    }
    else if (color == 3) {
        charColor = 'V';
    }
    */
    return charColor;
};
void mostrar(const tJuego& juego) {
    int casilla, ficha;
    tColor jug;

    cout << "\x1b[2J\x1b[H"; 
    setColor(Gris);
    cout << endl;

    for (int i = 0; i < NUM_CASILLAS; i++)
        cout << i / 10;
    cout << endl;
    for (int i = 0; i < NUM_CASILLAS; i++)
        cout << i % 10;
    cout << endl;

    for (int i = 0; i < NUM_CASILLAS; i++)
        cout << '>';
    cout << endl;

    for (int i = 0; i < NUM_CASILLAS; i++) {
        setColor(juego.casillas[i].calle2);
        if (juego.casillas[i].calle2 != Ninguno)
            cout << primeraEn(juego.jugadores[juego.casillas[i].calle2].fichas, i, juego) + 1;
        else
            cout << ' ';
        setColor(Gris);
    }
    cout << endl;

    for (int i = 0; i < NUM_CASILLAS; i++)
        if (esSeguro(i))
            cout << 'o';
        else
            cout << '-';
    cout << endl;

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


    for (int i = 0; i < NUM_FICHAS; i++) {
        casilla = 0;
        jug = Amarillo;
        setColor(jug);
        while (casilla < NUM_CASILLAS) {
            if (casilla == zanataJugador(jug)) {
                ficha = primeraEn(juego.jugadores[jug].fichas, 101 + i, juego);
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
                if (juego.jugadores[jug].fichas[i] == -1) 
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


    for (int i = 105; i <= 107; i++) {
        casilla = 0;
        jug = Amarillo;
        setColor(jug);
        while (casilla < NUM_CASILLAS) {
            if (casilla == zanataJugador(jug)) {
                ficha = primeraEn(juego.jugadores[jug].fichas, i, juego);
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
        cout << ((juego.jugadores[jug].fichas[3] == 108) ? '4' : '.');
        jug = tColor(int(jug) + 1);
        setColor(jug);
        cout << "               ";
        casilla += 17;
    }
    cout << endl << endl;
    setColor(Gris);
}
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