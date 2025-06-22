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
                    case 1:
                        printf("Ingrese el nombre del archivo CSV para cargar el inventario: ");
                        char nombreArchivo[100];
                        fgets(nombreArchivo, sizeof(nombreArchivo), stdin);
                        nombreArchivo[strcspn(nombreArchivo, "\n")] = 0; // Eliminar el salto de línea 
                        map_reset(productosPorCodigo);
                        map_reset(productosPorNombre);
                        map_reset(productosPorCategoria);
                        cargarInventario(nombreArchivo, productosPorNombre, productosPorCodigo, productosPorCategoria); 
                        break;
                    case 2:
                        limpiarPantalla(); 
                        menuBusqueda();
                        int opcionBusqueda;
                        scanf("%d", &opcionBusqueda);
                        getchar(); // Limpiar buffer
                        switch (opcionBusqueda) {
                            case 1: buscarProductoPorNombre(productosPorNombre); break;
                            case 2: listarProductosPorCategoria(productosPorCategoria); break;
                            //case 3: mostrarProductosStock(productosPorCategoria); break;
                            //case 4: mostrarVentasProductos(productosPorCodigo); break; // Esta función no está implementada
                            default: printf("Opción no válida.\n");
                        }
                        break;
                    case 3: 
                        menuModificarInventario();
                        int opcionModificar;
                        scanf("%d", &opcionModificar);
                        getchar(); // Limpiar buffer
                        switch (opcionModificar) {
                            case 1: registrarProducto(productosPorCodigo, productosPorCategoria, productosPorNombre); break;
                            case 2: modificarStock(productosPorCodigo); break;
                            case 3: eliminarProducto(productosPorCodigo, productosPorCategoria); break;
                            default: printf("Opción no válida.\n");
                        }
                        break;
                    case 4: guardarInventario(productosPorCodigo); break;
                    //case 5: generarReporte(productosPorCodigo); break;
                    default: printf("Opción no válida.\n");
                }
            }
        } else if (tipoUsuario == 2) {
            while (1) {
                mostrarMenuCliente();
                scanf("%d", &opcion);
                getchar(); // Limpiar buffer

                if (opcion == 0) break;

                switch (opcion) {
                    case 1: agregarAlCarrito(productosPorCodigo, carrito); break;
                    case 2: eliminarDelCarrito(carrito); break;
                    case 3: verCarrito(carrito); break;
                    //case 4: confirmarCompra(carrito, grafoCompras); break;
                    default: printf("Opción no válida.\n");
                }
            }
        } else {
            printf("Tipo de usuario no válido.\n");
        }
    }

    // Liberar memoria
    map_clean(productosPorCodigo);
    map_clean(productosPorCategoria);
    //freeGraph(grafoCompras);
    list_clean(carrito);
    return 0;
}