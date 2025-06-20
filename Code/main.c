#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "List.h"
#include "utils.h"
#include "graph.h"
#include "App.h"


int main() {
    HashMap *productosPorCodigo = createMap(2000);
    HashMap *productosPorNombre = createMap(2000);
    HashMap *productosPorCategoria = createMap(2000);
    //Graph *grafoCompras = createGraph();
    List *carrito = list_create();
    
    int opcion, tipoUsuario;
    while (1) {
        printf("\n=== Sistema de Gestión de Inventario ===\n");
        printf("1. Administrador\n2. Cliente\n0. Salir\n");
        printf("Seleccione tipo de usuario: ");
        scanf("%d", &tipoUsuario);
        getchar(); // Limpiar buffer

        if (tipoUsuario == 0) break;

        if (tipoUsuario == 1) {
            while (1) {
                mostrarMenuAdministrador();
                scanf("%d", &opcion);
                getchar(); // Limpiar buffer

                if (opcion == 0) break;

                switch (opcion) {
                    case 1: registrarProducto(productosPorCodigo, productosPorCategoria, productosPorNombre); break;
                    case 2: buscarProductoPorNombre(productosPorNombre); break;
                    case 3: listarProductosPorCategoria(productosPorCategoria); break;
                    case 4: modificarStock(productosPorCodigo); break;
                    case 5: mostrarProductosStockBajo(productosPorCategoria); break;
                    case 6: eliminarProducto(productosPorCodigo, productosPorCategoria); break;
                    case 7: guardarInventario(productosPorCodigo); break;
                    case 8: cargarInventario(productosPorNombre, productosPorCodigo ,productosPorCategoria); break;
                    //case 9: generarReporte(productosPorCodigo); break;
                    default: printf("Opción no válida.\n");
                }
            }
        }/* else if (tipoUsuario == 2) {
            while (1) {
                mostrarMenuCliente();
                scanf("%d", &opcion);
                getchar(); // Limpiar buffer

                if (opcion == 5) break;

                switch (opcion) {
                    case 1: agregarAlCarrito(productosPorCodigo, carrito); break;
                    case 2: eliminarDelCarrito(carrito); break;
                    case 3: verCarrito(carrito); break;
                    case 4: confirmarCompra(carrito, grafoCompras); break;
                    default: printf("Opción no válida.\n");
                }
            }
        } */else {
            printf("Tipo de usuario no válido.\n");
        }
    }

    // Liberar memoria
    map_clean(productosPorCodigo);
    map_clean(productosPorCategoria);
    //freeGraph(grafoCompras);
    list_clean(carrito);
    //hola como estas
    return 0;
}