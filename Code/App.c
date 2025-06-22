#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "List.h"
#include "utils.h"
#include "App.h"

void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
    puts("Presione una tecla para continuar...");
    getchar(); // Consume el '\n' del buffer de entrada
    getchar(); // Espera a que el usuario presione una tecla
}

void mostrarMenuAdministrador() {
    limpiarPantalla();
    printf("\n=== Menú Administrador ===\n");
    printf("1. Cargar inventario\n");
    printf("2. Métodos de búsqueda\n");
    printf("3. Modificar inventario\n");
    printf("4. Guardar inventario\n");
    printf("5. Generar reporte completo\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}


void mostrarMenuCliente() {
    limpiarPantalla();
    printf("\n=== Menú Cliente ===\n");
    printf("1. Agregar al Carrito\n");
    printf("2. Eliminar del Carrito\n");
    printf("3. Ver Carrito (Productos)\n");
    printf("4. Confirmar Compra\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

void menuBusqueda() {
    limpiarPantalla();
    printf("\n=== Menú de Búsqueda ===\n");
    printf("1. Buscar producto por nombre\n");
    printf("2. Buscar productos por categoría\n");
    printf("3. Mostrar stock de productos\n");
    printf("4. Mostrar ventas de productos\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

void menuModificarInventario() {
    limpiarPantalla();
    printf("\n=== Menú de Modificación de Inventario ===\n");
    printf("1. Registrar producto\n");
    printf("2. Modificar stock de producto\n");
    printf("3. Eliminar producto\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

void cargarInventario(char* nameFile, HashMap *productosPorNombre, HashMap *productosPorCodigo , HashMap *productosPorCategoria) {
    limpiarPantalla();
    FILE *file = fopen(nameFile, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        presioneTeclaParaContinuar();
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
    presioneTeclaParaContinuar();
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
            presioneTeclaParaContinuar();
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
    presioneTeclaParaContinuar();
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
        presioneTeclaParaContinuar();
        return;
    }

    List *listaProductos = (List *)pair->value;
    Producto *producto = list_first(listaProductos);
    if (!producto) {
        printf("No se encontraron productos en la categoría '%s'.\n", categoria);
        presioneTeclaParaContinuar();
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
    presioneTeclaParaContinuar();
}

void mostrarProductosStock(HashMap *productosPorCodigo) {
    limpiarPantalla();
    int umbral, opcion, contador = 0;

    // Solicitar al usuario el umbral
    printf("Ingrese el umbral de stock: ");
    scanf("%d", &umbral);
    getchar(); // Limpiar buffer

    // Solicitar al usuario si desea ver productos con stock mayor o menor al umbral
    printf("Seleccione una opción:\n");
    printf("1. Ver productos con stock menor o igual al umbral\n");
    printf("2. Ver productos con stock mayor o igual al umbral\n");
    printf("Opción: ");
    scanf("%d", &opcion);
    getchar(); // Limpiar buffer

    limpiarPantalla();
    if (opcion == 1) {
        printf("Productos con stock <= %d:\n", umbral);
    } else if (opcion == 2) {
        printf("Productos con stock >= %d:\n", umbral);
    } else {
        printf("Opción no válida.\n");
        presioneTeclaParaContinuar();
        return;
    }

    // Iterar sobre el mapa productosPorCodigo
    Pair *pair = firstMap(productosPorCodigo);
    while (pair != NULL) {
        Producto *producto = (Producto *)pair->value;
        if ((opcion == 1 && producto->stock <= umbral) || 
            (opcion == 2 && producto->stock >= umbral)) {
            printf("\n");
            printf("Nombre: %s\n", producto->nombre);
            printf("Categoría: %s\n", producto->categoria);
            printf("Marca: %s\n", producto->marca);
            printf("Código de Barras: %s\n", producto->codigoBarras);
            printf("Stock: %d\n", producto->stock);
            printf("Precio Venta: %.2f\n", producto->precioVenta);
            printf("-------------------------------------------------------------\n");
            contador++;
        }
        pair = nextMap(productosPorCodigo);
    }

    if (contador == 0) {
        printf("No se encontraron productos con las condiciones especificadas.\n");
    }
    presioneTeclaParaContinuar();
}

void mostrarVentasProductos(HashMap *productosPorCodigo) {
    limpiarPantalla();
    int umbral, opcion, contador = 0;

    // Solicitar al usuario el umbral
    printf("Ingrese el umbral de ventas: ");
    scanf("%d", &umbral);
    getchar(); // Limpiar buffer

    // Solicitar al usuario si desea ver productos con ventas mayores o menores al umbral
    printf("Seleccione una opción:\n");
    printf("1. Ver productos con ventas menores o iguales al umbral\n");
    printf("2. Ver productos con ventas mayores o iguales al umbral\n");
    printf("Opción: ");
    scanf("%d", &opcion);
    getchar(); // Limpiar buffer

    limpiarPantalla();
    if (opcion == 1) {
        printf("Productos con ventas <= %d:\n", umbral);
    } else if (opcion == 2) {
        printf("Productos con ventas >= %d:\n", umbral);
    } else {
        printf("Opción no válida.\n");
        presioneTeclaParaContinuar();
        return;
    }

    // Iterar sobre el mapa productosPorCodigo
    Pair *pair = firstMap(productosPorCodigo);
    while (pair != NULL) {
        Producto *producto = (Producto *)pair->value;
        if ((opcion == 1 && producto->vendidos <= umbral) || 
            (opcion == 2 && producto->vendidos >= umbral)) {
            printf("\n");
            printf("Nombre: %s\n", producto->nombre);
            printf("Categoría: %s\n", producto->categoria);
            printf("Marca: %s\n", producto->marca);
            printf("Código de Barras: %s\n", producto->codigoBarras);
            printf("Stock: %d\n", producto->stock);
            printf("Precio Venta: %.2f\n", producto->precioVenta);
            printf("Vendidos: %d\n", producto->vendidos);
            printf("-------------------------------------------------------------\n");
            contador++;
        }
        pair = nextMap(productosPorCodigo);
    }

    if (contador == 0) {
        printf("No se encontraron productos con las condiciones especificadas.\n");
    }
    presioneTeclaParaContinuar();
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
        presioneTeclaParaContinuar();
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
    presioneTeclaParaContinuar();
}

void modificarStock(HashMap* productosPorCodigo) {
    limpiarPantalla();
    char codBarra[51];
    printf("Ingrese el código de barras del producto a modificar: ");
    fgets(codBarra, sizeof(codBarra), stdin);
    codBarra[strcspn(codBarra, "\n")] = 0;

    Pair* pair = searchMap(productosPorCodigo, codBarra);
    if (pair == NULL) {
        printf("Producto no encontrado.\n");
        presioneTeclaParaContinuar();
        return;
    }
    Producto* producto = (Producto*)pair->value;
    printf("Stock actual: %d\n", producto->stock);
    printf("Ingrese el nuevo stock: ");
    int nuevoStock;
    scanf("%d", &nuevoStock);
    getchar(); // Limpiar buffer
    producto->stock = nuevoStock;
    printf("Stock actualizado correctamente.\n");
    presioneTeclaParaContinuar();
}

void eliminarProducto(HashMap* productosPorCodigo, HashMap* productosPorCategoria) {
    limpiarPantalla();
    char codBarra[51];
    printf("Ingrese el código de barras del producto a eliminar: ");
    fgets(codBarra, sizeof(codBarra), stdin);
    codBarra[strcspn(codBarra, "\n")] = 0;

    Pair* pair = searchMap(productosPorCodigo, codBarra);
    if (pair == NULL) {
        printf("Producto no encontrado.\n");
        presioneTeclaParaContinuar();
        return;
    }
    Producto* producto = (Producto*)pair->value;

    // Eliminar de productosPorCategoria
    Pair* pairCat = searchMap(productosPorCategoria, producto->categoria);
    if (pairCat != NULL) {
        List* lista = (List*)pairCat->value;
        Producto* prodLista = list_first(lista);
        int idx = 0;
        while (prodLista != NULL) {
            if (strcmp(prodLista->codigoBarras, codBarra) == 0) {
                // Eliminar de la lista
                // Mover current a la posición correcta
                list_first(lista);
                for (int i = 0; i < idx; i++) list_next(lista);
                list_popCurrent(lista);
                break;
            }
            prodLista = list_next(lista);
            idx++;
        }
    }

    // Eliminar de productosPorCodigo
    eraseMap(productosPorCodigo, codBarra);

    printf("Producto eliminado correctamente.\n");
    presioneTeclaParaContinuar();
}

void guardarInventario(HashMap* productosPorCodigo) {
    limpiarPantalla();
    char nombreArchivo[100] = "inventario_guardado.csv";
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("No se pudo abrir el archivo para guardar.\n");
        presioneTeclaParaContinuar();
        return;
    }
    // Escribir encabezado
    fprintf(archivo, "Nombre,Marca,Categoria,CodigoBarras,Stock,PrecioVenta,PrecioMercado,PrecioCosto,Vendidos\n");
    Pair* pair = firstMap(productosPorCodigo);
    while (pair != NULL) {
        Producto* producto = (Producto*)pair->value;
        fprintf(archivo, "%s,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d\n",
            producto->nombre,
            producto->marca,
            producto->categoria,
            producto->codigoBarras,
            producto->stock,
            producto->precioVenta,
            producto->precioMercado,
            producto->precioCosto,
            producto->vendidos
        );
        pair = nextMap(productosPorCodigo);
    }
    fclose(archivo);
    printf("Inventario guardado en '%s'.\n", nombreArchivo);
    presioneTeclaParaContinuar();
}

void agregarAlCarrito(HashMap *productosPorCodigo, List *carrito) {
    limpiarPantalla();
    char codigo[51];
    printf("Ingrese el código de barras del producto a agregar: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = 0;

    Pair *pair = searchMap(productosPorCodigo, codigo);
    if (!pair) {
        printf("Producto no encontrado.\n");
        presioneTeclaParaContinuar();
        return;
    }
    Producto *producto = (Producto *)pair->value;
    if (producto->stock <= 0) {
        printf("No hay stock disponible para este producto.\n");
        return;
    }
    int cantidad;
    printf("Ingrese la cantidad a agregar: ");
    scanf("%d", &cantidad);
    getchar(); // Limpiar buffer

    if (cantidad <= 0 || cantidad > producto->stock) {
        printf("Cantidad inválida o insuficiente stock.\n");
        return;
    }

    // Crear una copia del producto para el carrito (solo con los datos necesarios)
    Producto *productoCarrito = malloc(sizeof(Producto));
    *productoCarrito = *producto;
    productoCarrito->stock = cantidad;
    list_pushBack(carrito, productoCarrito);

    producto->stock -= cantidad;
    printf("Producto agregado al carrito.\n");
    presioneTeclaParaContinuar();
}

void eliminarDelCarrito(List *carrito) {
    limpiarPantalla();
    if (list_first(carrito) == NULL) {
        printf("El carrito está vacío.\n");
        presioneTeclaParaContinuar();
        return;
    }
    int idx = 0, pos, total = 0;
    Producto *prod = list_first(carrito);
    printf("Productos en el carrito:\n");
    while (prod) {
        printf("%d. %s | Marca: %s | Cantidad: %d\n", idx + 1, prod->nombre, prod->marca, prod->stock);
        prod = list_next(carrito);
        idx++;
    }
    total = idx;
    printf("Ingrese el número del producto a eliminar: ");
    scanf("%d", &pos);
    getchar(); // Limpiar buffer

    if (pos < 1 || pos > total) {
        printf("Opción inválida.\n");
        presioneTeclaParaContinuar();
        return;
    }
    // Volver al inicio y avanzar hasta la posición
    list_first(carrito);
    for (int i = 1; i < pos; i++) list_next(carrito);
    Producto *eliminado = list_popCurrent(carrito);
    free(eliminado);
    printf("Producto eliminado del carrito.\n");
    presioneTeclaParaContinuar();
}

void verCarrito(List *carrito) {
    limpiarPantalla();
    Producto *prod = list_first(carrito);
    if (!prod) {
        printf("El carrito está vacío.\n");
        presioneTeclaParaContinuar();
        return;
    }
    printf("Productos en el carrito:\n");
    int idx = 1;
    float total = 0;
    while (prod) {
        printf("%d. %s | Marca: %s | Cantidad: %d | Precio unitario: %.2f | Subtotal: %.2f\n",
            idx, prod->nombre, prod->marca, prod->stock, prod->precioVenta, prod->precioVenta * prod->stock);
        total += prod->precioVenta * prod->stock;
        prod = list_next(carrito);
        idx++;
    }
    printf("Total: %.2f\n", total);
    presioneTeclaParaContinuar();
}