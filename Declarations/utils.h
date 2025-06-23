#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"

char **leer_linea_csv(FILE *archivo, char separador);

List *split_string(const char *str, const char *delim);

// Función para limpiar la pantalla
void limpiarPantalla();

void presioneTeclaParaContinuar();

void insertarFrecuencia(HashMap *graph, char *nameA, char *nameB);
#endif /* UTILS_H */