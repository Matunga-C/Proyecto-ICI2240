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


//Función que muestra el menú del administrador
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

//Función que muestra el menú del cliente
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

//Función que muestra el menú de búsqueda
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

//Función que muestra el menú de modificación del inventario
void menuModificarInventario() {
    limpiarPantalla();
    printf("\n=== Menú de Modificación de Inventario ===\n");
    printf("1. Registrar producto\n");
    printf("2. Modificar stock de producto\n");
    printf("3. Eliminar producto\n");
    printf("0. Salir\n");
    printf("Seleccione una opción: ");
}

//Función que carga el csv que contiene el inventario de productos
void cargarInventario(char* nameFile, HashMap *productosPorNombre, HashMap *productosPorCodigo , HashMap *productosPorCategoria) {
    limpiarPantalla();
    //Se abre el archivo CSV
    FILE *file = fopen(nameFile, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        presioneTeclaParaContinuar();
        return;
    }
    char **campos;
    // Leer encabezado
    campos = leer_linea_csv(file, ',');
    //Lee cada linea del archivo CSV
    while ((campos = leer_linea_csv(file, ',')) != NULL) {
        // Verifica que existan todos los campos necesarios
        if (!campos[1] || !campos[2] || !campos[3] || !campos[4] ||
            !campos[5] || !campos[6] || !campos[7] || !campos[8]) {
            continue;
        }
        // Crea un nuevo producto y asigna memoria
        Producto *producto = malloc(sizeof(Producto));
        if (!producto) {
            perror("No se pudo asignar memoria para producto");
            // free(campos);
            continue;
        }

        //Asigna el nombre del producto
        // Normalizar el nombre
        trimWhitespace(campos[1]);
        toLowerCase(campos[1]);

        strncpy(producto->nombre, campos[1], sizeof(producto->nombre) - 1);
        producto->nombre[sizeof(producto->nombre) - 1] = '\0';
        
        //Asigna la marca del producto
        trimWhitespace(campos[2]);
        toLowerCase(campos[2]);

        strncpy(producto->marca, campos[2], sizeof(producto->marca) - 1);
        producto->marca[sizeof(producto->marca) - 1] = '\0';
        
        //Asigna la categoría del producto
        // Normalizar la categoría
        trimWhitespace(campos[3]);
        toLowerCase(campos[3]);

        strncpy(producto->categoria, campos[3], sizeof(producto->categoria) - 1);
        producto->categoria[sizeof(producto->categoria) - 1] = '\0';
        
        //Asigna el código de barras del producto
        strncpy(producto->codigoBarras, campos[8], sizeof(producto->codigoBarras) - 1);
        producto->codigoBarras[sizeof(producto->codigoBarras) - 1] = '\0';
        
        //Asigna el stock del producto
        producto->stock = atoi(campos[7]);
        
        //Asigna los precios del producto
        producto->precioVenta = atof(campos[4]);
        producto->precioMercado = atof(campos[5]);
        producto->precioCosto = atof(campos[6]);
        
        //Inicializa los vendidos a 0
        producto->vendidos = 0;

        // Insertar en el mapa de productosPorCodigo
        if (searchMap(productosPorCodigo, producto->codigoBarras) == NULL) {
            insertMap(productosPorCodigo, producto->codigoBarras, producto);
        }

        // Insertar en el mapa de productosPorNombre
        Pair *pairNombre = searchMap(productosPorNombre, producto->nombre);
        if (pairNombre == NULL) {
            List *listaProductos = createList();
            pushBackList(listaProductos, producto);
            insertMap(productosPorNombre, producto->nombre, listaProductos);
        } else {
            List *listaProductos = (List *)pairNombre->value;
            pushBackList(listaProductos, producto);
        }

        // Insertar en productosPorCategoria
        Pair *pairCategoria = searchMap(productosPorCategoria, producto->categoria);
        if (pairCategoria == NULL) {
            List *listaProductosCategoria = createList();
            pushBackList(listaProductosCategoria, producto);
            insertMap(productosPorCategoria, producto->categoria, listaProductosCategoria);
        } else {
            List *listaProductosCategoria = (List *)pairCategoria->value;
            pushBackList(listaProductosCategoria, producto);
        }
    }
    //Se muestra un mensaje de éxito al cargar el inventario y se cierra el archivo
    puts("Inventario cargado exitosamente.");
    fclose(file);
    presioneTeclaParaContinuar();
}

//Función encargada de buscar un producto por su nombre
void buscarPorNombre(HashMap *productosPorNombre) {
    limpiarPantalla();
    char nombre[51];
    printf("Ingrese el nombre del producto a buscar: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = 0;
    // Busca el producto en el mapa de productosPorNombre
    Pair *pair = searchMap(productosPorNombre, nombre);
    if (pair != NULL) {
        // Si se encuentra el producto, se muestra su información
        // Se obtiene la lista de productos con ese nombre y se itera sobre ella para mostrar los detalles
        List *listaProductos = (List *)pair->value;
        Producto *producto = (Producto *)firstList(listaProductos);
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

            producto = nextList(listaProductos);
        } 
    } else {
        // Si no se encuentra el producto, se muestra un mensaje no encontrado
        puts("Producto no encontrado.");
    }
    presioneTeclaParaContinuar();
}

//Función encargada de buscar productos por categoría
void buscarPorCategoria(HashMap *productosPorCategoria){
    limpiarPantalla();
    // Se indexa el mapa para mostrar las categorías disponibles
    Pair* temp = firstMap(productosPorCategoria);
    printf("Categorías disponibles:\n");
    while(temp != NULL){
        printf("- %s\n", (char*)temp->key);
        temp = nextMap(productosPorCategoria);
    }
    // Solicita al usuario que ingrese la categoría a buscar
    char categoria[51];
    printf("Ingrese la categoría de productos a listar: ");
    fgets(categoria, sizeof(categoria), stdin);
    categoria[strcspn(categoria, "\n")] = 0; // Eliminar salto de línea
    // Se busca la categoría en el mapa de productos por categoría, si no lo encuentra mustra un mensaje de error.
    Pair *pair = searchMap(productosPorCategoria, categoria);
    if (pair == NULL) {
        printf("No se encontraron productos en la categoría '%s'.\n", categoria);
        presioneTeclaParaContinuar();
        return;
    }
    // Si se encuentra la categoría, se obtiene la lista de productos y se itera sobre ella para mostrar los detalles
    List *listaProductos = (List *)pair->value;
    Producto *producto = firstList(listaProductos);
    if (!producto) {
        // Si la lista de productos está vacía, se muestra un mensaje de error
        printf("No se encontraron productos en la categoría '%s'.\n", categoria);
        presioneTeclaParaContinuar();
        return;
    }
    // Se muestra la información de los productos encontrados en la categoría
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

        producto = nextList(listaProductos);
    }
    presioneTeclaParaContinuar();
}

//Función que muestra los productos con stock mayor o menor al umbral ingresado por el usuario
void mostrarProductosStock(HashMap *productosPorCodigo) {
    limpiarPantalla();
    int umbral, opcion, contador = 0;

    // Solicitar al usuario el umbral desde el que desea ver los productos por stock
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
    // Limpiar pantalla para mostrar los resultados
    // Se muestra el mensaje correspondiente según la opción seleccionada
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

    // Iterar sobre el mapa productosPorCodigo mostrando solo los que cumplen con la condición del umbral
    // Se utiliza un contador para llevar la cuenta de los productos que cumplen con la condición
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
    // Si no se encontraron productos que cumplan con las condiciones, se muestra un mensaje
    if (contador == 0) {
        printf("No se encontraron productos con las condiciones especificadas.\n");
    }
    presioneTeclaParaContinuar();
}

//Función que muestra los productos con ventas mayores o menores al umbral ingresado por el usuario
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
    // Limpiar pantalla para mostrar los resultados
    // Se muestra el mensaje correspondiente según la opción seleccionada
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

    // Iterar sobre el mapa productosPorCodigo y mostrar los que cumplan con la condición de ventas
    // Se utiliza un contador para llevar la cuenta de los productos que cumplen con la condición
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
    // Si no se encontraron productos que cumplan con las condiciones, se muestra un mensaje
    if (contador == 0) {
        printf("No se encontraron productos con las condiciones especificadas.\n");
    }
    presioneTeclaParaContinuar();
}

//Función que registra un producto en el inventario
void registrarProducto(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap *productosPorNombre) {
    limpiarPantalla();
    // Se crea un nuevo producto y se asigna memoria
    puts("=== Registro de Producto ===");
    Producto *producto = malloc(sizeof(Producto));
    //se le pide al usuario que ingrese los datos del producto
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
    // Verifica si el código de barras ya existe en el inventario, si ya existe no se puede registrar el producto 
    // y se muestra un mensaje de error
    if (searchMap(productosPorCodigo, producto->codigoBarras) != NULL) {
        puts("El código de barras ya existe. No se puede registrar el producto.");
        presioneTeclaParaContinuar();
        free(producto);
        return;
    }
    // Si no encuentra el código de barras, se le pide al usuario que ingrese los datos restantes del producto
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
    producto->vendidos = 0; 
    // Se verifica si el producto ya existe en los mapas de productos por código, nombre y categoría
    // Si no existe, se inserta el producto en los mapas correspondientes
    if (searchMap(productosPorCodigo, producto->codigoBarras) == NULL) {
        insertMap(productosPorCodigo, producto->codigoBarras, producto);
    }
    // Si ya existe, se agrega a la lista de productos con el mismo nombre o categoría
    if(searchMap(productosPorNombre, producto->nombre) == NULL) {
        List *listaProductos = createList();
        pushBackList(listaProductos, producto);
        insertMap(productosPorNombre, producto->nombre, listaProductos);
    } else {
        List *listaProductos = (List *)searchMap(productosPorNombre, producto->nombre)->value;
        pushBackList(listaProductos, producto);
    }
    if ((searchMap(productosPorCategoria, producto->categoria) == NULL)) {
        List *listaProductosCategoria = createList();
        pushBackList(listaProductosCategoria, producto);
        insertMap(productosPorCategoria, producto->categoria, listaProductosCategoria);
    } else {
        List *listaProductosCategoria = (List *)searchMap(productosPorCategoria, producto->categoria)->value;
        pushBackList(listaProductosCategoria, producto);
    }
    puts("Producto registrado exitosamente.");
    presioneTeclaParaContinuar();
}
//Función que modifica el stock de un producto existente en el inventario
void modificarStock(HashMap* productosPorCodigo, HashMap* productosPorCategoria, HashMap* productosPorNombre) {
    limpiarPantalla();
    char codBarra[51];
    // Se solicita al usuario que ingrese el código de barras del producto a modificar
    printf("Ingrese el código de barras del producto a modificar: ");
    fgets(codBarra, sizeof(codBarra), stdin);
    codBarra[strcspn(codBarra, "\n")] = 0;
    // Se busca el producto en el mapa de productos por código, en caso de no encontrarlo se muestra un mensaje de error
    Pair* pair = searchMap(productosPorCodigo, codBarra);
    if (pair == NULL) {
        printf("Producto no encontrado.\n");
        presioneTeclaParaContinuar();
        return;
    }
    Producto* producto = (Producto*)pair->value;
    // Se muestra el stock actual del producto y se solicita al usuario que ingrese el nuevo stock
    printf("Stock actual: %d\n", producto->stock);
    printf("Ingrese el nuevo stock: ");
    int stockActual = producto->stock;
    int nuevoStock;
    scanf("%d", &nuevoStock);
    getchar(); // Limpiar buffer
    producto->stock = nuevoStock;
    Pair* pairNombre = searchMap(productosPorNombre, producto->nombre);
    if (pairNombre != NULL) {
        List* listaProductos = (List*)pairNombre->value;
        Producto* prodLista = firstList(listaProductos);
        while (prodLista != NULL) {
            if (strcmp(prodLista->codigoBarras, codBarra) == 0) {
                prodLista->stock = nuevoStock;
                break;
            }
            prodLista = nextList(listaProductos);
        }
    }

    // Actualiza el stock en el mapa de productos por categoría
    Pair* pairCategoria = searchMap(productosPorCategoria, producto->categoria);
    if (pairCategoria != NULL) {
        List* listaProductosCategoria = (List*)pairCategoria->value;
        Producto* prodLista = firstList(listaProductosCategoria);
        while (prodLista != NULL) {
            if (strcmp(prodLista->codigoBarras, codBarra) == 0) {
                prodLista->stock = nuevoStock;
                break;
            }
            prodLista = nextList(listaProductosCategoria);
        }
    }
    // Se muestra un mensaje de éxito al modificar el stock del producto
    printf("Stock actualizado correctamente.\n");
    presioneTeclaParaContinuar();
}
//Función que elimina un producto del inventario
void eliminarProducto(HashMap* productosPorCodigo, HashMap* productosPorCategoria, HashMap* productosPorNombre) {
    limpiarPantalla();
    char codBarra[51];
    // Se solicita al usuario que ingrese el código de barras del producto a eliminar
    printf("Ingrese el código de barras del producto a eliminar: ");
    fgets(codBarra, sizeof(codBarra), stdin);
    codBarra[strcspn(codBarra, "\n")] = 0;
    // Se busca el producto en el mapa de productos por código, en caso de no encontrarlo se muestra un mensaje de error
    Pair* pair = searchMap(productosPorCodigo, codBarra);
    if (pair == NULL) {
        printf("Producto no encontrado.\n");
        presioneTeclaParaContinuar();
        return;
    }
    Producto* producto = (Producto*)pair->value;

    // Elimina el producto de productosPorCategoria
    Pair* pairCat = searchMap(productosPorCategoria, producto->categoria);
    if (pairCat != NULL) {
        List* lista = (List*)pairCat->value;
        Producto* prodLista = firstList(lista);
        int idx = 0;
        while (prodLista != NULL) {
            if (strcmp(prodLista->codigoBarras, codBarra) == 0) {
                // Eliminar de la lista
                // Mover current a la posición correcta
                firstList(lista);
                for (int i = 0; i < idx; i++) nextList(lista);
                popCurrentList(lista);
                break;
            }
            prodLista = nextList(lista);
            idx++;
        }
        // Si la lista de productos en esa categoría queda vacía, se elimina la categoría del mapa
        if (firstList(lista) == NULL) {
            eraseMap(productosPorCategoria, producto->categoria);
        }
    }
    // Elimina el producto de productosPorNombre
    Pair* pairNom = searchMap(productosPorNombre, producto->nombre);
    if (pairNom != NULL) {
        List* lista = (List*)pairNom->value;
        Producto* prodLista = firstList(lista);
        int idx = 0;
        while (prodLista != NULL) {
            if (strcmp(prodLista->codigoBarras, codBarra) == 0) {
                // Eliminar de la lista
                // Mover current a la posición correcta
                firstList(lista);
                for (int i = 0; i < idx; i++) nextList(lista);
                popCurrentList(lista);
                break;
            }
            prodLista = nextList(lista);
            idx++;
        }
        // Si la lista de productos con ese nombre queda vacía, se elimina el nombre del mapa
        if (firstList(lista) == NULL) {
            eraseMap(productosPorNombre, producto->nombre);
        }
    }
    
    // Eliminar de productosPorCodigo
    eraseMap(productosPorCodigo, codBarra);

    printf("Producto eliminado correctamente.\n");
    presioneTeclaParaContinuar();
}

//Función que guarda el inventario en un archivo CSV
void guardarInventario(HashMap* productosPorCodigo) {
    limpiarPantalla();
    //se define el nombre del archivo CSV donde se guardará el inventario
    char nombreArchivo[100] = "inventario_guardado.csv";
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("No se pudo abrir el archivo para guardar.\n");
        presioneTeclaParaContinuar();
        return;
    }
    // Escribir encabezado
    fprintf(archivo, "ID, Nombre,Marca,Categoria,CodigoBarras,Stock,PrecioVenta,PrecioMercado,PrecioCosto,Vendidos\n");
    //se traspasan todos los produtos del mapa productosPorCodigo al archivo CSV
    // Se itera sobre el mapa de productos por código y se escribe cada producto en el
    Pair* pair = firstMap(productosPorCodigo);
    size_t idx = 0;
    while (pair != NULL) {
        Producto* producto = (Producto*)pair->value;
        fprintf(archivo, "%zu,%s,%s,%s,%s,%d,%.2f,%.2f,%.2f,%d\n", idx + 1,
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
        idx++;
    }
    fclose(archivo);
    // Se muestra un mensaje de éxito al guardar el inventario y el nombre del archivo
    printf("Inventario guardado en '%s'.\n", nombreArchivo);
    presioneTeclaParaContinuar();
}

void generarReporte(HashMap* productosPorCodigo, HashMap *productosPorCategoria, List* historialCompras, HashMap *contadorProducto) {
    limpiarPantalla();
    if (historialCompras == NULL || firstList(historialCompras) == NULL) {
        printf("No hay historial de compras para generar un reporte.\n");
        presioneTeclaParaContinuar();
        return;
    }
    printf("=== Generación de Reporte Completo ===\n"); 
    List *productosVendidos = createList();
    // Iterar sobre el mapa contadorProducto
    Pair *par = firstMap(contadorProducto);
    while (par != NULL) {
        // Buscar si ya existe un producto con el mismo nombre en la lista
        NombreVenta *existente = NULL;
        for (NombreVenta *nv = firstList(productosVendidos); nv != NULL; nv = nextList(productosVendidos)) {
            if (strcmp(nv->nombre, par->key) == 0) {
                existente = nv;
                break;
            }
        }
        if (existente) {
            // Si ya existe, sumar las ventas
            existente->cantidadVentas += *((int *)par->value);
        } else {
            // Si no existe, agregar un nuevo elemento
            NombreVenta *nombreVenta = malloc(sizeof(NombreVenta));
            if (nombreVenta == NULL) {
                printf("Error al asignar memoria.\n");
                presioneTeclaParaContinuar();
                return;
            }
            strcpy(nombreVenta->nombre, par->key);
            nombreVenta->cantidadVentas = *((int *)par->value);
            pushBackList(productosVendidos, nombreVenta);
        }
        par = nextMap(contadorProducto);
    }

    // Ordenar la lista por cantidad de ventas (de mayor a menor)
    for (NombreVenta * i = firstList(productosVendidos); i != NULL; i = nextList(productosVendidos)) {
        for (NombreVenta* j = nextList(productosVendidos); j != NULL; j = nextList(productosVendidos)) {
            if (i->cantidadVentas < j->cantidadVentas) {
                // Intercambiar los elementos
                NombreVenta temp = *i;
                *i = *j;
                *j = temp;
            }
        }
    }
    if (firstList(productosVendidos) != NULL) {
        printf("Top 3 productos más vendidos:\n");
        int contador = 0;
        for (NombreVenta *producto = firstList(productosVendidos); producto != NULL && contador < 3; producto = nextList(productosVendidos)) {
            printf("%d. %s - Ventas: %d\n", contador + 1, producto->nombre, producto->cantidadVentas);
            contador++;
        }
    }
    // Mostrar los 3 productos más vendidos
    

    HashMap* graph = createMap(2000);
    List* compra = firstList(historialCompras);

    // Paso 1: construir el grafo a partir de historial
    while (compra != NULL) {
        Producto* prodA = firstList(compra);
        while (prodA != NULL) {
            Producto* prodB = nextList(compra);
            while (prodB != NULL) {
                if (strcmp(prodA->codigoBarras, prodB->codigoBarras) != 0) {
                    insertarFrecuencia(graph, prodA->nombre, prodB->nombre);
                    insertarFrecuencia(graph, prodB->nombre, prodA->nombre);
                }
                prodB = nextList(compra);
            }
            prodA = nextList(compra);
        }
        compra = nextList(historialCompras);
    }
    List *productosConFrecuencias = createList();
    if (firstMap(graph) != NULL) {
        printf("=== Combos frecuentes detectados ===\n");
        Pair *parNodo = firstMap(graph);
        while (parNodo != NULL) {
            char *nombreA = parNodo->key;
            HashMap *adyacentes = (HashMap *)parNodo->value;

            // Aquí podrías buscar productoA si lo necesitas para calcular % co-ocurrencia

            Pair *parAdy = firstMap(adyacentes);
            while (parAdy != NULL) {
                char *nombreB = parAdy->key;
                int frecuencia = *((int *)parAdy->value);
                combo *comboFrecuente = malloc(sizeof(combo));
                if (comboFrecuente == NULL) {
                    printf("Error al asignar memoria para combo frecuente.\n");
                    presioneTeclaParaContinuar();
                    return;
                }
                strcpy(comboFrecuente->nombreA, nombreA);
                strcpy(comboFrecuente->nombreB, nombreB);
                comboFrecuente->frecuencia = frecuencia;
                pushBackList(productosConFrecuencias, comboFrecuente);
                //printf("- %s + %s → Comprados juntos %d veces\n", nombreA, nombreB, frecuencia);

                parAdy = nextMap(adyacentes);
            }

            parNodo = nextMap(graph);
        }
        for (combo *i = firstList(productosConFrecuencias); i != NULL; i = nextList(productosConFrecuencias)) {
            for (combo *j = nextList(productosConFrecuencias); j != NULL; j = nextList(productosConFrecuencias)) {
                if (i->frecuencia < j->frecuencia) {
                    // Intercambiar los elementos
                    combo temp = *i;
                    *i = *j;
                    *j = temp;
                }
            }
        }
        // Mostrar los combos más frecuentes
        int contador = 0;
        for (combo *c = firstList(productosConFrecuencias); c != NULL && contador < 3; c = nextList(productosConFrecuencias)) {
            printf("%d. %s + %s → Comprados juntos %d veces\n", contador + 1, c->nombreA, c->nombreB, c->frecuencia);
            contador++;
        }
    }
    List *productosPocasVentas = createList();
    // Paso 3: sugerencias adicionales por ventas bajas
    sugerirPromociones(productosPorCodigo, productosPorCategoria, contadorProducto, productosPocasVentas);
    printf("¿Quieres hacer un descuento a los productos con bajas ventas? (s/n): ");
    char respuesta;
    scanf(" %c", &respuesta);
    getchar(); // Limpiar buffer
    if (respuesta == 's' || respuesta == 'S') {
        // Aplicar descuentos a productos con bajas ventas
        realizarDescuento(productosPorCodigo, productosPorCategoria, contadorProducto, productosPocasVentas);
    } else {
        printf("No se aplicarán descuentos.\n");
    }

    presioneTeclaParaContinuar();
}

// Función que suguiere promociones para productos con bajas ventas
void sugerirPromociones(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap* contadorProducto, List *productosPocasVentas) {
    printf("\n\n=== Productos con pocas ventas: Sugerencias de promoción ===\n");

    const int umbral_ventas_bajas = 3; // Puedes ajustar este valor
    int i = 0;
    Pair *par = firstMap(productosPorCodigo);
    while (par != NULL && i < 10) { 
        Producto *producto = (Producto *)par->value;

        if (producto->vendidos <= umbral_ventas_bajas) {
            printf("\nProducto con bajas ventas detectado:\n");
            printf("%d)Nombre: %s | Marca: %s | Vendidos: %d | Stock actual: %d\n",
                   i + 1,producto->nombre, producto->marca, producto->vendidos, producto->stock);

            // Sugerencia de descuento
            float relacion = producto->precioVenta / producto->precioCosto;
            pushBackList(productosPocasVentas, producto);
            if (relacion > 1.5) {
                printf("→ Sugerencia: aplicar 10%% de descuento.\n");
            } else {
                printf("→ Sugerencia: aplicar 5%% de descuento.\n");
            }

            // Buscar candidatos en la misma categoría para combos
            Pair *pairCat = searchMap(productosPorCategoria, producto->categoria);
            if (pairCat != NULL) {
                List *lista = (List *)pairCat->value;
                Producto *candidato = firstList(lista);
                while (candidato != NULL) {
                    if (candidato != producto && (candidato->vendidos - producto->vendidos) <= 5 && strcmp(candidato->categoria, producto->categoria) == 0) {
                        printf("→ Sugerencia: crear combo con '%s' (vendidos: %d)\n",
                               candidato->nombre, candidato->vendidos);
                        break;
                    }
                    candidato = nextList(lista);
                }
            }

            printf("→ Acción sugerida: aplicar descuento o visibilidad en la tienda.\n");
            printf("-----------------------------------------------------------\n");
            i++;
        }

        par = nextMap(productosPorCodigo);
    }

    presioneTeclaParaContinuar();
}

void realizarDescuento(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap* contadorProducto, List *productosPocasVentas) {
    printf("Ingrese el número del producto al cual quiera aplicar un descuento: ");
    int opcion;
    scanf("%d", &opcion);
    getchar(); // Limpiar buffer

    // Verificar si la opción es válida
    if (opcion <= 0 || opcion > list_size(productosPocasVentas)) {
        printf("Opción inválida.\n");
        presioneTeclaParaContinuar();
        return;
    }

    // Iterar hasta el producto correspondiente
    Producto *producto = firstList(productosPocasVentas);
    for (int i = 1; i < opcion; i++) {
        producto = nextList(productosPocasVentas);
    }

    // Aplicar el descuento al producto seleccionado
    if (producto != NULL) {
        printf("Producto seleccionado: %s | Precio actual: %.2f\n", producto->nombre, producto->precioVenta);
        printf("Ingrese el porcentaje de descuento (ejemplo: 10 para 10%%): ");
        float porcentajeDescuento;
        scanf("%f", &porcentajeDescuento);
        getchar(); // Limpiar buffer

        if (porcentajeDescuento <= 0 || porcentajeDescuento > 100) {
            printf("Porcentaje de descuento inválido.\n");
            presioneTeclaParaContinuar();
            return;
        }

        // Calcular el nuevo precio
        float factorDescuento = 1 - (porcentajeDescuento / 100.0);
        producto->precioVenta *= factorDescuento;

        printf("Descuento aplicado. Nuevo precio: %.2f\n", producto->precioVenta);
    } else {
        printf("No se encontró el producto.\n");
    }

    presioneTeclaParaContinuar();
}

// Función que agrega un producto al carrito de compras
void agregarAlCarrito(HashMap *productosPorCodigo, List *carrito) {
    limpiarPantalla();
    char codigo[51];
    // Se solicita al usuario que ingrese el código de barras del producto a agregar al carrito
    printf("Ingrese el código de barras del producto a agregar: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = 0;
    // Se busca el producto en el mapa de productos por código, sino lo encuentra, muestra un mensaje de error
    Pair *pair = searchMap(productosPorCodigo, codigo);
    if (!pair) {
        printf("Producto no encontrado.\n");
        presioneTeclaParaContinuar();
        return;
    }
    // Si encuentra el producto, verifica si hay stock disponible, en caso de no haber stock, muestra un mensaje de error
    Producto *producto = (Producto *)pair->value;
    if (producto->stock <= 0) {
        printf("No hay stock disponible para este producto.\n");
        presioneTeclaParaContinuar();
        return;
    }
    // Si hay stock, solicita al usuario que ingrese la cantidad a agregar al carrito
    int cantidad;
    printf("Ingrese la cantidad a agregar: ");
    scanf("%d", &cantidad);
    getchar(); // Limpiar buffer
    // Si la cantidad de productos es inválida (menor o igual a 0 o mayor que el stock disponible), muestra un mensaje de error
    if (cantidad <= 0 || cantidad > producto->stock) {
        printf("Cantidad inválida o insuficiente stock.\n");
        presioneTeclaParaContinuar();
        return;
    }

    // Crear una copia del producto para el carrito (solo con los datos necesarios)
    Producto *productoCarrito = malloc(sizeof(Producto));
    *productoCarrito = *producto;
    productoCarrito->stock = cantidad;
    pushBackList(carrito, productoCarrito);
    // Actualizar el stock del producto original
    producto->stock -= cantidad;
    printf("Producto agregado al carrito.\n");
    presioneTeclaParaContinuar();
}

//Función que elimina un producto del carrito de compras
void eliminarDelCarrito(List *carrito) {
    limpiarPantalla();
    // Verifica si el carrito está vacío
    if (firstList(carrito) == NULL) {
        printf("El carrito está vacío.\n");
        presioneTeclaParaContinuar();
        return;
    }
    int idx = 0, pos, total = 0;
    Producto *prod = firstList(carrito);
    // Muestra los productos en el carrito y solicita al usuario que ingrese el número del producto a eliminar
    printf("Productos en el carrito:\n");
    while (prod) {
        printf("%d. %s | Marca: %s | Cantidad: %d\n", idx + 1, prod->nombre, prod->marca, prod->stock);
        prod = nextList(carrito);
        idx++;
    }
    total = idx;
    printf("Ingrese el número del producto a eliminar: ");
    scanf("%d", &pos);
    getchar(); // Limpiar buffer
    // Verifica si la posición ingresada es válida
    if (pos < 1 || pos > total) {
        printf("Opción inválida.\n");
        presioneTeclaParaContinuar();
        return;
    }
    // Volver al inicio y avanzar hasta la posición
    firstList(carrito);
    for (int i = 1; i < pos; i++) nextList(carrito);
    // Elimina el producto actual del carrito y libera la memoria
    Producto *eliminado = popCurrentList(carrito);
    free(eliminado);
    printf("Producto eliminado del carrito.\n");
    presioneTeclaParaContinuar();
}

//Función que muestra los productos en el carrito de compras
void verCarrito(List *carrito) {
    limpiarPantalla();
    //verifica si el carrito está vacío
    Producto *prod = firstList(carrito);
    if (!prod) {
        printf("El carrito está vacío.\n");
        presioneTeclaParaContinuar();
        return;
    }
    // Si hay productos en el carrito, muestra los detalles de cada producto
    printf("Productos en el carrito:\n");
    int idx = 1;
    float total = 0;
    while (prod) {
        printf("%d. %s | Marca: %s | Cantidad: %d | Precio unitario: %.2f | Subtotal: %.2f\n",
            idx, prod->nombre, prod->marca, prod->stock, prod->precioVenta, prod->precioVenta * prod->stock);
        total += prod->precioVenta * prod->stock;
        prod = nextList(carrito);
        idx++;
    }
    // Muestra el total de la compra
    printf("Total: %.2f\n", total);
    presioneTeclaParaContinuar();
}

//Función que confirma la compra de los productos en el carrito
void confirmarCompra(List *carrito, List *historialCompras ,HashMap *productosPorNombre ,HashMap *productosPorCategoria,HashMap *productosPorCodigo, HashMap *contadorProducto) {
    limpiarPantalla();
    float totalCompra = 0.0;
    // Verifica si el carrito está vacío
    if (firstList(carrito) == NULL) {
        printf("El carrito está vacío. No se puede confirmar la compra.\n");
        presioneTeclaParaContinuar();
        return;
    }
    List* compraHecha = createList();
    Producto* producto = firstList(carrito);
    // Muestra los productos que se van a comprar
    printf("Productos en el carrito:\n");
    while (producto != NULL) {
        printf("- %s | Marca: %s | Cantidad: %d | Precio unitario: %.2f\n",
               producto->nombre, producto->marca, producto->stock, producto->precioVenta);
        totalCompra += producto->precioVenta * producto->stock;    
        producto = nextList(carrito);
    }
    // Muestra el total de la compra
    printf("Total de la compra: %.2f\n", totalCompra);
    // Pregunta al usuario si desea confirmar la compra
    printf("\nConfirmar compra? (s/n): ");
    char confirmacion;
    // Se lee la respuesta del usuario
    scanf(" %c", &confirmacion);
    getchar(); // Limpiar buffer
    if (confirmacion != 's' && confirmacion != 'S') {
        printf("Compra cancelada.\n");
        presioneTeclaParaContinuar();
        return;
    }
    producto = firstList(carrito);
    while (producto != NULL) {
        // Busca el producto en el contador de productos
        Pair* pairProd = searchMap(contadorProducto, producto->nombre);
        if (pairProd != NULL) {
            int* vendidos = (int*)pairProd->value;
            *vendidos += producto->stock;
        } else {
            int* vendidos = malloc(sizeof(int));
            *vendidos = 0;
            if (vendidos == NULL) {
                printf("Error al asignar memoria.\n");
                presioneTeclaParaContinuar();
                return;
            }
            *vendidos = producto->stock;
            insertMap(contadorProducto, producto->nombre, vendidos);
        }

        // Busca el producto en el inventario
        Pair* pair = searchMap(productosPorCodigo, producto->codigoBarras);
        if (pair != NULL) {
            Producto* prodInventario = (Producto*)pair->value;

            // Aumenta el contador de vendidos
            prodInventario->vendidos += producto->stock;
            // Agrega el producto al historial de compras
            pushBackList(compraHecha, prodInventario);
        }

        Pair* pairNomb = searchMap(productosPorNombre, producto->nombre);
        if (pairNomb != NULL) {
            List* listaProductosNombre = (List*)pairNomb->value;
            Producto* prodLista = firstList(listaProductosNombre);
            while (prodLista != NULL) {
                if (strcmp(prodLista->codigoBarras, producto->codigoBarras) == 0) {
                    prodLista->vendidos = producto->stock;
                    break;
                }
            prodLista = nextList(listaProductosNombre);
            }
        }

        Pair* pairCategoria = searchMap(productosPorCategoria, producto->categoria);
        if (pairCategoria != NULL) {
            List* listaProductosCategoria = (List*)pairCategoria->value;
            Producto* prodLista = firstList(listaProductosCategoria);
            while (prodLista != NULL) {
                if (strcmp(prodLista->codigoBarras, producto->codigoBarras) == 0) {
                    prodLista->vendidos = producto->stock;
                    break;
                }
            prodLista = nextList(listaProductosCategoria);
            }
        }

        producto = nextList(carrito);
    }

    // Agrega la compra hecha al historial de compras
    pushBackList(historialCompras, compraHecha);
    // Limpia el carrito
    while (firstList(carrito) != NULL) {
        Producto* prod = popCurrentList(carrito);
        free(prod); // Libera la memoria del producto
    }
    // Muestra un mensaje de éxito al confirmar la compra
    printf("Compra confirmada exitosamente.\n");
    presioneTeclaParaContinuar();
}