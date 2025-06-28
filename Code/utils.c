#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "utils.h"
#include "List.h"
#include "Map.h"
#include "App.h"

#define MAX_LINE_LENGTH 4096
#define MAX_FIELDS      128

char **leer_linea_csv(FILE *archivo, char separador) {
    static char linea[MAX_LINE_LENGTH];
    static char *campos[MAX_FIELDS];
    int idx = 0;

    if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL)
        return NULL;  // fin de fichero

    // quitar salto de línea
    linea[strcspn(linea, "\r\n")] = '\0';

    char *ptr = linea;
    while (*ptr && idx < MAX_FIELDS - 1) {
        char *start;

        if (*ptr == '\"') {
            // campo entrecomillado
            ptr++;              // saltar la comilla inicial
            start = ptr;

            // compactar contenido: convertir "" → " y copiar el resto
            char *dest = ptr;
            while (*ptr) {
                if (*ptr == '\"' && *(ptr + 1) == '\"') {
                    *dest++ = '\"';  // una comilla literal
                    ptr += 2;        // saltar ambas
                }
                else if (*ptr == '\"') {
                    ptr++;           // fin del campo
                    break;
                }
                else {
                    *dest++ = *ptr++;
                }
            }
            *dest = '\0';        // terminar cadena

            // ahora ptr apunta justo después de la comilla de cierre
            if (*ptr == separador) ptr++;
        }
        else {
            // campo sin comillas
            start = ptr;
            while (*ptr && *ptr != separador)
                ptr++;
            if (*ptr == separador) {
                *ptr = '\0';
                ptr++;
            }
        }

        campos[idx++] = start;
    }

    campos[idx] = NULL;
    return campos;
}

List *split_string(const char *str, const char *delim) {
  List *result = createList();
  char *token = strtok((char *)str, delim);

  while (token != NULL) {
    // Eliminar espacios en blanco al inicio del token
    while (*token == ' ') {
      token++;
    }

    // Eliminar espacios en blanco al final del token
    char *end = token + strlen(token) - 1;
    while (*end == ' ' && end > token) {
      *end = '\0';
      end--;
    }

    // Copiar el token en un nuevo string
    char *new_token = strdup(token);

    // Agregar el nuevo string a la lista
    pushBackList(result, new_token);

    // Obtener el siguiente token
    token = strtok(NULL, delim);
  }

  return result;
}

void insertarFrecuencia(HashMap *graph, char *nameA, char *nameB){
    
    Pair* parA = searchMap(graph, nameA);
    if(!parA){
        HashMap* relations = createMap(2000);
        insertMap(graph, nameA, relations);
        parA = searchMap(graph, nameA);
    }

    HashMap* relationsA = (HashMap*)parA->value;
    Pair* parB = searchMap(relationsA, nameB);
    if(parB){
       int* frecuency = (int*)parB->value;
       (*frecuency)++;
    }
    else{
        int* newFrecuency = malloc(sizeof(int));
        *newFrecuency = 1;
        insertMap(relationsA, nameB, newFrecuency);
    }
}

void mostrarCarrito(List *carrito) {
    limpiarPantalla();
    Producto *prod = firstList(carrito);
    if (!prod) {
        printf("El carrito está vacío.\n");
        presioneTeclaParaContinuar();
        return;
    }
    printf("=== Carrito de Compras ===\n");
    int idx = 1;
    float total = 0;
    while (prod) {
        float subtotal = prod->precioVenta * prod->stock;
        printf("%d. %s | Marca: %s | Cantidad: %d | Precio unitario: %.2f | Subtotal: %.2f\n",
               idx, prod->nombre, prod->marca, prod->stock, prod->precioVenta, subtotal);
        total += subtotal;
        prod = nextList(carrito);
        idx++;
    }
    printf("-----------------------------\n");
    printf("Total a pagar: %.2f\n", total);
    presioneTeclaParaContinuar();
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void trimWhitespace(char* str) {
    char* end;

    // Eliminar espacios iniciales
    while (isspace((unsigned char)*str)) str++;

    // Eliminar espacios finales
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Escribir el carácter nulo al final
    *(end + 1) = '\0';
}

void puntoCarga() {
    for (int ciclo = 0; ciclo < 3; ciclo++) { // repite el ciclo 3 veces
        for (int puntos = 1; puntos <= 3; puntos++) {
            printf(".");
            fflush(stdout);
            Sleep(500); // 0.5 segundos entre cada punto
        }
        // Borra los 3 puntos
        for (int puntos = 0; puntos < 3; puntos++) {
            printf("\b \b"); // Borra los puntos
            Sleep(100); // Espera antes de imprimir el siguiente ciclo
            fflush(stdout);
        }
    }
}