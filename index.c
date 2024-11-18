#include "index.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "graph.h"

#define HASH_SIZE 100

NodoIndice *tablaHash[HASH_SIZE];
char nombresArchivos[MAX_DOCS][256];
int totalDocs = 0;
int palabrasIndexadas = 0;

void inicializarIndice() {
    for (int i = 0; i < HASH_SIZE; i++) {
        tablaHash[i] = NULL;
    }
}

int calcularHash(const char *palabra) {
    unsigned long hash = 0;
    while (*palabra) {
        hash = (hash * 31) + *palabra++;
    }
    return hash % HASH_SIZE;
}

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

int esStopword(const char *palabra) {
    const char *stopwords[] = {"el", "la", "los", "las", "un", "una", "de", "y", "en", "que", NULL};
    for (int i = 0; stopwords[i]; i++) {
        if (strcmp(stopwords[i], palabra) == 0) {
            return 1;
        }
    }
    return 0;
}

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

    // Preguntar si desea abrir algún documento
    char respuesta;
    printf("¿Desea abrir alguno de estos documentos? (s/n): ");
    scanf(" %c", &respuesta);
    if (respuesta == 's' || respuesta == 'S') {
        for (int i = 0; i < conteoDocumentos; i++) {
            printf("Abriendo el documento '%s'...\n", nombresArchivos[documentosEncontrados[i]]);
            abrirDocumento(nombresArchivos[documentosEncontrados[i]]);
        }
    } else {
        printf("No se abrirán documentos.\n");
    }
}



void agregarDocumento(int docID, const char *nombre) {
    strncpy(nombresArchivos[docID], nombre, sizeof(nombresArchivos[docID]));
    totalDocs++;
}

int totalPalabrasIndexadas() {
    return palabrasIndexadas;
}

int totalDocumentosCargados() {
    return totalDocs;
}


