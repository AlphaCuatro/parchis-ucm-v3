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
const bool debug = false;
const string Archivo = "archivo.txt";

//  DECLARACIÓN DE TIPOS Y ESTRUCTURAS

typedef int tCasillas[NUM_CASILLAS];
typedef int tFichas[NUM_FICHAS];
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
    
    int premio = 0;
    int seises = 0;
    int tirada = 0;
    int ultimaFichaMovida = 0;

    tCasilla casillas[NUM_CASILLAS] = { };
    tJugador jugadores[NUM_JUGADORES] = { };
    tColor turno = Ninguno;

};

//  DECLARACIÓN DE FUNCIONES

int eligeColor();                                                                                           //
int zanataJugador(int jugadorfin);                                                                          //
int primeraEn(tFichas jugador, int casilla, const tJuego juego);                                            //
int segundaEn(const tFichas jugador, int casilla);                                                          //
int cuantasEn(const tFichas jugador, int casilla);                                                          //
int salidaJugador(int jugador);                                                                             //
int colorAJugador(tColor color);                                                                            //
int tirarDado(bool& fin);                                                                                   //

void mostrar(const tJuego& juego);                                                                          //
void aCasita(int casilla, int& casilla2, int jugador, int jugador2, int& i);                                //
void jugar(tJuego& juego, bool& fin);                                                                       //
void saleFicha(tJuego& juego, int ficha);                                                                   //
void setColor(tColor color);                                                                                //
void iniciar(tJuego& juego);                                                                                //
void iniciaColores();                                                                                       //
void usarPremio(tJuego& juego);                                                                             //
void mover(tJuego& juego, int ficha, int casilla);                                                          //
void cargar(tJuego& juego);                                                                                 //
void reiniciarTurno(tJuego& juego, int primerJugador);                                                      //

bool finJuego(tJuego juego);                                                                                //
bool esSeguro(int seguro);                                                                                  //
bool puedeMover(const tJuego juego, int ficha, int& casilla);                                               //
bool procesa5(tJuego& juego);                                                                               //
bool procesa6(tJuego& juego);                                                                               //
bool puente(const tCasillas calle1, const tCasillas calle2, int casilla);                                   //

string colorACadena(tColor color);                                                                          //
char colorALetra(tColor color);                                                                             //

/*
    LAS FUNCIONES OPCIONALES DE LA
    IA DE LA V3 NO ESTÁN IMPLEMENTADAS
*/

//  IMPLEMENTACIÓN DE FUNCIONES

int main() {
    
    time(NULL);

    int primerJugador = rand() % 4;
    bool fin = false;
    tJuego juego;
    tColor jugador = tColor(eligeColor());
    juego.turno = tColor(primerJugador);

    iniciar(juego);

    while (!finJuego(juego)) {

        mostrar(juego);
        jugar(juego, fin);

        if (juego.premio != 0) {
            usarPremio(juego);
        }

        reiniciarTurno(juego, primerJugador);
    }

    cout << endl << "FIN DEL JUEGO";
    return 0;
}
void cargar(tJuego& juego) {

    ifstream archivo;
    int jugador, casilla;
    int tiradas[1000];
    bool stop = false;

    label:

    cout << "Presione una tecla para continuar...";
    system("PAUSE");

    archivo.open("archivo.txt");

    if (!archivo.is_open()) {
        cout << "ARCHIVO NO ENCONTRADO";
        goto label;
    }

    else if (archivo.is_open()) {

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

        for (int i = 0; !stop; i++) {

            archivo >> tiradas[i];
            if (tiradas[i] == -1) { stop = true; }
        }

        archivo.close();
    }
}
void usarPremio(tJuego& juego) {

    int tirada = juego.tirada;
    int ficha;
    int error = 0;
    bool tienePremio = false;
    bool stop = false;


    juego.tirada = juego.premio;
    mostrar(juego);

    cout << "Has ganado un premio, puedes contar " << juego.premio << " casillas con las siguientes fichas:" << endl;

    // Print fichas que puedan utilizar juego.premio

    for (int i = 0; i < NUM_FICHAS; i++) {

        if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {

            cout << "La ficha " <<  i + 1 << " puede moverse " << juego.premio << " casillas!" << endl;
            tienePremio = true;
        }
    }

    if (tienePremio) {

        cout << "Elige la ficha con la que quieras utilizar el premio: ";

        cin >> ficha;

        while (ficha > 4 || ficha < 1 || !stop) {

            if (error != 0) {
                cout << "Elige una opción válida: ";
            }

            error++;

            for (int i = 0; i < NUM_FICHAS && !stop; i++) {

                if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == ficha) {
                    stop;
                }
            }
        }

        if (ficha > 0 && ficha < 5 && puedeMover(juego, ficha - 1, juego.jugadores[juego.turno].fichas[ficha - 1])) {
            mover(juego, ficha - 1, juego.jugadores[juego.turno].fichas[ficha - 1] + juego.premio);
        }
    }
    // Si premio no puede ser utilizado

    else { cout << "No tienes ninguna ficha con la que puedas mover " << juego.premio << " casillas." << endl; }

    system("pause");

    // Reset del sistema de premios

    juego.premio = 0;
    juego.tirada = tirada;
}
void mover(tJuego& juego, int ficha, int casilla) {

    int casillaDestino = casilla;

    // Despeja calle1 si solo hay calle1

    if (juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2 == Ninguno && juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle1 == juego.turno) { 
        juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle1 = Ninguno;
    }

    // Despeja calle2, pasa su valor fichasEnMeta calle1

    else if (juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2 != Ninguno && juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2 != juego.turno && juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle1 == juego.turno) {
        juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle1 = juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2;
        juego.casillas[juego.jugadores[juego.turno].fichas[ficha]].calle2 = Ninguno;
    }


    if (juego.jugadores[juego.turno].fichas[ficha] == zanataJugador(juego.turno)) {
        juego.jugadores[juego.turno].fichas[ficha] = 100 + juego.tirada;
        casillaDestino = juego.tirada + 100;
    }

    else {

        juego.jugadores[juego.turno].fichas[ficha] = juego.jugadores[juego.turno].fichas[ficha] + juego.tirada;
        
        if (juego.jugadores[juego.turno].fichas[ficha] > 67 && juego.jugadores[juego.turno].fichas[ficha] < 101) {
            juego.jugadores[juego.turno].fichas[ficha] -= 68;
            casillaDestino = casillaDestino - 68;
        }
    }


    if (juego.casillas[casillaDestino].calle1 == Ninguno) {
        juego.casillas[casillaDestino].calle1 = juego.turno;
    }
    else if (juego.casillas[casillaDestino].calle1 != Ninguno && juego.casillas[casillaDestino].calle1 != juego.turno && juego.casillas[casillaDestino].calle2 == Ninguno && esSeguro(casillaDestino)) { //si es seguro y la casilla de la calle1 ya está ocupada
        juego.casillas[casillaDestino].calle2 = juego.turno;
    }
    else if (juego.casillas[casillaDestino].calle1 != Ninguno && juego.casillas[casillaDestino].calle1 == juego.turno && juego.casillas[casillaDestino].calle2 == Ninguno) { //si el propio jugador ya tiene una ficha en esa casilla
        juego.casillas[casillaDestino].calle2 = juego.turno;
    }
    else if (juego.casillas[casillaDestino].calle1 != Ninguno && juego.casillas[casillaDestino].calle1 != juego.turno && juego.casillas[casillaDestino].calle2 == Ninguno && !esSeguro(casillaDestino) && juego.jugadores[juego.turno].fichas[ficha] < 101) { //si el jugador se come ficha otra ficha
        for (int i = 0; i < NUM_FICHAS; i++) {

            if (juego.jugadores[juego.casillas[casillaDestino].calle1].fichas[i] == casillaDestino) {
                juego.jugadores[juego.casillas[casillaDestino].calle1].fichas[i] = -1;
                juego.casillas[casillaDestino].calle1 = juego.turno;
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
void jugar(tJuego& juego, bool& fin) { // Funcion importante,

    bool stop = false;
    int error = 0;
    int ficha = 0;
    int siete = 0;
    int casilla = 0;
    bool a = false;
    int casillapasada = 0;
    int opcion = 6;


    juego.seises = 0;
    juego.tirada = 8;

    for (int i = 0; i < NUM_FICHAS; i++) {
        if (juego.jugadores[tColor(juego.turno)].fichas[i] != -1) {
            casilla++;
        }
    }

    if (casilla == 4) { siete++; }

    while ((juego.tirada == 6 + siete || ficha == 0) && !fin && juego.seises != 3) {

        ficha++;
        cout << "Turno para jugador " << colorACadena(juego.turno) << "." << endl;
        juego.tirada = tirarDado(fin);
        if (juego.tirada == 6) {
            juego.tirada += siete;
        }
        cout << "DADO: " << juego.tirada << "." << endl;
        if (juego.tirada == 6 || juego.tirada == 7) { juego.seises++; }
        if (!procesa6(juego)) {
            if (juego.tirada == 7) { juego.tirada = 6; }

            switch (juego.tirada)
            {
            case 1:

                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;
                        casilla++;
                    }
                }
                if (casilla > 0) {

                    while (opcion > 4 || opcion < 1 || !stop || cin.fail()) {
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
                        for (int i = 0; i < NUM_FICHAS && !stop; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                stop = true;
                            }
                        }
                    }
                    casilla = 0;
                }
                break;

            case 2:

                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;
                        casilla++;
                    }
                }
                if (casilla > 0) {

                    while (opcion > 4 || opcion < 1 || !stop) {
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
                        for (int i = 0; i < NUM_FICHAS && !stop; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                stop = true;
                            }
                        }
                    }
                    casilla = 0;
                }
                break;
            case 3:

                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;
                        casilla++;
                    }
                }
                if (casilla > 0) {

                    while (opcion > 4 || opcion < 1 || !stop) {
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
                        for (int i = 0; i < NUM_FICHAS && !stop; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                stop = true;
                            }
                        }
                    }
                    casilla = 0;
                }
                break;
            case 4:

                for (int i = 0; i < NUM_FICHAS; i++) {
                    casillapasada = juego.jugadores[juego.turno].fichas[i];
                    if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                        cout << i + 1 << ": ficha puede moverse " << juego.tirada << " casillas." << endl;
                        casilla++;
                    }
                }
                if (casilla > 0) {
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
                    casilla = 0;
                }
                break;
            case 5:

                for (int i = 0; i < NUM_FICHAS && !a; i++) {

                    if (juego.jugadores[juego.turno].fichas[i] == -1 && procesa5(juego)) {
                        saleFicha(juego, i);

                        a = true;
                        casilla++;
                    }
                }

                if (casilla == 0) {
                    for (int i = 0; i < NUM_FICHAS; i++) {
                        casillapasada = juego.jugadores[juego.turno].fichas[i];
                        if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i])) {
                            cout << i + 1 << ": La ficha puede moverse " << juego.tirada << " casillas" << endl;

                        }
                    }
                }


                if (casilla == 0) {

                    while (opcion > 4 || opcion < 1 || !stop) {
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
                        for (int i = 0; i < NUM_FICHAS && !stop; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                stop = true;
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
                        casilla++;
                    }
                }
                if (casilla > 0) {

                    while (opcion > 4 || opcion < 1 || !stop) {
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
                        for (int i = 0; i < NUM_FICHAS && !stop; i++) {
                            casillapasada = juego.jugadores[juego.turno].fichas[i];
                            if (puedeMover(juego, i, juego.jugadores[juego.turno].fichas[i]) && i + 1 == opcion) {
                                stop = true;
                            }
                        }
                    }
                    casilla = 0;
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
void iniciar(tJuego& juego) { // Inicializa tablero, aleatorios y randomiza primer turno

    srand(time(NULL)); // Inicializador de num. pseudo-aleatorios
    setColor(Ninguno); // Hace que el fondo de la primera tirada se muestre completamente en blanco, en vez de en negro. No debería funcionar así, pero no hay drawbacks, así que ahí se queda de momento.
    juego.turno = tColor(0 + rand() % (3)); // Randomizador de primer turno

    for (int i = 0; i < NUM_CASILLAS; i++) {

        juego.casillas[i].calle1 = Ninguno;
        juego.casillas[i].calle2 = Ninguno;
    }


    for (int j = 0; j < NUM_JUGADORES; j++) {

        for (int i = 0; i < NUM_FICHAS; i++) {

            juego.jugadores[j].fichas[i] = -1;
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
    int casillareal = 0;

    if (casillafin > 67 && casillafin < 101) {
        casillafin -= 68;
    }
    if (juego.jugadores[juego.turno].fichas[ficha] == -1) {
        ok = false;
    }
    if (juego.jugadores[juego.turno].fichas[ficha] == zanataJugador(juego.turno)) {
        for (int i = 1; i <= juego.tirada && ok; i++) { 
            casillareal = 100 + i;

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
    if (juego.casillas[casillafin].calle1 != Ninguno && juego.casillas[casillafin].calle2 == juego.casillas[casillafin].calle1 && juego.jugadores[juego.turno].fichas[ficha] < 100) { //por si en la casilla que va ficha caer ya hay dos fichas
        ok = false;
    }
    return ok;
}
bool finJuego(const tJuego juego) { // Devuelve true si las 4 fichas de un jugador están en meta.

    bool finJuego = false;
    int fichasEnMeta = 0;

    for (int numJugador = 0; numJugador < NUM_JUGADORES; numJugador++) {

        for (int numFicha = 0; numFicha < NUM_FICHAS; numFicha++) {

            if (juego.jugadores[numJugador].fichas[numFicha] == 108) { fichasEnMeta++; }
            if (fichasEnMeta == 4) { finJuego = true; break; }
        }
    }

    return finJuego;
}
bool esSeguro(int s) { // Devuelve true si la casilla es segura. En caso contrario, devuelve false

    bool esSeguro = false;

    switch (s) {

    case 0:  esSeguro = true; break;
    case 5:  esSeguro = true; break;
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

    default: esSeguro = false; break;
    }

    return esSeguro;
}
int cuantasEn(const tFichas jugador, int casilla) { // Devuelve el número de fichas que habitan en la casilla 

    int fichasEnCasilla = 0;

    for (int i = 0; i < NUM_FICHAS; i++) {

        if (jugador[i] == casilla) {
            fichasEnCasilla++;
        }
    }

    return fichasEnCasilla;
}
int segundaEn(const tFichas jugador, int casilla) { // Devuelve el int correspondiente fichasEnMeta la ficha que esté en segundo lugar en esa casilla

    int segundaEn = -1;
    int a = 0;
    int b = 0;

    for (int i = 0; i < NUM_FICHAS; i++) {

        if (jugador[i] == casilla) {
            a++;
        }
    }

    if (a == 1) {
        for (int i = 0; i < NUM_FICHAS && segundaEn == -1; i++) {

            if (jugador[i] == casilla) {
                segundaEn = i;
            }
        }
    }

    else if (a == 2) {
        for (int i = 0; i < NUM_FICHAS && segundaEn == -1; i++) {

            if (jugador[i] == casilla) {
                b++;
            }

            if (b == 2) {
                segundaEn = i;
            }
        }

    }

    return segundaEn;
}
int primeraEn(const tFichas jugador, int casilla, tJuego juego) { // Devuelve el int correspondiente fichasEnMeta la ficha que esté en primer lugar en esa casilla

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

    if ((juego.casillas[casilla].calle2 != juego.casillas[casilla].calle1) && (juego.casillas[casilla].calle2 != Ninguno)) {
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
int colorAJugador(tColor color) { // Devuelve el tColor correspondiente fichasEnMeta cada jugador (int)

    int jugador = 0;

    switch (color) {

    case Amarillo:  jugador = 0; break;
    case Azul:      jugador = 1; break;
    case Rojo:      jugador = 2; break;
    case Verde:     jugador = 3; break;
    }

    return jugador;
}
int zanataJugador(int jugador) {  // Devuelve la zanata correspondiente fichasEnMeta cada jugador
   
    int casillaZanata = 0;
    
    switch (jugador) {
    case 0: casillaZanata = 0;  break;
    case 1: casillaZanata = 17; break;
    case 2: casillaZanata = 34; break;
    case 3: casillaZanata = 51; break;
    }

    return casillaZanata;
}
int salidaJugador(int jugador) { // Devuelve la casilla de salida correspondiente fichasEnMeta cada jugador

    int casillaSalida = 0;
    
    switch (jugador)
    {
    case 0: casillaSalida = 5; break;
    case 1: casillaSalida = 22; break;
    case 2: casillaSalida = 39; break;
    case 3: casillaSalida = 56; break;
    }

    return casillaSalida;
}
int eligeColor() {  // Permite elegir el color del jugador, contiene path anti-error implementado con labels. (Eficiencia ligeramente superior fichasEnMeta while-loop, aunque despreciable fichasEnMeta bajo nivel y más "feo")

    int jugador = 0; 

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

        label:

        cin >> jugador;
        cout << endl;

        while (jugador < 1 || jugador > 4) {

            cout << "Elige una opcion valida: ";
                goto label;
        }

        system("CLS");
        system("PAUSE");

        return jugador;
        
    }

}
int tirarDado(bool& terminarDebug) { // Si debug está activo solicita input (0 para salir). Si no, randomiza un número del 1 al 6 sobre un pseudo-aleatorio basado en tiempo local.

    int dado;

    if (debug) {

        cout << "Dado: (0 para salir): ";
        cin >> dado;

        if (dado == 0) { terminarDebug = true; }
                                            
    }

    else dado = rand() % 6 + 1;

    return dado;

}
string colorACadena(tColor color) {   // tColor --> String correspondiente
 
    string strColor;

    switch (color) {

    case 0: strColor =  "Amarillo";
    case 1: strColor = "Azul";
    case 2: strColor = "Rojo";
    case 3: strColor = "Verde";

    }

    return strColor;
}
char colorALetra(tColor color) {  // tColor --> Char correspondiente

    char charColor = '!';

    switch (color) {

    case 0: charColor = 'M';
    case 1: charColor = 'A';
    case 2: charColor = 'R';
    case 3: charColor = 'V';
    
    }

    return charColor;
};
void mostrar(const tJuego& juego) { // Función dada, muestra tablero
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
void setColor(tColor color) { // Función dada, relaciona el color en terminal con su tColor correspondiente
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
void iniciaColores() { // Función dada, inicia colores
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
void reiniciarTurno(tJuego& juego, int primerJugador) { // Agrupa el último paso de jugar,  en lo relacionado con los turnos.

    juego.turno = tColor(juego.turno + 1);
    if ((primerJugador + juego.turno) > 3) { juego.turno = tColor(primerJugador + juego.turno - 4); }
    primerJugador = 0;
    system("pause");
    system("CLS");

}