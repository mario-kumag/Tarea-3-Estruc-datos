#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Grafo grafo;

void inicializarGrafo(int numDocs) {
    grafo.numDocs = numDocs;
    for (int i = 0; i < numDocs; i++) {
        grafo.adyacencia[i] = NULL;
        grafo.pageRank[i] = 0.0;
    }
}

void agregarEnlace(int origen, int destino) {
    NodoGrafo *nuevo = malloc(sizeof(NodoGrafo));
    nuevo->docID = destino;
    nuevo->siguiente = grafo.adyacencia[origen];
    grafo.adyacencia[origen] = nuevo;
}

void calcularPageRank(double dampingFactor, int iteraciones) {
    for (int i = 0; i < grafo.numDocs; i++) {
        grafo.pageRank[i] = 1.0 / grafo.numDocs;
    }

    for (int iter = 0; iter < iteraciones; iter++) {
        double nuevoPageRank[MAX_DOCS] = {0};
        for (int i = 0; i < grafo.numDocs; i++) {
            NodoGrafo *nodo = grafo.adyacencia[i];
            while (nodo) {
                int destino = nodo->docID;
                nuevoPageRank[destino] += grafo.pageRank[i];
                nodo = nodo->siguiente;
            }
        }

        for (int i = 0; i < grafo.numDocs; i++) {
            grafo.pageRank[i] = dampingFactor * nuevoPageRank[i] + (1 - dampingFactor) / grafo.numDocs;
        }
    }
}

double obtenerPageRank(int docID) {
    return grafo.pageRank[docID];
}

void mostrarTopPageRank(int n) {
    struct {
        double pageRank;
        int docID;
    } documentos[MAX_DOCS];

    // Copiar valores en un arreglo temporal
    for (int i = 0; i < grafo.numDocs; i++) {
        documentos[i].pageRank = grafo.pageRank[i];
        documentos[i].docID = i;
    }

    // Ordenar los documentos por PageRank (burbuja por simplicidad)
    for (int i = 0; i < grafo.numDocs - 1; i++) {
        for (int j = 0; j < grafo.numDocs - i - 1; j++) {
            if (documentos[j].pageRank < documentos[j + 1].pageRank) {
                // Intercambiar
                double tempPageRank = documentos[j].pageRank;
                int tempDocID = documentos[j].docID;
                documentos[j].pageRank = documentos[j + 1].pageRank;
                documentos[j].docID = documentos[j + 1].docID;
                documentos[j + 1].pageRank = tempPageRank;
                documentos[j + 1].docID = tempDocID;
            }
        }
    }

    // Mostrar los top `n` documentos
    printf("\n--- Top %d Documentos por PageRank ---\n", n);
    for (int i = 0; i < n && i < grafo.numDocs; i++) {
        printf("Documento %d: PageRank = %.4f\n", documentos[i].docID, documentos[i].pageRank);
    }
    printf("------------------------------------\n");
}
