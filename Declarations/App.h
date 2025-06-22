#ifndef App_h
#define App_h

#include "Map.h"
#include "List.h"
#include "utils.h"

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


void presioneTeclaParaContinuar();

void limpiarPantalla();

void mostrarMenuAdministrador();

void mostrarMenuCliente();

void menuBusqueda();

void menuModificarInventario();

void mostrarVentasProductos(HashMap *productosPorCodigo);

void registrarProducto(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap *productosPorNombre);

void buscarProductoPorNombre(HashMap *productosPorNombre);

void modificarStock(HashMap *productosPorCodigo);

void eliminarProducto(HashMap *productosPorCodigo, HashMap *productosPorCategoria);

void guardarInventario(HashMap *productosPorCodigo);

void cargarInventario(char* nameFile, HashMap *productosPorNombre, HashMap *productosPorCodigo, HashMap *productosPorCategoria);

void listarProductosPorCategoria(HashMap *productosPorCategoria);

void mostrarProductosStock(HashMap *productosPorCategoria);

void agregarAlCarrito(HashMap *productosPorCodigo, List *carrito);

void eliminarDelCarrito(List *carrito);

void confirmarCompra(List *carrito, List *historialCompras ,HashMap *productosPorCodigo);

void verCarrito(List *carrito);

void sugerirPromociones(HashMap *productosPorCodigo, HashMap *productosPorCategoria);

#endif