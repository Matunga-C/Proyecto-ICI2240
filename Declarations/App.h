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

typedef struct {
    char nombre[51];
    int cantidadVentas;
} NombreVenta;

typedef struct{
    char nombreA[51];
    char nombreB[51];
    int frecuencia;
} combo;

void presioneTeclaParaContinuar();

void limpiarPantalla();

void mostrarMenuAdministrador();

void mostrarMenuCliente();

void menuBusqueda();

void menuModificarInventario();

void mostrarVentasProductos(HashMap *productosPorCodigo);

void registrarProducto(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap *productosPorNombre);

void buscarPorNombre(HashMap *productosPorNombre);

void modificarStock(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap *productosPorNombre);

void eliminarProducto(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap *productosPorNombre);

void guardarInventario(HashMap *productosPorCodigo);

void cargarInventario(char* nameFile, HashMap *productosPorNombre, HashMap *productosPorCodigo, HashMap *productosPorCategoria);

void buscarPorCategoria(HashMap *productosPorCategoria);

void mostrarProductosStock(HashMap *productosPorCategoria);

void agregarAlCarrito(HashMap *productosPorCodigo, List *carrito);

void eliminarDelCarrito(List *carrito);

void confirmarCompra(List *carrito, List *historialCompras ,HashMap *productosPorNombre ,HashMap *productosPorCategoria,HashMap *productosPorCodigo, HashMap *contadorProducto);

void generarReporte(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap* productosPorNombre,List *historialCompras, HashMap *contadorProducto);

void verCarrito(List *carrito);

void sugerirPromociones(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap* contadorProducto, List *productosPocasVentas);

void realizarDescuento(HashMap *productosPorCodigo, HashMap *productosPorCategoria, HashMap *productosPorNombre ,HashMap* contadorProducto, List *productosPocasVentas);

#endif