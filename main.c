/**
 * @file main.c
 * @brief Programa principal para el motor de busqueda.
 *
 * Este archivo contiene la logica principal del programa, incluyendo
 * la inicializacion, carga de archivos, procesamiento de datos y menu principal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "index.h"
#include "graph.h"
#include "utils.h"

/**
 * @brief Carga archivos desde un directorio al indice y al grafo.
 *
 * Procesa todos los archivos con extension .txt del directorio especificado,
 * agrega su contenido al indice y sus enlaces al grafo.
 *
 * @param directorio Ruta al directorio que contiene los archivos.
 */
void cargarArchivosEnIndiceYGrafo(const char *directorio);

/**
 * @brief Muestra estadisticas del sistema.
 *
 * Imprime la cantidad total de palabras indexadas, documentos cargados
 * y los documentos con mayor PageRank.
 */
void mostrarEstadisticas();

/**
 * @brief Menu principal del sistema.
 *
 * Permite al usuario interactuar con el sistema, realizar busquedas,
 * consultar estadisticas y recalcular el PageRank.
 */
void menuPrincipal();

/**
 * @brief Abre un documento especificado.
 *
 * Utiliza el sistema operativo para abrir el archivo con la aplicacion por defecto.
 *
 * @param nombreArchivo Nombre o ruta del archivo a abrir.
 */
void abrirDocumento(const char *nombreArchivo);

/**
 * @brief Funcion principal.
 *
 * Inicializa el indice y el grafo, carga los archivos desde el directorio "docs",
 * calcula el PageRank inicial y lanza el menu principal.
 *
 * @return 0 si el programa termina correctamente.
 */
int main() {
    inicializarIndice();
    inicializarGrafo(MAX_DOCS);

    cargarArchivosEnIndiceYGrafo("docs");

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

    printf("Leyendo archivos de la carpeta: %s\n", directorio);

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt") != NULL) {
            char rutaArchivo[512];
            snprintf(rutaArchivo, sizeof(rutaArchivo), "%s/%s", directorio, entry->d_name);

            printf("Procesando archivo: %s\n", rutaArchivo);

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
    printf("\n--- Estadisticas del Sistema ---\n");
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
    int status = system(comando);
    if (status == -1) {
        printf("Error al intentar abrir el documento '%s'.\n", nombreArchivo);
    }
}

void menuPrincipal() {
    int opcion;
    char consulta[100];
    do {
        printf("\n--- Motor de Busqueda ---\n");
        printf("1. Buscar documentos por palabra clave\n");
        printf("2. Mostrar estadisticas del sistema\n");
        printf("3. Recalcular PageRank\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            printf("Error al leer la opcion. Intente nuevamente.\n");
            opcion = -1; // Asignar valor invalido
            while (getchar() != '\n'); // Limpiar el buffer de entrada
        }

        while (getchar() != '\n'); // Limpieza adicional del buffer por seguridad

        switch (opcion) {
            case 1:
                printf("Ingrese una palabra o frase para buscar: ");
                if (fgets(consulta, sizeof(consulta), stdin) == NULL) {
                    printf("Error al leer la consulta. Intente nuevamente.\n");
                    consulta[0] = '\0'; // Evitar procesar una consulta invalida
                } else {
                    consulta[strcspn(consulta, "\n")] = 0; // Eliminar salto de linea
                    convertirAMinusculas(consulta);
                    buscarDocumentos(consulta);
                }
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
                printf("Opcion no valida. Intente nuevamente.\n");
        }
    } while (opcion != 4);
}
