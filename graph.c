/**
 * @file graph.c
 * @brief Implementacion del modulo de grafo para el sistema de documentos.
 *
 * Este modulo gestiona la representacion del grafo de documentos y calcula el
 * PageRank de cada documento basado en los enlaces entre ellos.
 */

#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Grafo grafo; ///< Estructura global que representa el grafo.

/**
 * @brief Inicializa el grafo con un numero especifico de documentos.
 *
 * Asigna espacio para las listas de adyacencia y establece los valores iniciales de PageRank.
 *
 * @param numDocs Numero de documentos que formaran parte del grafo.
 */
void inicializarGrafo(int numDocs) {
    grafo.numDocs = numDocs;
    for (int i = 0; i < numDocs; i++) {
        grafo.adyacencia[i] = NULL;
        grafo.pageRank[i] = 0.0;
    }
}

/**
 * @brief Agrega un enlace dirigido en el grafo entre dos documentos.
 *
 * Crea un enlace del documento de origen al documento de destino en la lista de adyacencia.
 *
 * @param origen Identificador del documento de origen.
 * @param destino Identificador del documento de destino.
 */
void agregarEnlace(int origen, int destino) {
    NodoGrafo *nuevo = malloc(sizeof(NodoGrafo));
    nuevo->docID = destino;
    nuevo->siguiente = grafo.adyacencia[origen];
    grafo.adyacencia[origen] = nuevo;
}

/**
 * @brief Calcula el PageRank de cada documento en el grafo.
 *
 * Utiliza el metodo iterativo de PageRank con un factor de amortiguamiento y un numero fijo de iteraciones.
 *
 * @param dampingFactor Factor de amortiguamiento utilizado en el calculo.
 * @param iteraciones Numero de iteraciones para refinar los valores de PageRank.
 */
void calcularPageRank(double dampingFactor, int iteraciones) {
    // Inicializar PageRank uniforme
    for (int i = 0; i < grafo.numDocs; i++) {
        grafo.pageRank[i] = 1.0 / grafo.numDocs;
    }

    // Iteraciones de refinamiento
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

/**
 * @brief Obtiene el PageRank de un documento especifico.
 *
 * @param docID Identificador del documento.
 * @return Valor del PageRank del documento.
 */
double obtenerPageRank(int docID) {
    return grafo.pageRank[docID];
}

/**
 * @brief Muestra los documentos con los mayores valores de PageRank.
 *
 * Ordena los documentos por PageRank y muestra los n primeros.
 *
 * @param n Numero de documentos a mostrar.
 */
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

    // Ordenar los documentos por PageRank (ordenamiento burbuja)
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
