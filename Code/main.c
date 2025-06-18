#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "List.h"
#include "utils.h"
#include "graph.h"

void mostrarMenuAdministrador() {
    printf("\n=== Menú Administrador ===\n");
    printf("1. Registrar nuevo producto\n");
    printf("2. Buscar producto por nombre\n");
    printf("3. Listar productos por categoría\n");
    printf("4. Modificar stock de producto\n");
    printf("5. Mostrar productos con stock bajo\n");
    printf("6. Eliminar producto\n");
    printf("7. Guardar inventario\n");
    printf("8. Cargar inventario\n");
    printf("9. Generar reporte completo\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

void mostrarMenuCliente() {
    printf("\n=== Menú Cliente ===\n");
    printf("1. Agregar al Carrito\n");
    printf("2. Eliminar del Carrito\n");
    printf("3. Ver Carrito (Productos)\n");
    printf("4. Confirmar Compra\n");
    printf("5. Salir\n");
    printf("Seleccione una opción: ");
}

int main() {
    HashMap *productosPorNombre = createMap(1000000);
    HashMap *productosPorCategoria = createMap(1000000);
    Graph *grafoCompras = createGraph();
    List *carrito = createList();
    
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
                    case 1: registrarProducto(productosPorNombre, productosPorCategoria); break;
                    case 2: buscarProductoPorNombre(productosPorNombre); break;
                    case 3: listarProductosPorCategoria(productosPorCategoria); break;
                    case 4: modificarStock(productosPorNombre); break;
                    case 5: mostrarProductosStockBajo(productosPorCategoria); break;
                    case 6: eliminarProducto(productosPorNombre, productosPorCategoria); break;
                    case 7: guardarInventario(productosPorNombre); break;
                    case 8: cargarInventario(productosPorNombre, productosPorCategoria); break;
                    case 9: generarReporte(productosPorNombre); break;
                    default: printf("Opción no válida.\n");
                }
            }
        } else if (tipoUsuario == 2) {
            while (1) {
                mostrarMenuCliente();
                scanf("%d", &opcion);
                getchar(); // Limpiar buffer

                if (opcion == 5) break;

                switch (opcion) {
                    case 1: agregarAlCarrito(productosPorNombre, carrito); break;
                    case 2: eliminarDelCarrito(carrito); break;
                    case 3: verCarrito(carrito); break;
                    case 4: confirmarCompra(carrito, grafoCompras); break;
                    default: printf("Opción no válida.\n");
                }
            }
        } else {
            printf("Tipo de usuario no válido.\n");
        }
    }

    // Liberar memoria
    freeMap(productosPorNombre);
    freeMap(productosPorCategoria);
    freeGraph(grafoCompras);
    freeList(carrito);

    return 0;
}