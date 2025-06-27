#ifndef List_h
#define List_h

typedef struct List List;

// Esta función crea una lista vacía y devuelve un puntero a la lista.
List *createList();

// Esta función devuelve un puntero al primer elemento de la lista.
void *firstList(List *L);

// Esta función mueve el puntero de la lista al siguiente elemento y devuelve un
// puntero a dicho elemento.
void *nextList(List *L);

// Esta función inserta un nuevo elemento al inicio de la lista.
void pushFrontList(List *L, void *dato);

// Esta función inserta un nuevo elemento al final de la lista.
void pushBackList(List *L, void *dato);

// Esta función inserta un nuevo elemento a continuación del actual de la lista.
void pushCurrentList(List *L, void *dato);

// Esta función elimina el primer elemento de la lista.
void *popFrontList(List *L);

// Esta función elimina el último elemento de la lista.
void *popBackLIst(List *L);

// Esta función elimina el elemento actual de la lista.
void *popCurrentList(List *L);

// Esta función elimina todos los elementos de la lista.
void cleanList(List *L);

// Función para insertar ordenado de acuerdo a la función lower_than
void sortedInsertList(List *L, void *data, int (*lower_than)(void *data1, void *data2));

// Esta función devuelve el tamaño de la lista.
int list_size(List* L);

#endif /* List_h */