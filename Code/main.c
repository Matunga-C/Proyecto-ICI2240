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

void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
    puts("Presione una tecla para continuar...");
    getchar(); // Consume el '\n' del buffer de entrada
    getchar(); // Espera a que el usuario presione una tecla
}

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
    FILE *file = fopen("productos_supermercado_500_limpio.csv", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }
    char **campos;
    // Leer encabezado
    campos = leer_linea_csv(file, ',');

    while ((campos = leer_linea_csv(file, ',')) != NULL) {
        // Verifica que existan todos los campos necesarios
        if (!campos[1] || !campos[2] || !campos[3] || !campos[4] ||
            !campos[5] || !campos[6] || !campos[7] || !campos[8]) {
            // Si tu función leer_linea_csv usa memoria dinámica, libera aquí
            // free(campos);
            continue;
        }

        Producto *producto = malloc(sizeof(Producto));
        if (!producto) {
            perror("No se pudo asignar memoria para producto");
            // free(campos);
            continue;
        }

        strncpy(producto->nombre, campos[1], sizeof(producto->nombre) - 1);
        producto->nombre[sizeof(producto->nombre) - 1] = '\0';

        strncpy(producto->marca, campos[2], sizeof(producto->marca) - 1);
        producto->marca[sizeof(producto->marca) - 1] = '\0';

        strncpy(producto->categoria, campos[3], sizeof(producto->categoria) - 1);
        producto->categoria[sizeof(producto->categoria) - 1] = '\0';

        strncpy(producto->codigoBarras, campos[8], sizeof(producto->codigoBarras) - 1);
        producto->codigoBarras[sizeof(producto->codigoBarras) - 1] = '\0';

        producto->stock = atoi(campos[7]);
        producto->precioVenta = atof(campos[4]);
        producto->precioMercado = atof(campos[5]);
        producto->precioCosto = atof(campos[6]);
        producto->vendidos = 0;

        // Insertar en productosPorCodigo
        if (searchMap(productosPorCodigo, producto->codigoBarras) == NULL) {
            insertMap(productosPorCodigo, producto->codigoBarras, producto);
        }

        // Insertar en productosPorNombre
        Pair *pairNombre = searchMap(productosPorNombre, producto->nombre);
        if (pairNombre == NULL) {
            List *listaProductos = list_create();
            list_pushBack(listaProductos, producto);
            insertMap(productosPorNombre, producto->nombre, listaProductos);
        } else {
            List *listaProductos = (List *)pairNombre->value;
            list_pushBack(listaProductos, producto);
        }

        // Insertar en productosPorCategoria
        Pair *pairCategoria = searchMap(productosPorCategoria, producto->categoria);
        if (pairCategoria == NULL) {
            List *listaProductosCategoria = list_create();
            list_pushBack(listaProductosCategoria, producto);
            insertMap(productosPorCategoria, producto->categoria, listaProductosCategoria);
        } else {
            List *listaProductosCategoria = (List *)pairCategoria->value;
            list_pushBack(listaProductosCategoria, producto);
        }

        // Si tu función leer_linea_csv usa memoria dinámica, libera aquí
        // free(campos);
    }
    puts("Inventario cargado exitosamente.");
    fclose(file);
}

void buscarProductoPorNombre(HashMap *productosPorNombre) {
    limpiarPantalla();
    char nombre[51];
    printf("Ingrese el nombre del producto a buscar: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    Pair *pair = searchMap(productosPorNombre, nombre);
    if (pair != NULL) {
        List *listaProductos = (List *)pair->value;
        Producto *producto = (Producto *)list_first(listaProductos);
        if (!producto) {
            puts("No se encontraron productos con ese nombre.");
            return;
        }
        while (producto != NULL) {
            printf("\n");
            printf("Nombre: %s\n", producto->nombre);
            printf("Marca: %s\n", producto->marca);
            printf("Categoría: %s\n", producto->categoria);
            printf("Código de barras: %s\n", producto->codigoBarras);
            printf("Stock: %d\n", producto->stock);
            printf("Precio de venta: %.2f\n", producto->precioVenta);
            printf("Precio de mercado: %.2f\n", producto->precioMercado);
            printf("Precio de costo: %.2f\n", producto->precioCosto);
            printf("Vendidos: %d\n", producto->vendidos);
            printf("-----------------------------\n");

            producto = list_next(listaProductos);
        } 
    } else {
        puts("Producto no encontrado.");
    }
}

void listarProductosPorCategoria(HashMap *productosPorCategoria){
    limpiarPantalla();
    char categoria[51];
    printf("Ingrese la categoría de productos a listar: ");
    fgets(categoria, sizeof(categoria), stdin);
    categoria[strcspn(categoria, "\n")] = 0; // Eliminar salto de línea

    Pair *pair = searchMap(productosPorCategoria, categoria);
    if (pair == NULL) {
        printf("No se encontraron productos en la categoría '%s'.\n", categoria);
        return;
    }

    List *listaProductos = (List *)pair->value;
    Producto *producto = list_first(listaProductos);
    if (!producto) {
        printf("No se encontraron productos en la categoría '%s'.\n", categoria);
        return;
    }
    printf("\nProductos en la categoría '%s':\n", categoria);
    while (producto) {
        printf("\n");
        printf("Nombre: %s\n", producto->nombre);
        printf("Marca: %s\n", producto->marca);
        printf("Código de Barras: %s\n", producto->codigoBarras);
        printf("Stock: %d\n", producto->stock);
        printf("Precio Venta: %.2f\n", producto->precioVenta);
        printf("Precio Mercado: %.2f\n", producto->precioMercado);
        printf("Precio Costo: %.2f\n", producto->precioCosto);
        printf("Vendidos: %d\n", producto->vendidos);
        printf("-------------------------------------------------------------\n");

        producto = list_next(listaProductos);
    }
}

void mostrarProductosStockBajo(HashMap *productosPorCategoria) {
    limpiarPantalla();
    int umbral = 15;

    printf("Productos con stock bajo (<= %d):\n", umbral);
    Pair *pair = firstMap(productosPorCategoria);
    while (pair != NULL) {
        List *listaProductos = (List *)pair->value;
        Producto *producto = list_first(listaProductos);
        while (producto != NULL) {
            if (producto->stock <= umbral) {
                printf("\n");
                printf("Categoría: %s\n", producto->categoria);
                printf("Nombre: %s\n", producto->nombre);
                printf("Marca: %s\n", producto->marca);
                printf("Código de Barras: %s\n", producto->codigoBarras);
                printf("Stock: %d\n", producto->stock);
                printf("Precio Venta: %.2f\n", producto->precioVenta);
                printf("-------------------------------------------------------------\n");
            }
            producto = list_next(listaProductos);
        }
        pair = nextMap(productosPorCategoria);
    }
}

void registrarProducto(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap *productosPorNombre) {
    limpiarPantalla();
    Producto *producto = malloc(sizeof(Producto));
    printf("Ingrese el nombre del producto: ");
    fgets(producto->nombre, sizeof(producto->nombre), stdin);
    producto->nombre[strcspn(producto->nombre, "\n")] = 0; // Eliminar salto de línea

    printf("Ingrese la marca del producto: ");
    fgets(producto->marca, sizeof(producto->marca), stdin);
    producto->marca[strcspn(producto->marca, "\n")] = 0; // Eliminar salto de línea

    printf("Ingrese la categoría del producto: ");
    fgets(producto->categoria, sizeof(producto->categoria), stdin);
    producto->categoria[strcspn(producto->categoria, "\n")] = 0; // Eliminar salto de línea

    printf("Ingrese el código de barras del producto: ");
    fgets(producto->codigoBarras, sizeof(producto->codigoBarras), stdin);
    producto->codigoBarras[strcspn(producto->codigoBarras, "\n")] = 0; // Eliminar salto de línea

    if (searchMap(productosPorCodigo, producto->codigoBarras) != NULL) {
        puts("El código de barras ya existe. No se puede registrar el producto.");
        free(producto);
        return;
    }
    
    printf("Ingrese el stock del producto: ");
    scanf("%d", &producto->stock);
    getchar(); // Limpiar buffer

    printf("Ingrese el precio de venta del producto: ");
    scanf("%f", &producto->precioVenta);
    getchar(); // Limpiar buffer

    printf("Ingrese el precio de mercado del producto: ");
    scanf("%f", &producto->precioMercado);
    getchar(); // Limpiar buffer

    printf("Ingrese el precio de costo del producto: ");
    scanf("%f", &producto->precioCosto);
    getchar(); // Limpiar buffer

    if (searchMap(productosPorCodigo, producto->codigoBarras) == NULL) {
        
        insertMap(productosPorCodigo, producto->codigoBarras, producto);
    }
    if(searchMap(productosPorNombre, producto->nombre) == NULL) {
        List *listaProductos = list_create();
        list_pushBack(listaProductos, producto);
        insertMap(productosPorNombre, producto->nombre, listaProductos);
    } else {
        List *listaProductos = (List *)searchMap(productosPorNombre, producto->nombre)->value;
        list_pushBack(listaProductos, producto);
    }
    if ((searchMap(productosPorCategoria, producto->categoria) == NULL)) {
        List *listaProductosCategoria = list_create();
        list_pushBack(listaProductosCategoria, producto);
        insertMap(productosPorCategoria, producto->categoria, listaProductosCategoria);
    } else {
        List *listaProductosCategoria = (List *)searchMap(productosPorCategoria, producto->categoria)->value;
        list_pushBack(listaProductosCategoria, producto);
    }
    puts("Producto registrado exitosamente.");
}

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
                    /*case 4: modificarStock(productosPorCodigo); break;*/
                    case 5: mostrarProductosStockBajo(productosPorCategoria); break;
                    /*case 6: eliminarProducto(productosPorCodigo, productosPorCategoria); break;
                    case 7: guardarInventario(productosPorCodigo); break;*/
                    case 8: cargarInventario(productosPorNombre,productosPorCodigo ,productosPorCategoria); break;
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