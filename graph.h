#ifndef GRAPH_H
#define GRAPH_H

#define MAX_DOCS 100

typedef struct NodoGrafo {
    int docID;
    struct NodoGrafo *siguiente;
} NodoGrafo;

typedef struct {
    NodoGrafo *adyacencia[MAX_DOCS];
    double pageRank[MAX_DOCS];
    int numDocs;
} Grafo;

void inicializarGrafo(int numDocs);
void agregarEnlace(int origen, int destino);
void calcularPageRank(double dampingFactor, int iteraciones);
double obtenerPageRank(int docID);

// Declaración de función para mostrar el top PageRank
void mostrarTopPageRank(int n);

#endif
