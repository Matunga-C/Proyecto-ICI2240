#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "List.h"
#include "utils.h"
#include "graph.h"

typedef struct {
    char nombre[51];
    char categoria[51];
    char codigoBarras[51];
    int stock;
    float precioVenta;
    float precioMercado;
    float precioCosto;
    int vendidos;
    char marca[51];
} Producto;



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

void cargarInventario(HashMap *productosPorNombre, HashMap *productosPorCodigo , HashMap *productosPorCategoria) {
    limpiarPantalla();
    FILE *file = fopen("productos_supermercado_completo.csv", "r");
    if (file == NULL) {
        perror(
            "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
        return;
      }
    
    char **campos;
    campos = leer_linea_csv(file, ',');

    while ((campos = leer_linea_csv(file, ',')) != NULL) {
        Producto *producto = malloc(sizeof(Producto));
        strcpy(producto->nombre, campos[1]); // Almacena el id de la funckion utilizando la funcion strcpy para copiar el string       
        strcpy(producto->marca, campos[2]); // Almacena el nombre del album utilizando la funcion strcpy para copiar el string     
        strcpy(producto->categoria, campos[3]); // Almacena el nombre de la cancion utilizando la funcion strcpy para copiar el string
        strcpy(producto->codigoBarras, campos[8]);
        producto->stock = atoi(campos[7]); // Convierte el stock a entero
        producto->precioVenta = atof(campos[4]); // Convierte el precio de venta a float
        producto->precioMercado = atof(campos[5]); // Convierte el precio de mercado a float
        producto->precioCosto = atof(campos[6]); // Convierte el precio de costo a float 

        insertMap(productosPorNombre, producto->nombre, producto);
        insertMap(productosPorCodigo, producto->codigoBarras, producto);
        insertMap(productosPorCategoria, producto->categoria, producto);
    }
    
    fclose(file);
    printf("Inventario cargado exitosamente.\n");
}

int main() {
    HashMap *productosPorCodigo = createMap(1000000);
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
                    case 1: registrarProducto(productosPorCodigo, productosPorCategoria); break;
                    case 2: buscarProductoPorNombre(productosPorCodigo); break;
                    case 3: listarProductosPorCategoria(productosPorCategoria); break;
                    case 4: modificarStock(productosPorCodigo); break;
                    case 5: mostrarProductosStockBajo(productosPorCategoria); break;
                    case 6: eliminarProducto(productosPorCodigo, productosPorCategoria); break;
                    case 7: guardarInventario(productosPorCodigo); break;
                    case 8: cargarInventario(productosPorNombre,productosPorCodigo ,productosPorCategoria); break;
                    case 9: generarReporte(productosPorCodigo); break;
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
                    case 1: agregarAlCarrito(productosPorCodigo, carrito); break;
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
    freeMap(productosPorCodigo);
    freeMap(productosPorCategoria);
    freeGraph(grafoCompras);
    freeList(carrito);
    //hola como estas
    return 0;
}