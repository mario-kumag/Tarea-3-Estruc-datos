#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "index.h"
#include "graph.h"
#include "utils.h"

void cargarArchivosEnIndiceYGrafo(const char *directorio);
void mostrarEstadisticas();
void menuPrincipal();
void abrirDocumento(const char *nombreArchivo);

int main() {
    inicializarIndice();
    inicializarGrafo(MAX_DOCS);

    cargarArchivosEnIndiceYGrafo(".");

    calcularPageRank(0.85, 20);

    menuPrincipal();
    return 0;
}

void cargarArchivosEnIndiceYGrafo(const char *directorio) {
    DIR *dir;
    struct dirent *entry;
    int docID = 0;

    if ((dir = opendir(directorio)) == NULL) {
        perror("No se pudo abrir el directorio");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt") != NULL) {
            char rutaArchivo[512];
            snprintf(rutaArchivo, sizeof(rutaArchivo), "%s/%s", directorio, entry->d_name);

            FILE *archivo = fopen(rutaArchivo, "r");
            if (!archivo) {
                perror("No se pudo abrir el archivo");
                continue;
            }

            agregarDocumento(docID, rutaArchivo);
            char palabra[100];
            while (fscanf(archivo, "%99s", palabra) == 1) {
                convertirAMinusculas(palabra);
                if (!esStopword(palabra)) {
                    agregarPalabraIndice(palabra, docID);
                }
                if (strncmp(palabra, "link:", 5) == 0) {
                    int enlaceID = atoi(palabra + 5);
                    agregarEnlace(docID, enlaceID);
                }
            }
            fclose(archivo);
            docID++;
        }
    }
    closedir(dir);
}

void mostrarEstadisticas() {
    printf("\n--- Estadísticas del Sistema ---\n");
    printf("Total de palabras indexadas: %d\n", totalPalabrasIndexadas());
    printf("Total de documentos: %d\n", totalDocumentosCargados());
    printf("Top 5 documentos por PageRank:\n");
    mostrarTopPageRank(5);
    printf("--------------------------------\n");
}

void abrirDocumento(const char *nombreArchivo) {
    printf("Abriendo el documento '%s'...\n", nombreArchivo);
    char comando[512];
    snprintf(comando, sizeof(comando), "xdg-open \"%s\" 2>/dev/null || open \"%s\" 2>/dev/null || start \"%s\"", 
             nombreArchivo, nombreArchivo, nombreArchivo);
    system(comando);
}

void menuPrincipal() {
    int opcion;
    char consulta[100];
    do {
        printf("\n--- Motor de Búsqueda ---\n");
        printf("1. Buscar documentos por palabra clave\n");
        printf("2. Mostrar estadísticas del sistema\n");
        printf("3. Recalcular PageRank\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar(); // Limpia el buffer de entrada

        switch (opcion) {
            case 1:
                printf("Ingrese una palabra o frase para buscar: ");
                fgets(consulta, sizeof(consulta), stdin);
                consulta[strcspn(consulta, "\n")] = 0; // Eliminar el salto de línea
                convertirAMinusculas(consulta);
                buscarDocumentos(consulta);
                break;
            case 2:
                mostrarEstadisticas();
                break;
            case 3:
                calcularPageRank(0.85, 20);
                printf("PageRank recalculado.\n");
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Intente nuevamente.\n");
        }
    } while (opcion != 4);
}
