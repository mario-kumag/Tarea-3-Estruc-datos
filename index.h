/**
 * @file index.h
 * @brief Definiciones y funciones relacionadas con el indice de palabras.
 *
 * Este archivo contiene las estructuras y funciones necesarias para gestionar
 * el indice invertido que relaciona palabras clave con documentos.
 */

#ifndef INDEX_H
#define INDEX_H

#define MAX_DOCS 100 ///< Numero maximo de documentos que pueden ser indexados.
#define MAX_WORDS 1000 ///< Numero maximo de palabras que pueden ser indexadas.

/**
 * @struct NodoIndice
 * @brief Representa un nodo en el indice invertido.
 *
 * Cada nodo contiene una palabra clave, una lista de identificadores de documentos
 * en los que aparece y un puntero a la siguiente palabra en caso de colision.
 */
typedef struct NodoIndice {
    char *palabra; ///< Palabra clave del nodo.
    int docIDs[MAX_DOCS]; ///< Lista de identificadores de documentos donde aparece la palabra.
    int conteoDocs; ///< Numero de documentos en los que aparece la palabra.
    struct NodoIndice *siguiente; ///< Puntero al siguiente nodo en caso de colision.
} NodoIndice;

/**
 * @brief Inicializa la estructura del indice invertido.
 *
 * Establece las estructuras internas en un estado inicial, listas para almacenar datos.
 */
void inicializarIndice();

/**
 * @brief Agrega una palabra al indice junto con su identificador de documento.
 *
 * Si la palabra ya existe en el indice, se agrega el identificador del documento
 * a la lista correspondiente.
 *
 * @param palabra Palabra a indexar.
 * @param docID Identificador del documento donde aparece la palabra.
 */
void agregarPalabraIndice(const char *palabra, int docID);

/**
 * @brief Busca documentos que contienen una palabra clave especifica.
 *
 * Imprime los documentos que contienen la palabra, junto con sus valores de PageRank.
 *
 * @param consulta Palabra clave a buscar en el indice.
 */
void buscarDocumentos(const char *consulta);

/**
 * @brief Determina si una palabra es una stopword.
 *
 * Las stopwords son palabras comunes que no se indexan, como "el", "la", "y".
 *
 * @param palabra Palabra a verificar.
 * @return 1 si la palabra es una stopword, 0 en caso contrario.
 */
int esStopword(const char *palabra);

/**
 * @brief Agrega un documento al sistema.
 *
 * Almacena el nombre del documento y lo registra en el indice global.
 *
 * @param docID Identificador del documento.
 * @param nombre Nombre del archivo del documento.
 */
void agregarDocumento(int docID, const char *nombre);

/**
 * @brief Abre un documento especificado por su nombre.
 *
 * Intenta abrir el documento utilizando el programa asociado por defecto
 * en el sistema operativo.
 *
 * @param nombreArchivo Nombre del archivo del documento.
 */
void abrirDocumento(const char *nombreArchivo);

/**
 * @brief Devuelve el total de palabras indexadas en el sistema.
 *
 * @return Numero de palabras actualmente almacenadas en el indice.
 */
int totalPalabrasIndexadas();

/**
 * @brief Devuelve el total de documentos cargados en el sistema.
 *
 * @return Numero de documentos actualmente registrados en el sistema.
 */
int totalDocumentosCargados();

#endif
