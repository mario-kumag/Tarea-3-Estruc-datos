/**
 * @file index.c
 * @brief Implementacion del modulo de indice invertido.
 *
 * Este modulo gestiona el indice invertido para asociar palabras con documentos,
 * permite agregar documentos al sistema y buscar documentos por palabras clave.
 */

#include "index.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "graph.h"

#define HASH_SIZE 100 ///< Tamaño de la tabla hash utilizada para el indice invertido.

NodoIndice *tablaHash[HASH_SIZE]; ///< Tabla hash para almacenar las palabras y sus documentos asociados.
char nombresArchivos[MAX_DOCS][256]; ///< Almacena los nombres de los documentos cargados.
int totalDocs = 0; ///< Contador del total de documentos cargados.
int palabrasIndexadas = 0; ///< Contador del total de palabras indexadas.

/**
 * @brief Inicializa el indice invertido.
 *
 * Establece todas las posiciones de la tabla hash en NULL.
 */
void inicializarIndice() {
    for (int i = 0; i < HASH_SIZE; i++) {
        tablaHash[i] = NULL;
    }
}

/**
 * @brief Calcula el hash de una palabra.
 *
 * Utiliza una funcion de hash basada en multiplicacion por 31 para obtener
 * una posicion en la tabla hash.
 *
 * @param palabra Palabra a la cual calcular el hash.
 * @return Posicion en la tabla hash.
 */
int calcularHash(const char *palabra) {
    unsigned long hash = 0;
    while (*palabra) {
        hash = (hash * 31) + *palabra++;
    }
    return hash % HASH_SIZE;
}

/**
 * @brief Agrega una palabra al indice invertido.
 *
 * Si la palabra ya existe en el indice, se agrega el identificador del documento
 * al conjunto de documentos asociados a la palabra. Si no, se crea una nueva entrada.
 *
 * @param palabra Palabra a agregar.
 * @param docID Identificador del documento donde aparece la palabra.
 */
void agregarPalabraIndice(const char *palabra, int docID) {
    int hash = calcularHash(palabra);
    NodoIndice *actual = tablaHash[hash];

    while (actual) {
        if (strcmp(actual->palabra, palabra) == 0) {
            actual->docIDs[actual->conteoDocs++] = docID;
            return;
        }
        actual = actual->siguiente;
    }

    NodoIndice *nuevoNodo = malloc(sizeof(NodoIndice));
    nuevoNodo->palabra = strdup(palabra);
    nuevoNodo->docIDs[0] = docID;
    nuevoNodo->conteoDocs = 1;
    nuevoNodo->siguiente = tablaHash[hash];
    tablaHash[hash] = nuevoNodo;
    palabrasIndexadas++;
}

/**
 * @brief Verifica si una palabra es una stopword.
 *
 * Comprueba si la palabra pertenece a una lista de palabras comunes que no
 * deben ser indexadas.
 *
 * @param palabra Palabra a verificar.
 * @return 1 si es una stopword, 0 en caso contrario.
 */
int esStopword(const char *palabra) {
    const char *stopwords[] = {"el", "la", "los", "las", "un", "una", "de", "y", "en", "que", NULL};
    for (int i = 0; stopwords[i]; i++) {
        if (strcmp(stopwords[i], palabra) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Busca documentos que contienen una palabra especifica.
 *
 * Muestra los documentos que contienen la palabra buscada junto con su PageRank.
 * Permite al usuario abrir los documentos encontrados.
 *
 * @param consulta Palabra a buscar.
 */
void buscarDocumentos(const char *consulta) {
    int hash = calcularHash(consulta);
    NodoIndice *actual = tablaHash[hash];
    int documentosEncontrados[MAX_DOCS];
    int conteoDocumentos = 0;

    // Buscar documentos que contienen la palabra
    while (actual) {
        if (strcmp(actual->palabra, consulta) == 0) {
            printf("Resultados para la palabra '%s':\n", consulta);
            for (int i = 0; i < actual->conteoDocs; i++) {
                int docID = actual->docIDs[i];
                documentosEncontrados[conteoDocumentos++] = docID;
                printf(" - Documento: %s (PageRank: %.4f)\n", nombresArchivos[docID], obtenerPageRank(docID));
            }
            break;
        }
        actual = actual->siguiente;
    }

    if (conteoDocumentos == 0) {
        printf("La palabra '%s' no fue encontrada.\n", consulta);
        return;
    }

    // Preguntar si desea abrir algun documento
    char respuesta;
    printf("¿Desea abrir alguno de estos documentos? (s/n): ");
    if (scanf(" %c", &respuesta) != 1) {
        printf("Error al leer la respuesta. Asumiendo 'n'.\n");
        respuesta = 'n';
    }
    if (respuesta == 's' || respuesta == 'S') {
        for (int i = 0; i < conteoDocumentos; i++) {
            printf("Abriendo el documento '%s'...\n", nombresArchivos[documentosEncontrados[i]]);
            abrirDocumento(nombresArchivos[documentosEncontrados[i]]);
        }
    } else {
        printf("No se abriran documentos.\n");
    }
}

/**
 * @brief Agrega un documento al sistema.
 *
 * Almacena el nombre del documento y actualiza el contador total de documentos.
 *
 * @param docID Identificador del documento.
 * @param nombre Nombre del archivo del documento.
 */
void agregarDocumento(int docID, const char *nombre) {
    strncpy(nombresArchivos[docID], nombre, sizeof(nombresArchivos[docID]));
    totalDocs++;
}

/**
 * @brief Obtiene el total de palabras indexadas.
 *
 * @return Total de palabras indexadas en el sistema.
 */
int totalPalabrasIndexadas() {
    return palabrasIndexadas;
}

/**
 * @brief Obtiene el total de documentos cargados.
 *
 * @return Total de documentos cargados en el sistema.
 */
int totalDocumentosCargados() {
    return totalDocs;
}
