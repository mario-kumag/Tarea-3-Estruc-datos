#ifndef INDEX_H
#define INDEX_H

#define MAX_DOCS 100
#define MAX_WORDS 1000

typedef struct NodoIndice {
    char *palabra;
    int docIDs[MAX_DOCS];
    int conteoDocs;
    struct NodoIndice *siguiente; // Lista enlazada para resolver colisiones
} NodoIndice;

void inicializarIndice();
void agregarPalabraIndice(const char *palabra, int docID);
void buscarDocumentos(const char *consulta);
int esStopword(const char *palabra);
void abrirDocumento(const char *nombreArchivo);


void agregarDocumento(int docID, const char *nombre);

// Declaración de funciones de estadísticas
int totalPalabrasIndexadas();
int totalDocumentosCargados();

#endif
