#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Definición de constantes para el tamaño del cartón y el rango de números de cada columna
const int NUM_ROWS = 5;
const int NUM_COLS = 5;
const int BINGO_SIZE = 75;

// Función que inicializa el cartón
void inicializarCarton(int numeros[NUM_ROWS][NUM_COLS], bool marcados[NUM_ROWS][NUM_COLS]) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            numeros[i][j] = 0;
            marcados[i][j] = false;
        }
    }
}

// Función que genera números aleatorios en el cartón respetando las reglas del juego
void generarCarton(int numeros[NUM_ROWS][NUM_COLS]) {
    int rango[NUM_COLS][2] = { {1, 15}, {16, 30}, {31, 45}, {46, 60}, {61, 75} };  // Rango de números por columna

    for (int col = 0; col < NUM_COLS; col++) {
        int numerosDisponibles[15];
        for (int i = 0; i < 15; i++) {
            numerosDisponibles[i] = rango[col][0] + i;
        }

        // Desordenar los números dentro del rango para que sean aleatorios
        for (int i = 14; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = numerosDisponibles[i];
            numerosDisponibles[i] = numerosDisponibles[j];
            numerosDisponibles[j] = temp;
        }

        // Asignar los números al cartón, respetando la casilla central vacía
        for (int row = 0; row < NUM_ROWS; row++) {
            if (col == 2 && row == 2) {
                numeros[row][col] = 0;  // Casilla central
            }
            else {
                numeros[row][col] = numerosDisponibles[row];
            }
        }
    }
}

// Función que imprime el cartón en la consola
void imprimirCarton(int numeros[NUM_ROWS][NUM_COLS], bool marcados[NUM_ROWS][NUM_COLS]) {
    cout << "--------------------------" << endl;
    for (int i = 0; i < NUM_ROWS; i++) {
        cout << "|"; //inicio de linea
        for (int j = 0; j < NUM_COLS; j++) {
            if (marcados[i][j]) {
                cout << "  X |";  // Si el número está marcado, se imprime una X
            }
            else {
                string espacio = " ";
                //Longitud del numero
                if(numeros[i][j] < 10) 
                    espacio = "  ";
                
                cout << espacio << numeros[i][j] << " |";  // Si no está marcado, se imprime el número
            }
        }
        cout << endl;  // Salto de línea al final de cada fila
        cout << "--------------------------" << endl;
    }
}

// Función que genera los números del bombo de forma aleatoria
void generarBombo(int bombo[BINGO_SIZE]) {
    for (int i = 0; i < BINGO_SIZE; i++) {
        bombo[i] = i + 1;
    }

    // Desordenar los números del bombo para que sean aleatorios
    for (int i = BINGO_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = bombo[i];
        bombo[i] = bombo[j];
        bombo[j] = temp;
    }
}

// Función que marca un número en el cartón si es que está presente
bool marcarNumero(int numeros[NUM_ROWS][NUM_COLS], bool marcados[NUM_ROWS][NUM_COLS], int numero) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (numeros[i][j] == numero) {
                marcados[i][j] = true;  // Marcar el número
                return true;  // Devolver verdadero si se encontró y marcó el número
            }
        }
    }
    return false;  // Devolver falso si no se encontró el número en el cartón
}

// Función que verifica si un cartón forma la letra L
bool verificarPatronL(bool marcados[NUM_ROWS][NUM_COLS]) {
    // Verificar la primera columna
    for (int i = 0; i < NUM_ROWS; i++) {
        if (!marcados[i][0]) {
            return false;
        }
    }
    // Verificar la última fila
    for (int j = 1; j < NUM_COLS; j++) {
        if (!marcados[4][j]) {
            return false;
        }
    }
    return true;
}

// Función que verifica si un cartón forma la letra A
bool verificarPatronA(bool marcados[NUM_ROWS][NUM_COLS]) {
    // Verificar la primera columna y la última columna
    for (int i = 0; i < NUM_ROWS; i++) {
        if (!marcados[i][0] || !marcados[i][4]) {
            return false;
        }
    }
    // Verificar la primera fila
    if (!marcados[0][1] || !marcados[0][2] || !marcados[0][3]) {
        return false;
    }
    // Verificar la tercera fila
    if (!marcados[2][1] || !marcados[2][3]) {
        return false;
    }
    return true;
}

// Función que verifica si un cartón forma la letra O
bool verificarPatronO(bool marcados[NUM_ROWS][NUM_COLS]) {
    // Verificar la primera columna y la última columna
    for (int i = 0; i < NUM_ROWS; i++) {
        if (!marcados[i][0] || !marcados[i][NUM_COLS - 1]) {
            return false;
        }
    }
    // Verificar la primera fila
    if (!marcados[0][1] || !marcados[0][2] || !marcados[0][3]) {
        return false;
    }
    // Verificar la quinta fila
    if (!marcados[4][1] || !marcados[4][2] || !marcados[4][3]) {
        return false;
    }
    return true;
}

// Función que verifica si un cartón ha completado el apagón
bool verificarApagon(int numeros[NUM_ROWS][NUM_COLS], bool marcados[NUM_ROWS][NUM_COLS]) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            // Verificar que todas las casillas estén marcadas, excepto la casilla central que es 0
            if (!marcados[i][j] && numeros[i][j] != 0) {
                return false;
            }
        }
    }
    return true;
}

// Función principal que controla el flujo del juego
void jugar(int cartones[][NUM_ROWS][NUM_COLS], bool marcados[][NUM_ROWS][NUM_COLS], int numCartones, int bombo[], int sizeBombo, char patron) {
    //Control para verificar si la cartilla gano o no
    bool bJugada[2] = {false,false};

    cout << "=======================" << endl;
    cout << "Iniciamos el BINGO-LPOO" << endl;
    cout << "=======================" << endl;
    for (int i = 0; i < sizeBombo; i++) {
        int bola = bombo[i];
        cout << "Juagada " << (i + 1) << ", salio el numero: " << bola << endl;
        cout << "==================" << endl;
        
        // Verificar cada cartón
        for (int j = 0; j < numCartones; j++) {
            marcarNumero(cartones[j], marcados[j], bola);
            cout << "Carton " << (j + 1) << endl;
            imprimirCarton(cartones[j], marcados[j]);

            // Verificar si se ha formado el patrón L
            if (patron == 'L' && verificarPatronL(marcados[j])) {
                cout << "===================================================" << endl;
                cout << "El Carton " << (j + 1) << " ha ganado con el patron L!" << endl;
                bJugada[j] = true; //Gano
            }

            // Verificar si se ha formado el patrón A
            if (patron == 'A' && verificarPatronA(marcados[j])) {
                cout << "===================================================" << endl;
                cout << "El Carton " << (j + 1) << " ha ganado con el patron A!" << endl;
                bJugada[j] = true; //Gano
            }

            // Verificar si se ha formado el patrón O
            if (patron == 'O' && verificarPatronO(marcados[j])) {
                cout << "===================================================" << endl;
                cout << "El Carton " << (j + 1) << " ha ganado con el patron O!" << endl;
                bJugada[j] = true; //Gano
            }

            // Verificar si se ha completado el apagón
            if (patron == 'Z' && verificarApagon(cartones[j], marcados[j])) {
                cout << "===================================================" << endl;
                cout << "El Carton " << (j + 1) << " ha ganado con el apagon!" << endl;
                bJugada[j] = true; //Gano
            }
        }
    
        //Verificamos si hubo ganador
        if (bJugada[0] || bJugada[1])
            return; //Salimos
    }
}

int main() {
    int cartones[2][NUM_ROWS][NUM_COLS];  // Se crean 2 cartones para el juego
    bool marcados[2][NUM_ROWS][NUM_COLS];
    int bombo[BINGO_SIZE];
    char opcion, patron;

    // Semilla para la generación de números aleatorios
    srand(time(0));

    cout << "Bienvenido al juego del Bingo LPOO." << endl;
    cout << "1.- Generar los 2 cartones de forma aleatoria." << endl;
    cout << "2.- Generar el bombo de bolillas de forma aleatoria." << endl;
    cout << "3.- Jugar." << endl;

    do {
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch (opcion) {
        case '1':
            // Inicializar y generar los cartones de forma aleatoria
            inicializarCarton(cartones[0], marcados[0]);
            inicializarCarton(cartones[1], marcados[1]);
            generarCarton(cartones[0]);
            generarCarton(cartones[1]);
            cout << "Cartones generados:" << endl;
            cout << "==================" << endl;
            cout << "Carton 1:" << endl;
            imprimirCarton(cartones[0], marcados[0]);
            cout << "Carton 2:" << endl;
            imprimirCarton(cartones[1], marcados[1]);
            break;
        case '2':
            // Generar el bombo de forma aleatoria
            generarBombo(bombo);
            cout << "Bombo ha sido generado." << endl;
            break;
        case '3':
            // Iniciar el juego
            cout << "Ingrese la letra a formar, las opciones son L, A, O, Z (Apagon): ";
            cin >> patron;
            jugar(cartones, marcados, 2, bombo, BINGO_SIZE, patron);
            break;
        default:
            cout << "Opción no valida. Intente de nuevo." << endl;
        }
    } while (opcion != '3');

    return 0;
}
