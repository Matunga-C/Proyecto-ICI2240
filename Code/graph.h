/*#ifndef tda_h
#define tda_h

typedef struct Graph Graph;



typedef struct {
    char nombre[50];
    char categoria[30];
    char codigoBarras[13];
    int stock;
    int precio;
    char proveedor[50];
} Producto;

typedef struct NodoProducto {
    Producto producto;
    struct NodoProducto *siguiente;
} NodoProducto;

typedef struct {
    NodoProducto *cabeza;
    int cantidad;
} ListaProductos;

void inicializar_lista(ListaProductos *lista);
void agregar_producto_lista(ListaProductos *lista, Producto producto);
Producto* buscar_producto_lista(ListaProductos *lista, const char *nombre);
void eliminar_producto_lista(ListaProductos *lista, const char *nombre);
void liberar_lista(ListaProductos *lista);


typedef struct NodoMapa {
    char clave[50]; 
    Producto *producto;
    struct NodoMapa *siguiente;
} NodoMapa;

typedef struct {
    NodoMapa **tabla;
    int capacidad;
} MapaProductos;

MapaProductos* crear_mapa(int capacidad);
void insertar_mapa(MapaProductos *mapa, const char *clave, Producto *producto);
Producto* buscar_mapa(MapaProductos *mapa, const char *clave);
void eliminar_mapa(MapaProductos *mapa, const char *clave);
void liberar_mapa(MapaProductos *mapa);

#endif*/