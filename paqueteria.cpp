// Proyecto final - Estructura de Datos
// Sistema de paqueteria y rutas entre sucursales
// ABB, cola, pila, grafo y radix sort para ordenar por peso

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

const int NUM_SUC = 6;
const int MAX_COLA = 10;
const int MAX_PILA = 10;
const int MAX_PAQ = 100;

string sucursales[NUM_SUC] = {"Centro", "Norte", "Sur", "Oriente", "Poniente", "Puerto"};

enum Estado { REGISTRADO, EN_ESPERA, DESPACHADO };

struct Paquete {
    int folio;
    string descripcion;
    double peso;
    int origen;
    int destino;
    Estado estado;
};

struct Nodo {
    Paquete dato;
    Nodo* izq;
    Nodo* der;
};

struct Cola {
    int datos[MAX_COLA];
    int frente;
    int final;
    int cantidad;
};

struct Pila {
    int datos[MAX_PILA];
    int tope;
};

Nodo* raiz = NULL;
Cola cola;
Pila historial;
int grafo[NUM_SUC][NUM_SUC];


// ---------- lectura de datos ----------

int leerEntero(string msg) {
    int x;
    while (true) {
        cout << msg;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        if (cin.eof()) exit(0);
        cout << "Error: escribe un numero entero" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double leerDecimal(string msg) {
    double x;
    while (true) {
        cout << msg;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        if (cin.eof()) exit(0);
        cout << "Error: escribe un numero" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string leerTexto(string msg) {
    string s;
    while (true) {
        cout << msg;
        if (!getline(cin, s)) exit(0);
        if (s != "") return s;
        cout << "Error: no puede ir vacio" << endl;
    }
}

void mostrarSucursales() {
    for (int i = 0; i < NUM_SUC; i++) {
        cout << i << ") " << sucursales[i] << "  ";
    }
    cout << endl;
}

int leerSucursal(string msg) {
    while (true) {
        int s = leerEntero(msg);
        if (s >= 0 && s < NUM_SUC) return s;
        cout << "Esa sucursal no existe (0 a " << NUM_SUC - 1 << ")" << endl;
    }
}

string nombreEstado(Estado e) {
    if (e == REGISTRADO) return "Registrado";
    if (e == EN_ESPERA) return "En espera";
    return "Despachado";
}

void imprimirPaquete(Paquete p) {
    cout << "Folio: " << p.folio << endl;
    cout << "Descripcion: " << p.descripcion << endl;
    cout << "Peso: " << fixed << setprecision(2) << p.peso << " kg" << endl;
    cout << "Origen: " << sucursales[p.origen] << endl;
    cout << "Destino: " << sucursales[p.destino] << endl;
    cout << "Estado: " << nombreEstado(p.estado) << endl;
}


// ---------- ABB ----------

Nodo* nuevoNodo(Paquete p) {
    Nodo* n = new Nodo;
    n->dato = p;
    n->izq = NULL;
    n->der = NULL;
    return n;
}

// regresa false si el folio ya estaba
bool insertar(Nodo*& nodo, Paquete p) {
    if (nodo == NULL) {
        nodo = nuevoNodo(p);
        return true;
    }
    if (p.folio < nodo->dato.folio)
        return insertar(nodo->izq, p);
    else if (p.folio > nodo->dato.folio)
        return insertar(nodo->der, p);
    return false;
}

Nodo* buscar(Nodo* nodo, int folio) {
    while (nodo != NULL) {
        if (folio == nodo->dato.folio) return nodo;
        if (folio < nodo->dato.folio) nodo = nodo->izq;
        else nodo = nodo->der;
    }
    return NULL;
}

void preorden(Nodo* n) {
    if (n == NULL) return;
    cout << n->dato.folio << " ";
    preorden(n->izq);
    preorden(n->der);
}

void inorden(Nodo* n) {
    if (n == NULL) return;
    inorden(n->izq);
    cout << n->dato.folio << " ";
    inorden(n->der);
}

void postorden(Nodo* n) {
    if (n == NULL) return;
    postorden(n->izq);
    postorden(n->der);
    cout << n->dato.folio << " ";
}

int contarNodos(Nodo* n) {
    if (n == NULL) return 0;
    return 1 + contarNodos(n->izq) + contarNodos(n->der);
}

void borrarArbol(Nodo* n) {
    if (n == NULL) return;
    borrarArbol(n->izq);
    borrarArbol(n->der);
    delete n;
}


// ---------- cola circular ----------

void iniciarCola(Cola& c) {
    c.frente = 0;
    c.final = -1;
    c.cantidad = 0;
}

bool colaVacia(Cola& c) { return c.cantidad == 0; }
bool colaLlena(Cola& c) { return c.cantidad == MAX_COLA; }

bool encolar(Cola& c, int folio) {
    if (colaLlena(c)) return false;
    c.final = (c.final + 1) % MAX_COLA;
    c.datos[c.final] = folio;
    c.cantidad++;
    return true;
}

bool desencolar(Cola& c, int& folio) {
    if (colaVacia(c)) return false;
    folio = c.datos[c.frente];
    c.frente = (c.frente + 1) % MAX_COLA;
    c.cantidad--;
    return true;
}

bool verFrente(Cola& c, int& folio) {
    if (colaVacia(c)) return false;
    folio = c.datos[c.frente];
    return true;
}


// ---------- pila ----------

void iniciarPila(Pila& p) { p.tope = -1; }
bool pilaVacia(Pila& p) { return p.tope == -1; }
bool pilaLlena(Pila& p) { return p.tope == MAX_PILA - 1; }

bool apilar(Pila& p, int folio) {
    if (pilaLlena(p)) return false;
    p.tope++;
    p.datos[p.tope] = folio;
    return true;
}

bool verCima(Pila& p, int& folio) {
    if (pilaVacia(p)) return false;
    folio = p.datos[p.tope];
    return true;
}


// ---------- grafo ----------
// fila = origen, columna = destino

void iniciarGrafo() {
    for (int i = 0; i < NUM_SUC; i++)
        for (int j = 0; j < NUM_SUC; j++)
            grafo[i][j] = 0;

    grafo[0][1] = 1; // Centro -> Norte
    grafo[1][5] = 1; // Norte -> Puerto
    grafo[0][2] = 1; // Centro -> Sur
    grafo[2][3] = 1; // Sur -> Oriente
    grafo[3][0] = 1; // Oriente -> Centro (ciclo)
    grafo[4][0] = 1; // Poniente -> Centro
    // a Poniente no llega nada, para probar el caso sin ruta
}

// BFS, regresa cuantas sucursales tiene la ruta (0 si no hay)
int buscarRuta(int origen, int destino, int ruta[]) {
    bool visitado[NUM_SUC];
    int pred[NUM_SUC];
    int aux[NUM_SUC];
    int ini = 0, fin = 0;

    for (int i = 0; i < NUM_SUC; i++) {
        visitado[i] = false;
        pred[i] = -1;
    }

    visitado[origen] = true;
    aux[fin++] = origen;
    bool encontrado = false;

    while (ini < fin && !encontrado) {
        int actual = aux[ini++];
        for (int v = 0; v < NUM_SUC; v++) {
            if (grafo[actual][v] == 1 && !visitado[v]) {
                visitado[v] = true;
                pred[v] = actual;
                aux[fin++] = v;
                if (v == destino) {
                    encontrado = true;
                    break;
                }
            }
        }
    }

    if (!encontrado) return 0;

    // se arma al reves con los predecesores y luego se voltea
    int temp[NUM_SUC];
    int n = 0;
    for (int v = destino; v != -1; v = pred[v])
        temp[n++] = v;
    for (int i = 0; i < n; i++)
        ruta[i] = temp[n - 1 - i];
    return n;
}

void imprimirRuta(int ruta[], int n) {
    for (int i = 0; i < n; i++) {
        cout << sucursales[ruta[i]];
        if (i < n - 1) cout << " -> ";
    }
    cout << endl;
}


// ---------- radix sort ----------

// se copia en inorden para que ya queden por folio de menor a mayor
void copiarArbol(Nodo* n, Paquete arr[], int& cant) {
    if (n == NULL || cant >= MAX_PAQ) return;
    copiarArbol(n->izq, arr, cant);
    if (cant < MAX_PAQ) arr[cant++] = n->dato;
    copiarArbol(n->der, arr, cant);
}

// el peso lo paso a gramos para trabajar con enteros
long long gramos(Paquete p) {
    return llround(p.peso * 1000);
}

// counting sort por un digito (exp = 1, 10, 100...)
void ordenarPorDigito(Paquete arr[], int n, long long exp) {
    Paquete salida[MAX_PAQ];
    int conteo[10] = {0};

    for (int i = 0; i < n; i++)
        conteo[(gramos(arr[i]) / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        conteo[i] += conteo[i - 1];

    // de atras para adelante para que no se pierda el orden de los empates
    for (int i = n - 1; i >= 0; i--) {
        int d = (gramos(arr[i]) / exp) % 10;
        conteo[d]--;
        salida[conteo[d]] = arr[i];
    }

    for (int i = 0; i < n; i++)
        arr[i] = salida[i];
}

void imprimirArreglo(Paquete arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << "[" << arr[i].folio << ":" << gramos(arr[i]) << "g] ";
    cout << endl;
}

void radixSort(Paquete arr[], int n) {
    if (n <= 1) return;

    long long mayor = gramos(arr[0]);
    for (int i = 1; i < n; i++)
        if (gramos(arr[i]) > mayor) mayor = gramos(arr[i]);

    int pasada = 1;
    for (long long exp = 1; mayor / exp > 0; exp *= 10) {
        ordenarPorDigito(arr, n, exp);
        cout << "Pasada " << pasada << " (exp " << exp << "): ";
        imprimirArreglo(arr, n);
        pasada++;
    }
}


// ---------- opciones del menu ----------

void registrarPaquete() {
    cout << endl << "-- Registrar paquete --" << endl;
    Paquete p;

    p.folio = leerEntero("Folio: ");
    if (p.folio <= 0) {
        cout << "El folio tiene que ser positivo" << endl;
        return;
    }
    if (buscar(raiz, p.folio) != NULL) {
        cout << "Ya existe un paquete con el folio " << p.folio << ", no se registro" << endl;
        return;
    }

    p.descripcion = leerTexto("Descripcion: ");

    p.peso = leerDecimal("Peso (kg): ");
    if (p.peso <= 0) {
        cout << "El peso tiene que ser mayor a 0" << endl;
        return;
    }
    if (p.peso > 100000) {
        cout << "Peso demasiado grande" << endl;
        return;
    }

    mostrarSucursales();
    p.origen = leerSucursal("Origen: ");
    p.destino = leerSucursal("Destino: ");
    if (p.origen == p.destino) {
        cout << "El origen y el destino no pueden ser iguales" << endl;
        return;
    }

    p.estado = REGISTRADO;
    insertar(raiz, p);
    cout << "Paquete registrado" << endl;
}

void buscarPaquete() {
    cout << endl << "-- Buscar paquete --" << endl;
    if (raiz == NULL) {
        cout << "No hay paquetes registrados" << endl;
        return;
    }
    int folio = leerEntero("Folio: ");
    Nodo* n = buscar(raiz, folio);
    if (n == NULL) {
        cout << "No se encontro el folio " << folio << endl;
        return;
    }
    imprimirPaquete(n->dato);
}

void mostrarRecorridos() {
    cout << endl << "-- Recorridos del ABB --" << endl;
    if (raiz == NULL) {
        cout << "El arbol esta vacio" << endl;
        return;
    }
    cout << "Preorden:  ";
    preorden(raiz);
    cout << endl;
    cout << "Inorden:   ";
    inorden(raiz);
    cout << endl;
    cout << "Postorden: ";
    postorden(raiz);
    cout << endl;
}

void agregarACola() {
    cout << endl << "-- Agregar a la cola --" << endl;
    if (raiz == NULL) {
        cout << "No hay paquetes registrados" << endl;
        return;
    }
    int folio = leerEntero("Folio: ");
    Nodo* n = buscar(raiz, folio);
    if (n == NULL) {
        cout << "No existe ese folio" << endl;
        return;
    }
    if (n->dato.estado != REGISTRADO) {
        cout << "Solo se pueden encolar paquetes Registrados (este esta " << nombreEstado(n->dato.estado) << ")" << endl;
        return;
    }
    if (!encolar(cola, folio)) {
        cout << "La cola esta llena, el paquete sigue como Registrado" << endl;
        return;
    }
    n->dato.estado = EN_ESPERA;
    cout << "Folio " << folio << " agregado a la cola (En espera)" << endl;
}

void mostrarPendientes() {
    cout << endl << "-- Paquetes pendientes --" << endl;
    if (colaVacia(cola)) {
        cout << "La cola esta vacia" << endl;
        return;
    }
    cout << "Frente: ";
    for (int i = 0; i < cola.cantidad; i++) {
        int pos = (cola.frente + i) % MAX_COLA;
        cout << cola.datos[pos] << " ";
    }
    cout << " :Final" << endl;
}

void intentarDespacho() {
    cout << endl << "-- Despachar primer paquete --" << endl;
    int folio;
    if (!verFrente(cola, folio)) {
        cout << "No hay paquetes en la cola" << endl;
        return;
    }

    Nodo* n = buscar(raiz, folio);
    if (n == NULL) {
        desencolar(cola, folio);
        return;
    }

    cout << "Folio " << folio << ": " << sucursales[n->dato.origen] << " a " << sucursales[n->dato.destino] << endl;

    int ruta[NUM_SUC];
    int largo = buscarRuta(n->dato.origen, n->dato.destino, ruta);

    if (largo == 0) {
        // no hay ruta, se manda al final
        desencolar(cola, folio);
        encolar(cola, folio);
        n->dato.estado = EN_ESPERA;
        cout << "No hay ruta, el paquete se fue al final de la cola (sigue En espera)" << endl;
        return;
    }

    if (pilaLlena(historial)) {
        cout << "Si hay ruta pero el historial esta lleno, el paquete se queda al frente" << endl;
        return;
    }

    cout << "Ruta: ";
    imprimirRuta(ruta, largo);
    desencolar(cola, folio);
    n->dato.estado = DESPACHADO;
    apilar(historial, folio);
    cout << "Paquete despachado" << endl;
}

void ultimoDespacho() {
    cout << endl << "-- Ultimo despacho --" << endl;
    int folio;
    if (!verCima(historial, folio)) {
        cout << "Todavia no hay despachos" << endl;
        return;
    }
    Nodo* n = buscar(raiz, folio);
    if (n != NULL) imprimirPaquete(n->dato);
}

void mostrarHistorial() {
    cout << endl << "-- Historial de despachos --" << endl;
    if (pilaVacia(historial)) {
        cout << "El historial esta vacio" << endl;
        return;
    }
    // solo recorro el arreglo, no saco nada de la pila
    for (int i = historial.tope; i >= 0; i--) {
        Nodo* n = buscar(raiz, historial.datos[i]);
        cout << historial.datos[i];
        if (n != NULL)
            cout << " - " << n->dato.descripcion << " (" << sucursales[n->dato.origen] << " -> " << sucursales[n->dato.destino] << ")";
        if (i == historial.tope) cout << "   <- cima";
        cout << endl;
    }
}

void agregarConexion() {
    cout << endl << "-- Agregar conexion --" << endl;
    mostrarSucursales();
    int o = leerSucursal("Origen: ");
    int d = leerSucursal("Destino: ");
    if (o == d) {
        cout << "No se puede conectar una sucursal con ella misma" << endl;
        return;
    }
    if (grafo[o][d] == 1) {
        cout << "Esa conexion ya existe" << endl;
        return;
    }
    grafo[o][d] = 1;
    cout << "Conexion agregada: " << sucursales[o] << " -> " << sucursales[d] << endl;
}

void eliminarConexion() {
    cout << endl << "-- Eliminar conexion --" << endl;
    mostrarSucursales();
    int o = leerSucursal("Origen: ");
    int d = leerSucursal("Destino: ");
    if (grafo[o][d] == 0) {
        cout << "Esa conexion no existe" << endl;
        return;
    }
    grafo[o][d] = 0;
    cout << "Conexion eliminada: " << sucursales[o] << " -> " << sucursales[d] << endl;
}

void mostrarMatriz() {
    cout << endl << "-- Matriz de adyacencia --" << endl;
    cout << setw(10) << " ";
    for (int j = 0; j < NUM_SUC; j++)
        cout << setw(9) << sucursales[j];
    cout << endl;

    for (int i = 0; i < NUM_SUC; i++) {
        cout << left << setw(10) << sucursales[i] << right;
        for (int j = 0; j < NUM_SUC; j++)
            cout << setw(9) << grafo[i][j];
        cout << endl;
    }

    cout << endl << left << setw(10) << "Sucursal" << right << setw(10) << "Entrada" << setw(10) << "Salida" << endl;
    for (int i = 0; i < NUM_SUC; i++) {
        int entrada = 0, salida = 0;
        for (int k = 0; k < NUM_SUC; k++) {
            salida += grafo[i][k];
            entrada += grafo[k][i];
        }
        cout << left << setw(10) << sucursales[i] << right << setw(10) << entrada << setw(10) << salida << endl;
    }
}

void reportePeso() {
    cout << endl << "-- Reporte por peso --" << endl;
    if (raiz == NULL) {
        cout << "No hay paquetes registrados" << endl;
        return;
    }

    Paquete arr[MAX_PAQ];
    int n = 0;
    copiarArbol(raiz, arr, n);
    if (contarNodos(raiz) > MAX_PAQ)
        cout << "El arreglo se lleno, solo se copiaron " << MAX_PAQ << " paquetes" << endl;

    cout << "Arreglo original: ";
    imprimirArreglo(arr, n);
    radixSort(arr, n);

    cout << endl;
    cout << left << setw(7) << "Folio" << setw(20) << "Descripcion" << setw(10) << "Peso" << setw(10) << "Origen" << setw(10) << "Destino" << "Estado" << endl;
    for (int i = 0; i < n; i++) {
        cout << left << setw(7) << arr[i].folio
             << setw(20) << arr[i].descripcion.substr(0, 19)
             << setw(10) << fixed << setprecision(2) << arr[i].peso
             << setw(10) << sucursales[arr[i].origen]
             << setw(10) << sucursales[arr[i].destino]
             << nombreEstado(arr[i].estado) << endl;
    }
    cout << right;
}

// 8 paquetes para la demo (40 y 50 pesan lo mismo)
void cargarPrueba() {
    Paquete datos[8] = {
        {50, "Laptop", 2.50, 0, 5, REGISTRADO},
        {30, "Libros", 4.00, 0, 1, REGISTRADO},
        {70, "Refacciones", 7.25, 0, 4, REGISTRADO},
        {20, "Ropa", 1.20, 2, 0, REGISTRADO},
        {40, "Telefono", 2.50, 4, 1, REGISTRADO},
        {60, "Juguetes", 3.10, 1, 5, REGISTRADO},
        {80, "Microondas", 12.00, 3, 2, REGISTRADO},
        {45, "Documentos", 0.80, 5, 0, REGISTRADO}
    };
    for (int i = 0; i < 8; i++)
        insertar(raiz, datos[i]);
    cout << "Paquetes de prueba cargados" << endl;
}

void menu() {
    cout << endl;
    cout << "===== PAQUETERIA =====" << endl;
    cout << "1. Registrar paquete" << endl;
    cout << "2. Buscar paquete por folio" << endl;
    cout << "3. Mostrar recorridos del ABB" << endl;
    cout << "4. Agregar paquete a la cola de despacho" << endl;
    cout << "5. Mostrar paquetes pendientes" << endl;
    cout << "6. Intentar el despacho del primer paquete" << endl;
    cout << "7. Consultar el ultimo despacho" << endl;
    cout << "8. Mostrar historial de despachos" << endl;
    cout << "9. Agregar conexion entre sucursales" << endl;
    cout << "10. Eliminar conexion entre sucursales" << endl;
    cout << "11. Mostrar matriz de adyacencia y grados" << endl;
    cout << "12. Mostrar reporte ordenado por peso" << endl;
    cout << "13. Salir" << endl;
}

int main() {
    iniciarCola(cola);
    iniciarPila(historial);
    iniciarGrafo();

    string r = leerTexto("Cargar paquetes de prueba? (s/n): ");
    if (r == "s" || r == "S") cargarPrueba();

    int op;
    do {
        menu();
        op = leerEntero("Opcion: ");
        switch (op) {
            case 1: registrarPaquete(); break;
            case 2: buscarPaquete(); break;
            case 3: mostrarRecorridos(); break;
            case 4: agregarACola(); break;
            case 5: mostrarPendientes(); break;
            case 6: intentarDespacho(); break;
            case 7: ultimoDespacho(); break;
            case 8: mostrarHistorial(); break;
            case 9: agregarConexion(); break;
            case 10: eliminarConexion(); break;
            case 11: mostrarMatriz(); break;
            case 12: reportePeso(); break;
            case 13: cout << "Adios" << endl; break;
            default: cout << "Opcion no valida" << endl;
        }
    } while (op != 13);

    borrarArbol(raiz);
    return 0;
}
