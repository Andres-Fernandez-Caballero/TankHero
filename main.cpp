#include <iostream>
#include "math.h"
#include "stdio.h"
#include "string.h"

using namespace std;

struct BarraVida {
    int vidaMaxima;
    int vidaActual;
};

struct Posicion {
    int fila = -1;
    int columna = -1;
};

struct Tanque {
    char nombre[100];
    int ataque;
    int defensa;
    int alcance;
    struct BarraVida barraVida;
    struct Posicion posicion;
};

struct CampoDeBatalla {
    struct Tanque mapa[10][10];
    int tanquesEnJuego = 0;
};

// Funciones para controlar el campo de batalla
void informacionDelCampoDeBatalla(CampoDeBatalla* campoDeBatalla);
void limpiarCampoDeBatalla(CampoDeBatalla* campoDeBatalla);
void dibujarMapa(CampoDeBatalla* campoDeBatalla);
bool existeTanqueEnPosicion(CampoDeBatalla* campoDeBatalla, int fila, int columna);
struct Tanque* obtenerTanqueEnPosicion(CampoDeBatalla* campoDeBatalla, int fila, int columna);
bool estaElTableroLleno(CampoDeBatalla* campoDeBatalla);

// funciones para controlar tanques
struct Tanque crearTanque();

void agregarTanqueALCampoDeBatalla(Tanque* tanque, CampoDeBatalla* campoDeBatalla, int fila, int columna);
int estaEnAlcance(struct Tanque* tanque1, struct Tanque* tanque2, struct CampoDeBatalla* campoDeBatalla);
void atacarTanque(Tanque* tanqueAtacante, Tanque* tanqueDefensor, struct CampoDeBatalla* campoDeBatalla);
void moverTanque(Tanque* tanque, CampoDeBatalla* campoDeBatalla, int fila, int columna);



int main() {
    struct CampoDeBatalla campoDeBatalla;

    printf("Bienvenido al juego de tanques\n");

    struct Tanque tanque1 = crearTanque();
    struct Tanque tanque2 = crearTanque();

    bool posicionOcupada = true;
    do {
        printf("Ingrese la posicion del tanque 1 fila: ");
        int fila1;
        scanf("%d", &fila1);

        printf("Ingrese la posicion del tanque 1 columna: ");
        int columna1;
        scanf("%d", &columna1);

        if (!existeTanqueEnPosicion(&campoDeBatalla, tanque1.posicion.fila, tanque1.posicion.columna)) {
            posicionOcupada = false;
            agregarTanqueALCampoDeBatalla(&tanque1, &campoDeBatalla, tanque1.posicion.fila, tanque1.posicion.columna);
        }
        else {
            printf("La posicion esta ocupada, por favor ingrese otra\n");
        }

    } while (posicionOcupada);

    do {
        printf("Ingrese la posicion del tanque 2 fila: ");
        int fila2;
        scanf("%d", &fila2);

        printf("Ingrese la posicion del tanque 2 columna: ");
        int columna2;
        scanf("%d", &columna2);

        if (!existeTanqueEnPosicion(&campoDeBatalla, tanque2.posicion.fila, tanque2.posicion.columna)) {
            posicionOcupada = false;
            agregarTanqueALCampoDeBatalla(&tanque2, &campoDeBatalla, tanque2.posicion.fila, tanque2.posicion.columna);
        }
        else {
            printf("La posicion esta ocupada, por favor ingrese otra\n");
        }

    } while (posicionOcupada);

    do {
        if (estaEnAlcance(&tanque1, &tanque2, &campoDeBatalla) && estaEnAlcance(&tanque2, &tanque1, &campoDeBatalla)) {
            printf("Los tanques estan en alcance\n");
            // inicia la batalla
            atacarTanque(&tanque1, &tanque2, &campoDeBatalla);
            if (tanque2.barraVida.vidaActual > 0) {
                atacarTanque(&tanque2, &tanque1, &campoDeBatalla);
                if (tanque1.barraVida.vidaActual <= 0) {
                    printf("El tanque 2 ha ganado\n");
                }
            }
            else {
                printf("El tanque 1 ha ganado\n");
            }



        }
        else {
            printf("Los tanques no estan en alcance\n");
            // mover tanques
            moverTanque(&tanque1, &campoDeBatalla, tanque2.posicion.fila + 1, tanque2.posicion.columna);
        }
    } while (tanque1.barraVida.vidaActual > 0 && tanque2.barraVida.vidaActual > 0);


    return 0;
}

void informacionDelCampoDeBatalla(CampoDeBatalla* campoDeBatalla) {
    cout << "Campo De Batalla\n";
    cout << "Tanques en juego " << campoDeBatalla->tanquesEnJuego << endl;
    for (int fila = 0; fila < 10;fila++) {
        for (int columna = 0; columna < 10; columna++) {
            if (existeTanqueEnPosicion(campoDeBatalla, fila, columna)) {
                cout << "* " << campoDeBatalla->mapa[fila][columna].nombre << "(" << fila << ";" << columna << ")" << endl;
            }
        }
    }
}

void limpiarCampoDeBatalla(CampoDeBatalla* campoDeBatalla) {
    campoDeBatalla->tanquesEnJuego = 0;
    struct Tanque tanqueVacio; // se lo pone en la parte superior para crear solo una instancia de tanque vacio
    for (int fila = 0; fila < 10; fila++) {
        for (int columna = 0; columna < 10; columna++) {
            campoDeBatalla->mapa[fila][columna] = tanqueVacio;
        }
    }
}

void dibujarMapa(CampoDeBatalla* campoDeBatalla) {
    cout << "**********************Campo De "
        "Batalla*********************************";
    for (int fila = 0; fila < 10; fila++) {
        cout << endl << endl;
        for (int columna = 0; columna < 10; columna++) {
            if (existeTanqueEnPosicion(campoDeBatalla, fila, columna)) {
                cout << "T ";
            }
            else {
                cout << "* ";
            }
        }
    }
    cout << "\n******************************************************************"
        "**"
        "***\n";
}

void agregarTanqueALCampoDeBatalla(Tanque* tanque, CampoDeBatalla* campoDeBatalla, int fila, int columna) {
    tanque->posicion.fila = fila;
    tanque->posicion.columna = columna;
    campoDeBatalla->mapa[fila][columna] = *tanque;

    cout << "Tanque " << campoDeBatalla->mapa[fila][columna].nombre << " agregado al campo de batalla en coordenadas (" << fila << ";" << columna << ")\n";
    campoDeBatalla->tanquesEnJuego++;

    //campoDeBatalla->tanques[ campoDeBatalla->tanquesEnJuego-1 ]= tanque;
}

struct Tanque crearTanque() {
    printf("Ingrese los datos del primer tanque\n");

    printf("Ingrese el nombre del tanque: ");
    char nombre[100];
    scanf("%s", nombre);

    printf("Ingrese la vida maxima del tanque: ");
    int vidaMaxima;
    scanf("%d", &vidaMaxima);

    printf("Ingrese el ataque del tanque: ");
    int ataque;
    scanf("%d", &ataque);

    printf("Ingrese la defensa del tanque: ");
    int defensa;
    scanf("%d", &defensa);

    printf("Ingrese el alcance del tanque: ");
    int alcance;
    scanf("%d", &alcance);

    struct Tanque nuevoTanque;

    strcpy(nuevoTanque.nombre, nombre);
    nuevoTanque.barraVida.vidaMaxima = vidaMaxima;
    nuevoTanque.barraVida.vidaActual = vidaMaxima;
    nuevoTanque.ataque = ataque;
    nuevoTanque.defensa = defensa;
    nuevoTanque.alcance = alcance;

    return nuevoTanque;
}

bool existeTanqueEnPosicion(CampoDeBatalla* campoDeBatalla, int fila, int columna) {
    return strcmp(campoDeBatalla->mapa[fila][columna].nombre, "") != 0;
}

struct Tanque* obtenerTanqueEnPosicion(CampoDeBatalla* campoDeBatalla, int fila, int columna) {
    if (existeTanqueEnPosicion(campoDeBatalla, fila, columna)) {
        return &campoDeBatalla->mapa[fila][columna];
    }
    return NULL;
}

int estaEnAlcance(Tanque* tanqueAtacante, Tanque* tanque2, CampoDeBatalla* campoDeBatalla) {

    if(tanqueAtacante->posicion.fila == -1 || tanqueAtacante->posicion.columna == -1 || tanque2->posicion.fila == -1 || tanque2->posicion.columna == -1){
        return -1; // no estan en el campo de batalla
    }

    int distanciaFila = tanqueAtacante->posicion.fila - tanque2->posicion.fila;
    int distanciaColumna = tanqueAtacante->posicion.columna - tanque2->posicion.columna;
    int moduloDistancia = sqrt(pow(distanciaFila, 2) + pow(distanciaColumna, 2));

    if (moduloDistancia <= tanqueAtacante->alcance) {
        return 1;
    }
    else {
        return 0;
    }
}

void atacarTanque(Tanque* tanqueAtacante, Tanque* tanqueDefensor, CampoDeBatalla* campoDeBatalla) {
    if (estaEnAlcance(tanqueAtacante, tanqueDefensor, campoDeBatalla) == 1) {
        int ataqueResultado = tanqueAtacante->ataque - tanqueDefensor->defensa;
        if(ataqueResultado < 0) { 
            ataqueResultado = 0; 
        }

        tanqueDefensor->barraVida.vidaActual -= ataqueResultado;
        if (tanqueDefensor->barraVida.vidaActual < 0) {
            tanqueDefensor->barraVida.vidaActual = 0;
        }
        cout << "El tanque " << tanqueAtacante->nombre << " ataco al tanque " << tanqueDefensor->nombre << endl;
        cout << "Vida actual del tanque " << tanqueDefensor->nombre << " es " << tanqueDefensor->barraVida.vidaActual << endl;
    }
    else {
        cout << "El tanque " << tanqueAtacante->nombre << " no esta en rango de ataque de " << tanqueDefensor->nombre << endl;
    }
}

bool estaElTableroLleno(CampoDeBatalla* campoDeBatalla) {
    bool estaLleno = true;
    for (int fila = 0; fila < 10; fila++) {
        for (int columna = 0; columna < 10; columna++) {
            if (existeTanqueEnPosicion(campoDeBatalla, fila, columna) == false) {
                estaLleno = false;
            }
        }
    }
    return estaLleno;
}

void moverTanque(Tanque* tanque, CampoDeBatalla* campoDeBatalla, int fila, int columna) {
    if (existeTanqueEnPosicion(campoDeBatalla, fila, columna)) {
        printf("No se puede mover el tanque %s a la posicion (%d;%d) porque ya hay un tanque en esa posicion\n", tanque->nombre, fila, columna);
    }
    else if (fila < 0 && fila > 9 && columna < 0 && columna > 9) {
        printf("No se puede mover el tanque %s a la posicion (%d;%d) porque esta fuera del tablero\n", tanque->nombre, fila, columna);

    }
    else {
        struct Tanque tanqueVacio;
        campoDeBatalla->mapa[tanque->posicion.fila][tanque->posicion.columna] = tanqueVacio;

        tanque->posicion.fila = fila;
        tanque->posicion.columna = columna;
        campoDeBatalla->mapa[fila][columna] = *tanque;
    }
}