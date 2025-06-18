#ifndef List_h
#define List_h

typedef struct List List;

// Esta función crea una lista vacía y devuelve un puntero a la lista.
List list_create();

// Esta función devuelve un puntero al primer elemento de la lista.
voidlist_first(List L);

// Esta función mueve el puntero de la lista al siguiente elemento y devuelve un
// puntero a dicho elemento.
voidlist_next(List L);

// Esta función inserta un nuevo elemento al inicio de la lista.
void list_pushFront(ListL, void dato);

// Esta función inserta un nuevo elemento al final de la lista.
void list_pushBack(ListL, void dato);

// Esta función inserta un nuevo elemento a continuación del actual de la lista.
void list_pushCurrent(ListL, void dato);

// Esta función elimina el primer elemento de la lista.
voidlist_popFront(List L);

// Esta función elimina el último elemento de la lista.
voidlist_popBack(List L);

// Esta función elimina el elemento actual de la lista.
voidlist_popCurrent(List L);

// Esta función elimina todos los elementos de la lista.
void list_clean(ListL);

// Función para insertar ordenado de acuerdo a la función lower_than
void list_sortedInsert(List L, voiddata,
                       int (lower_than)(voiddata1, void data2));

int list_size(List L);

#endif /* List_h */