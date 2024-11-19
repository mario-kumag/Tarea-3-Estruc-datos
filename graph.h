/**
 * @file graph.h
 * @brief Definiciones y funciones relacionadas con la estructura de grafo.
 *
 * Este archivo contiene las estructuras de datos y las funciones necesarias para
 * gestionar el grafo de documentos, incluyendo el calculo de PageRank.
 */

#ifndef GRAPH_H
#define GRAPH_H

#define MAX_DOCS 100 ///< Numero maximo de documentos que puede manejar el grafo.

/**
 * @struct NodoGrafo
 * @brief Representa un nodo en la lista de adyacencia del grafo.
 *
 * Cada nodo almacena el identificador de un documento al que se apunta y un puntero
 * al siguiente nodo en la lista de adyacencia.
 */
typedef struct NodoGrafo {
    int docID; ///< Identificador del documento.
    struct NodoGrafo *siguiente; ///< Puntero al siguiente nodo en la lista de adyacencia.
} NodoGrafo;

/**
 * @struct Grafo
 * @brief Estructura que representa el grafo de documentos.
 *
 * Incluye las listas de adyacencia, los valores de PageRank y el numero de documentos.
 */
typedef struct {
    NodoGrafo *adyacencia[MAX_DOCS]; ///< Listas de adyacencia para cada documento.
    double pageRank[MAX_DOCS]; ///< Valores de PageRank para cada documento.
    int numDocs; ///< Numero total de documentos en el grafo.
} Grafo;

/**
 * @brief Inicializa el grafo con el numero de documentos especificado.
 *
 * @param numDocs Numero de documentos que formaran parte del grafo.
 */
void inicializarGrafo(int numDocs);

/**
 * @brief Agrega un enlace dirigido entre dos documentos en el grafo.
 *
 * @param origen Identificador del documento de origen.
 * @param destino Identificador del documento de destino.
 */
void agregarEnlace(int origen, int destino);

/**
 * @brief Calcula el PageRank de cada documento en el grafo.
 *
 * @param dampingFactor Factor de amortiguamiento utilizado en el calculo.
 * @param iteraciones Numero de iteraciones para refinar los valores de PageRank.
 */
void calcularPageRank(double dampingFactor, int iteraciones);

/**
 * @brief Obtiene el PageRank de un documento especifico.
 *
 * @param docID Identificador del documento.
 * @return Valor del PageRank del documento.
 */
double obtenerPageRank(int docID);

/**
 * @brief Muestra los documentos con los mayores valores de PageRank.
 *
 * @param n Numero de documentos a mostrar.
 */
void mostrarTopPageRank(int n);

#endif
