#include "utils.h"
#include <ctype.h>

void convertirAMinusculas(char *str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}
