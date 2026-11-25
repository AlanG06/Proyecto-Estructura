// Proyecto final - Estructura de Datos
// Sistema de paqueteria: ABB, cola, pila, grafo y radix sort

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <cstdlib>
using namespace std;

const int N = 6;        // sucursales
const int MAX = 10;     // tamano de la cola y de la pila
const int MAX_PAQ = 100;

string suc[N] = {"Centro", "Norte", "Sur", "Oriente", "Poniente", "Puerto"};

// fila = origen, columna = destino
int grafo[N][N] = {
//   Ce No Su Or Po Pu
    {0, 1, 1, 0, 0, 0},  // Centro
    {0, 0, 0, 0, 0, 1},  // Norte
    {0, 0, 0, 1, 0, 0},  // Sur
    {1, 0, 0, 0, 0, 0},  // Oriente
    {1, 0, 0, 0, 0, 0},  // Poniente
    {0, 0, 0, 0, 0, 0}   // Puerto
};

struct Paquete {
    int folio;
    string desc;
    double peso;
    int origen, destino;
    string estado;
};

struct Nodo {
    Paquete p;
    Nodo* izq;
    Nodo* der;
};

Nodo* raiz = NULL;

int cola[MAX], frente = 0, cantCola = 0;   // cola circular
int pila[MAX], tope = -1;                  // historial


// ---------- lectura ----------

int leerInt(string msg) {
    int x;
    cout << msg;
    while (!(cin >> x)) {
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Numero invalido, otra vez: ";
    }
    cin.ignore(1000, '\n');
    return x;
}

double leerDouble(string msg) {
    double x;
    cout << msg;
    while (!(cin >> x)) {
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Numero invalido, otra vez: ";
    }
    cin.ignore(1000, '\n');
    return x;
}

int leerSucursal(string msg) {
    int s = leerInt(msg);
    while (s < 0 || s >= N)
        s = leerInt("No existe, escribe de 0 a 5: ");
    return s;
}

void listarSucursales() {
    for (int i = 0; i < N; i++)
        cout << i << ") " << suc[i] << "  ";
    cout << endl;
}


// ---------- ABB ----------

bool insertar(Nodo*& n, Paquete p) {
    if (n == NULL) {
        n = new Nodo{p, NULL, NULL};
        return true;
    }
    if (p.folio < n->p.folio) return insertar(n->izq, p);
    if (p.folio > n->p.folio) return insertar(n->der, p);
    return false;   // folio repetido
}

Nodo* buscar(Nodo* n, int folio) {
    while (n != NULL && n->p.folio != folio)
        n = (folio < n->p.folio) ? n->izq : n->der;
    return n;
}

void preorden(Nodo* n)  { if (n) { cout << n->p.folio << " "; preorden(n->izq); preorden(n->der); } }
void inorden(Nodo* n)   { if (n) { inorden(n->izq); cout << n->p.folio << " "; inorden(n->der); } }
void postorden(Nodo* n) { if (n) { postorden(n->izq); postorden(n->der); cout << n->p.folio << " "; } }

void mostrar(Paquete p) {
    cout << "Folio: " << p.folio << endl;
    cout << "Descripcion: " << p.desc << endl;
    cout << "Peso: " << fixed << setprecision(2) << p.peso << " kg" << endl;
    cout << "Ruta: " << suc[p.origen] << " -> " << suc[p.destino] << endl;
    cout << "Estado: " << p.estado << endl;
}


// ---------- cola y pila ----------

bool encolar(int folio) {
    if (cantCola == MAX) return false;
    cola[(frente + cantCola) % MAX] = folio;
    cantCola++;
    return true;
}

int desencolar() {
    int folio = cola[frente];
    frente = (frente + 1) % MAX;
    cantCola--;
    return folio;
}

bool apilar(int folio) {
    if (tope == MAX - 1) return false;
    pila[++tope] = folio;
    return true;
}


// ---------- grafo ----------

// BFS: regresa el numero de sucursales en la ruta, 0 si no hay
int buscarRuta(int origen, int destino, int ruta[]) {
    bool visitado[N] = {false};
    int pred[N], aux[N], ini = 0, fin = 0;
    for (int i = 0; i < N; i++) pred[i] = -1;

    visitado[origen] = true;
    aux[fin++] = origen;
    while (ini < fin) {
        int act = aux[ini++];
        for (int v = 0; v < N; v++) {
            if (grafo[act][v] == 1 && !visitado[v]) {   // visitado evita ciclos
                visitado[v] = true;
                pred[v] = act;
                aux[fin++] = v;
            }
        }
    }
    if (!visitado[destino]) return 0;

    // se recorre de destino a origen con pred y se acomoda al reves
    int largo = 0;
    for (int v = destino; v != -1; v = pred[v]) largo++;
    int i = largo - 1;
    for (int v = destino; v != -1; v = pred[v]) ruta[i--] = v;
    return largo;
}


// ---------- radix sort ----------

int gramos(Paquete p) { return (int)round(p.peso * 1000); }

// inorden -> el arreglo queda por folio de menor a mayor
void copiar(Nodo* n, Paquete arr[], int& k) {
    if (n == NULL || k == MAX_PAQ) return;
    copiar(n->izq, arr, k);
    arr[k++] = n->p;
    copiar(n->der, arr, k);
}

void radixSort(Paquete arr[], int n) {
    int mayor = 0;
    for (int i = 0; i < n; i++)
        if (gramos(arr[i]) > mayor) mayor = gramos(arr[i]);

    Paquete salida[MAX_PAQ];
    for (int exp = 1; mayor / exp > 0; exp *= 10) {
        int conteo[10] = {0};
        for (int i = 0; i < n; i++) conteo[gramos(arr[i]) / exp % 10]++;
        for (int d = 1; d < 10; d++) conteo[d] += conteo[d - 1];
        // de atras hacia adelante para que los empates no cambien de orden
        for (int i = n - 1; i >= 0; i--) salida[--conteo[gramos(arr[i]) / exp % 10]] = arr[i];
        for (int i = 0; i < n; i++) arr[i] = salida[i];

        cout << "Digito " << exp << ": ";
        for (int i = 0; i < n; i++) cout << arr[i].folio << "(" << gramos(arr[i]) << "g) ";
        cout << endl;
    }
}


// ---------- opciones ----------

void registrar() {
    Paquete p;
    p.folio = leerInt("Folio: ");
    if (p.folio <= 0)              { cout << "El folio debe ser positivo" << endl; return; }
    if (buscar(raiz, p.folio))     { cout << "Ese folio ya existe" << endl; return; }
    cout << "Descripcion: ";
    getline(cin, p.desc);
    if (p.desc == "")              { cout << "La descripcion no puede ir vacia" << endl; return; }
    p.peso = leerDouble("Peso (kg): ");
    if (p.peso <= 0)               { cout << "El peso debe ser mayor a 0" << endl; return; }
    listarSucursales();
    p.origen = leerSucursal("Origen: ");
    p.destino = leerSucursal("Destino: ");
    if (p.origen == p.destino)     { cout << "Origen y destino deben ser distintos" << endl; return; }
    p.estado = "Registrado";
    insertar(raiz, p);
    cout << "Paquete registrado" << endl;
}

void buscarPaquete() {
    Nodo* n = buscar(raiz, leerInt("Folio: "));
    if (n) mostrar(n->p);
    else cout << "No existe ese folio" << endl;
}

void recorridos() {
    if (!raiz) { cout << "El arbol esta vacio" << endl; return; }
    cout << "Preorden:  "; preorden(raiz);  cout << endl;
    cout << "Inorden:   "; inorden(raiz);   cout << endl;
    cout << "Postorden: "; postorden(raiz); cout << endl;
}

void agregarCola() {
    Nodo* n = buscar(raiz, leerInt("Folio: "));
    if (!n)                           cout << "No existe ese folio" << endl;
    else if (n->p.estado != "Registrado") cout << "Solo se encolan paquetes Registrados (este esta " << n->p.estado << ")" << endl;
    else if (!encolar(n->p.folio))    cout << "La cola esta llena, sigue como Registrado" << endl;
    else {
        n->p.estado = "En espera";
        cout << "Agregado a la cola" << endl;
    }
}

void pendientes() {
    if (cantCola == 0) { cout << "La cola esta vacia" << endl; return; }
    cout << "Frente -> ";
    for (int i = 0; i < cantCola; i++) cout << cola[(frente + i) % MAX] << " ";
    cout << "<- Final" << endl;
}

void despachar() {
    if (cantCola == 0) { cout << "La cola esta vacia" << endl; return; }
    Paquete& p = buscar(raiz, cola[frente])->p;
    int ruta[N];
    int largo = buscarRuta(p.origen, p.destino, ruta);

    if (largo == 0) {
        encolar(desencolar());   // pasa al final
        cout << "Folio " << p.folio << ": no hay ruta, pasa al final de la cola (En espera)" << endl;
    } else if (tope == MAX - 1) {
        cout << "Hay ruta pero el historial esta lleno, el paquete se queda al frente" << endl;
    } else {
        cout << "Folio " << p.folio << " ruta: ";
        for (int i = 0; i < largo; i++) cout << suc[ruta[i]] << (i < largo - 1 ? " -> " : "");
        cout << endl;
        desencolar();
        apilar(p.folio);
        p.estado = "Despachado";
        cout << "Paquete despachado" << endl;
    }
}

void ultimo() {
    if (tope == -1) cout << "No hay despachos" << endl;
    else mostrar(buscar(raiz, pila[tope])->p);
}

void historialDespachos() {
    if (tope == -1) { cout << "No hay despachos" << endl; return; }
    for (int i = tope; i >= 0; i--)   // solo se lee, la pila no cambia
        cout << pila[i] << (i == tope ? "  <- cima" : "") << endl;
}

void cambiarConexion(int valor) {
    listarSucursales();
    int o = leerSucursal("Origen: ");
    int d = leerSucursal("Destino: ");
    if (o == d)                   cout << "No puede ser la misma sucursal" << endl;
    else if (grafo[o][d] == valor) cout << (valor ? "Esa conexion ya existe" : "Esa conexion no existe") << endl;
    else {
        grafo[o][d] = valor;
        cout << (valor ? "Conexion agregada: " : "Conexion eliminada: ") << suc[o] << " -> " << suc[d] << endl;
    }
}

void matriz() {
    cout << setw(10) << "";
    for (int j = 0; j < N; j++) cout << setw(4) << suc[j].substr(0, 3);
    cout << "   Ent  Sal" << endl;
    for (int i = 0; i < N; i++) {
        int ent = 0, sal = 0;
        cout << left << setw(10) << suc[i] << right;
        for (int j = 0; j < N; j++) {
            cout << setw(4) << grafo[i][j];
            sal += grafo[i][j];   // fila
            ent += grafo[j][i];   // columna
        }
        cout << setw(6) << ent << setw(5) << sal << endl;
    }
}

void reporte() {
    if (!raiz) { cout << "No hay paquetes" << endl; return; }
    Paquete arr[MAX_PAQ];
    int n = 0;
    copiar(raiz, arr, n);
    radixSort(arr, n);
    cout << endl;
    for (int i = 0; i < n; i++)
        cout << left << setw(6) << arr[i].folio << setw(15) << arr[i].desc << right
             << fixed << setprecision(2) << setw(7) << arr[i].peso << " kg   " << arr[i].estado << endl;
}

// 8 paquetes para la demo (40 y 50 pesan igual)
void cargarPrueba() {
    Paquete datos[8] = {
        {50, "Laptop", 2.50, 0, 5, "Registrado"},      // Centro -> Puerto (con escala)
        {30, "Libros", 4.00, 0, 1, "Registrado"},      // Centro -> Norte
        {70, "Refacciones", 7.25, 0, 4, "Registrado"}, // Centro -> Poniente (sin ruta)
        {20, "Ropa", 1.20, 2, 0, "Registrado"},
        {40, "Telefono", 2.50, 4, 1, "Registrado"},
        {60, "Juguetes", 3.10, 1, 5, "Registrado"},
        {80, "Microondas", 12.00, 3, 2, "Registrado"},
        {45, "Documentos", 0.80, 5, 0, "Registrado"}
    };
    for (int i = 0; i < 8; i++) insertar(raiz, datos[i]);
    cout << "Paquetes de prueba cargados" << endl;
}


int main() {
    char r;
    cout << "Cargar paquetes de prueba? (s/n): ";
    cin >> r;
    if (r == 's' || r == 'S') cargarPrueba();

    int op;
    do {
        cout << endl << "===== PAQUETERIA =====" << endl;
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
        op = leerInt("Opcion: ");
        cout << endl;

        switch (op) {
            case 1:  registrar(); break;
            case 2:  buscarPaquete(); break;
            case 3:  recorridos(); break;
            case 4:  agregarCola(); break;
            case 5:  pendientes(); break;
            case 6:  despachar(); break;
            case 7:  ultimo(); break;
            case 8:  historialDespachos(); break;
            case 9:  cambiarConexion(1); break;
            case 10: cambiarConexion(0); break;
            case 11: matriz(); break;
            case 12: reporte(); break;
            case 13: cout << "Adios" << endl; break;
            default: cout << "Opcion no valida" << endl;
        }
    } while (op != 13);
    return 0;
}
