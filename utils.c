/**
 * @file utils.c
 * @brief Utilidades generales para el sistema de manejo de documentos.
 *
 * Este archivo contiene funciones auxiliares que se utilizan en diversas partes
 * del sistema, como la conversion de cadenas a minusculas.
 */

#include "utils.h"
#include <ctype.h>

/**
 * @brief Convierte una cadena de caracteres a minusculas.
 *
 * Recorre cada caracter de la cadena y lo transforma a su equivalente en minuscula.
 * Modifica la cadena en su lugar.
 *
 * @param str Puntero a la cadena que se desea convertir.
 */
void convertirAMinusculas(char *str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}
