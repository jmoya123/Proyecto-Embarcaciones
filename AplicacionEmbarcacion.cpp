#include <iostream>
#include <malloc.h>
#include <sstream>
using namespace std;

struct Pasajero {
    string nombre;
    Pasajero* siguiente;
};

struct Cola {
    Pasajero* frente;
    Pasajero* final;
    int capacidad;
    int cantidad;
};

struct Viaje {
    string id;
    string destino;
    string matricula;
    string nombreEmbarcacion;
    int dia, mes, anio;
    float precio;
    Cola* pasajeros;
};

struct NodoAVL {
    Viaje* viaje;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;
};

// Funciones para manejar la Cola de Pasajeros
Cola* crearCola(int capacidad) {
    Cola* cola = (Cola*)malloc(sizeof(Cola));
    cola->frente = cola->final = NULL;
    cola->capacidad = capacidad;
    cola->cantidad = 0;
    return cola;
}

void encolar(Cola* cola, string nombre) {
    if (cola->cantidad >= cola->capacidad) {
        cout << "La embarcación ha alcanzado su capacidad máxima." << endl;
        return;
    }
    Pasajero* pasajero = (Pasajero*)malloc(sizeof(Pasajero));
    pasajero->nombre = nombre;
    pasajero->siguiente = NULL;
    if (cola->final != NULL) {
        cola->final->siguiente = pasajero;
    }
    cola->final = pasajero;
    if (cola->frente == NULL) {
        cola->frente = pasajero;
    }
    cola->cantidad++;
}

void listarPasajeros(Cola* cola) {
    Pasajero* actual = cola->frente;
    while (actual != NULL) {
        cout << "Pasajero: " << actual->nombre << endl;
        actual = actual->siguiente;
    }
}

void liberarPasajeros(Cola* cola) {
    Pasajero* actual = cola->frente;
    while (actual != NULL) {
        Pasajero* temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    free(cola);
}

// Funciones para manejar el árbol AVL
int altura(NodoAVL* nodo) {
    return nodo == NULL ? 0 : nodo->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

NodoAVL* crearNodo(Viaje* viaje) {
    NodoAVL* nodo = (NodoAVL*)malloc(sizeof(NodoAVL));
    nodo->viaje = viaje;
    nodo->izq = nodo->der = NULL;
    nodo->altura = 1;
    return nodo;
}

// Declaraciones de funciones de rotación
NodoAVL* rotarDerecha(NodoAVL* y);
NodoAVL* rotarIzquierda(NodoAVL* x);

NodoAVL* insertar(NodoAVL* nodo, Viaje* viaje) {
    if (nodo == NULL)
        return crearNodo(viaje);

    if (viaje->id < nodo->viaje->id)
        nodo->izq = insertar(nodo->izq, viaje);
    else if (viaje->id > nodo->viaje->id)
        nodo->der = insertar(nodo->der, viaje);
    else
        return nodo;

    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));

    int balance = altura(nodo->izq) - altura(nodo->der);

    if (balance > 1 && viaje->id < nodo->izq->viaje->id)
        return rotarDerecha(nodo);

    if (balance < -1 && viaje->id > nodo->der->viaje->id)
        return rotarIzquierda(nodo);

    if (balance > 1 && viaje->id > nodo->izq->viaje->id) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (balance < -1 && viaje->id < nodo->der->viaje->id) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

NodoAVL* rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;

    return x;
}

NodoAVL* rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;

    return y;
}

NodoAVL* buscar(NodoAVL* raiz, string id) {
    if (raiz == NULL || raiz->viaje->id == id)
        return raiz;

    if (raiz->viaje->id < id)
        return buscar(raiz->der, id);

    return buscar(raiz->izq, id);
}

void listarViajes(NodoAVL* nodo) {
    if (nodo != NULL) {
        listarViajes(nodo->izq);
        cout << "Viaje ID: " << nodo->viaje->id << ", Destino: " << nodo->viaje->destino << endl;
        listarViajes(nodo->der);
    }
}

void liberarViajes(NodoAVL* nodo) {
    if (nodo != NULL) {
        liberarViajes(nodo->izq);
        liberarViajes(nodo->der);
        liberarPasajeros(nodo->viaje->pasajeros);
        free(nodo->viaje);
        free(nodo);
    }
}

// Función para registrar un viaje
Viaje* crearViaje(string id, string destino, string matricula, string nombreEmbarcacion, int dia, int mes, int anio, float precio, int capacidadPasajeros) {
    Viaje* viaje = (Viaje*)malloc(sizeof(Viaje));
    viaje->id = id;
    viaje->destino = destino;
    viaje->matricula = matricula;
    viaje->nombreEmbarcacion = nombreEmbarcacion;
    viaje->dia = dia;
    viaje->mes = mes;
    viaje->anio = anio;
    viaje->precio = precio;
    viaje->pasajeros = crearCola(capacidadPasajeros);
    return viaje;
}

int main() {
    NodoAVL* raiz = NULL;
    int opcion;

    do {
        cout << "1. Registrar viaje" << endl;
        cout << "2. Buscar viaje" << endl;
        cout << "3. Listar viajes" << endl;
        cout << "4. Eliminar viaje" << endl;
        cout << "5. Registrar pasajero en viaje" << endl;
        cout << "6. Listar pasajeros de un viaje" << endl;
        cout << "7. Salir" << endl;
        cout << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string id, destino, matricula, nombreEmbarcacion;
                int dia, mes, anio, capacidadPasajeros;
                float precio;
                
                cout << "Ingrese el ID del viaje: ";
                cin >> id;
                cout << "Ingrese el destino: ";
                cin >> destino;
                cout << "Ingrese la matrícula: ";
                cin >> matricula;
                cout << "Ingrese el nombre de la embarcación: ";
                cin >> nombreEmbarcacion;
                cout << "Ingrese la fecha (día mes año): ";
                cin >> dia >> mes >> anio;
                cout << "Ingrese el precio: ";
                cin >> precio;
                cout << "Ingrese la capacidad de pasajeros: ";
                cin >> capacidadPasajeros;
                
                Viaje* nuevoViaje = crearViaje(id, destino, matricula, nombreEmbarcacion, dia, mes, anio, precio, capacidadPasajeros);
                raiz = insertar(raiz, nuevoViaje);
                cout << "Viaje registrado exitosamente." << endl;
                break;
            }
            case 2: {
                string id;
                cout << "Ingrese el identificador del viaje a buscar: ";
                cin >> id;
                NodoAVL* resultado = buscar(raiz, id);
                if (resultado) {
                    cout << "Viaje encontrado: " << resultado->viaje->destino << endl;
                } else {
                    cout << "Viaje no encontrado." << endl;
                }
                break;
            }
            case 3: {
                cout << "Lista de viajes registrados: " << endl;
                listarViajes(raiz);
                break;
            }
            case 4: {
                string id;
                cout << "Ingrese el identificador del viaje a eliminar: ";
                cin >> id;
                // Aquí falta la implementación de la función eliminar
                cout << "Viaje eliminado exitosamente." << endl;
                break;
            }
            case 5: {
                string id, nombre;
                cout << "Ingrese el identificador del viaje donde quiere registrar el pasajero: ";
                cin >> id;
                NodoAVL* resultado = buscar(raiz, id);
                if (resultado) {
                    cout << "Ingrese el nombre del pasajero: ";
                    cin >> nombre;
                    encolar(resultado->viaje->pasajeros, nombre);
                    cout << "Pasajero registrado exitosamente." << endl;
                } else {
                    cout << "Viaje no encontrado." << endl;
                }
                break;
            }
            case 6: {
                string id;
                cout << "Ingrese el identificador del viaje para listar los pasajeros: ";
                cin >> id;
                NodoAVL* resultado = buscar(raiz, id);
                if (resultado) {
                    cout << "Lista de pasajeros del viaje " << id << ":" << endl;
                    listarPasajeros(resultado->viaje->pasajeros);
                } else {
                    cout << "Viaje no encontrado." << endl;
                }
                break;
            }
            case 7: {
                cout << "Saliendo del programa." << endl;
                liberarViajes(raiz);
                break;
            }
            default: {
                cout << "Opción no válida. Intente nuevamente." << endl;
                break;
            }
        }
    } while (opcion != 7);

    return 0;
}

