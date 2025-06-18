#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"



void inicializar_lista(ListaProductos *lista) {
    lista->cabeza = NULL;
    lista->cantidad = 0;
}

void agregar_producto_lista(ListaProductos *lista, Producto producto) {
    NodoProducto *nuevo = (NodoProducto*)malloc(sizeof(NodoProducto));
    if (!nuevo) return;
    nuevo->producto = producto;
    nuevo->siguiente = lista->cabeza;
    lista->cabeza = nuevo;
    lista->cantidad++;
}

Producto* buscar_producto_lista(ListaProductos *lista, const char *nombre) {
    NodoProducto *actual = lista->cabeza;
    while (actual) {
        if (strcmp(actual->producto.nombre, nombre) == 0) {
            return &(actual->producto);
        }
        actual = actual->siguiente;
    }
    return NULL;
}

void eliminar_producto_lista(ListaProductos *lista, const char *nombre) {
    NodoProducto *actual = lista->cabeza;
    NodoProducto *anterior = NULL;
    while (actual) {
        if (strcmp(actual->producto.nombre, nombre) == 0) {
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            } else {
                lista->cabeza = actual->siguiente;
            }
            free(actual);
            lista->cantidad--;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}

void liberar_lista(ListaProductos *lista) {
    NodoProducto *actual = lista->cabeza;
    while (actual) {
        NodoProducto *temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    lista->cabeza = NULL;
    lista->cantidad = 0;
}

// Función hash (suma de caracteres módulo capacidad)
int hash(const char *clave, int capacidad) {
    unsigned int hash = 0;
    while (*clave) {
        hash = hash * 31 + (unsigned char)(*clave++);
    }
    return hash % capacidad;
}

MapaProductos* crear_mapa(int capacidad) {
    MapaProductos *mapa = (MapaProductos*)malloc(sizeof(MapaProductos));
    if (!mapa) return NULL;
    mapa->tabla = (NodoMapa**)calloc(capacidad, sizeof(NodoMapa*));
    if (!mapa->tabla) {
        free(mapa);
        return NULL;
    }
    mapa->capacidad = capacidad;
    return mapa;
}

void insertar_mapa(MapaProductos *mapa, const char *clave, Producto *producto) {
    int idx = hash(clave, mapa->capacidad);
    NodoMapa *nuevo = (NodoMapa*)malloc(sizeof(NodoMapa));
    if (!nuevo) return;
    strncpy(nuevo->clave, clave, sizeof(nuevo->clave));
    nuevo->clave[sizeof(nuevo->clave)-1] = '\0';
    nuevo->producto = producto;
    nuevo->siguiente = mapa->tabla[idx];
    mapa->tabla[idx] = nuevo;
}

Producto* buscar_mapa(MapaProductos *mapa, const char *clave) {
    int idx = hash(clave, mapa->capacidad);
    NodoMapa *actual = mapa->tabla[idx];
    while (actual) {
        if (strcmp(actual->clave, clave) == 0) {
            return actual->producto;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

void eliminar_mapa(MapaProductos *mapa, const char *clave) {
    int idx = hash(clave, mapa->capacidad);
    NodoMapa *actual = mapa->tabla[idx];
    NodoMapa *anterior = NULL;
    while (actual) {
        if (strcmp(actual->clave, clave) == 0) {
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            } else {
                mapa->tabla[idx] = actual->siguiente;
            }
            free(actual);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}

void liberar_mapa(MapaProductos *mapa) {
    for (int i = 0; i < mapa->capacidad; i++) {
        NodoMapa *actual = mapa->tabla[i];
        while (actual) {
            NodoMapa *temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
    }
    free(mapa->tabla);
    free(mapa);
}