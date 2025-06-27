## Sinospsis codigo 
Este sistema permite a los usuarios gestionar el inventario de un supermercado. Los usuarios pueden registrar nuevos productos, actualizar stock y consultar el inventario actual. El objetivo es facilitar la administración de productos y asegurar un control eficiente sobre los productos del supermercado. 

Requisitos previos:

• Tener instalado Visual Studio Code.
• Instalar la extensión C/C++
• Tener instalado un compilador de C, como gcc.

Pasos para compilar y ejecutar:

1. Descarga y descomprime el archivo .zip del repositorio o ingrese a travez del link.
2. Abre el proyecto en Visual Studio Code
• Inicia Visual Studio Code.
• Selecciona archivo > abrir carpeta... y elige la carpeta donde descomprimiste el proyecto.
3.   Compila el código
• Abre el terminal integrado
• Compila el programa con el siguiente comando ( gcc -o App Code/*.c -IDeclarations)
4.   Ejecutar el programa
• Una vez compilado, ejecuta la aplicación con: ./App
5. Cargar el csv. llamado "inventario.csv"

## Funciones Principales

Funcion Principal: Este programa tiene una estructura principal "main" que es:

1. Administrador.
2. Cliente.
3. Salir.

Funcion "Administrador": La funcion Administrador, es la principal encargada de permitir la gestion del programa, capaz de realizar busquedas, modificar el inventario, cargar inventario y generar reporte.

1. Cargar inventario
2. Métodos de búsqueda
3. Modificar inventario
4. Guardar inventario
5. Generar reporte completo
0. Salir

Función "Métodos de Busqueda" y "Modificar inventario": Ambas son funciones con menus independietes, con el objetivo de reforzar el funcionamiento de cada una, en el caso de Metodo de busqueda permite realizar busquedas precisas de lo deseado, por otra parte, tenemos Modificar Inventario, que permite una mejor gestion a la hora de registra, eliminar o modificar datos.

"Métodos de Busqueda"
1. Buscar producto por nombre
2. Buscar productos por categoría
3. Mostrar stock de productos
4. Mostrar ventas de productos
0. Salir

"Modificar inventario"

1. Registrar producto
2. Modificar stock de producto
3. Eliminar producto
0. Salir

Funcion "Cliente": La funcion cliente tiene el proposito de realizar compras, para la satisfaccion de los clientes, y obtener informacion de los productos vendidos para realizar el reporte.

1. Agregar al Carrito
2. Eliminar del Carrito
3. Ver Carrito (Productos)
4. Confirmar Compra
0. Salir



## Descripción de las funciones

Funciones de Administrador:

1. Cargar inventario: La funcion primordial para ejecutar el programa, necesita el ingreso escrito del csv. a cargar, esta función lee, al inicio, "inventario.csv" para almacenar cada dato del csv en su respectiva tabla Hash (Nombre, Categoria, Codigo), luego puede leer "inventario_guardado.csv", esto depende si se ejecutar alguna modificacion en el inventario, por el uso de "Modificar Inventario". 
Complejidad Temporal es de O(n) n es la cantidad de datos en el csv.

2. Métodos de búsqueda: La funcion metodos de busqueda abre los submenus que permiten encontrar los productos, ya sea por nombre o por su categoria, cuenta con mas submenus que se explicara mas adelante. 
Complejida Temporal es de O(1) al ser solo un printf

3. Modificar inventario: Muestra un submenú para modificar el inventario (registrar, modificar stock, eliminar producto). Limpia la pantalla y ofrece cuatro opciones. En main.c, se llama cuando el administrador selecciona la opción 3, ejecutando funciones como registrarProducto, modificarStock o eliminarProducto según la selección. 
Complejidad temporal: O(1), por que imprime un menú fijo.

4. Guardar inventario: Guarda el inventario en "inventario_guardado.csv". Escribe un encabezado y recorre productosPorCodigo para guardar los datos de cada producto en formato CSV. En main.c, se usa en la opción 4 del menú de administrador. 
Complejidad temporal: O(n), donde n es el número de productos.

5. Generar reporte completo:



Funciones dentro de Métodos de Busqueda:

1. Buscar producto por nombre: Busca productos por nombre y muestra sus detalles. El terminal debe escribir un nombre para que luego se busque en productosPorNombre, e iterar sobre la lista de productos con ese nombre para mostrar sus datos. Si no encuentra el producto, muestra un mensaje. 
Complejidad temporal: O(1) en promedio para la búsqueda en el hashMap, más O(m) para iterar sobre la lista de productos con el mismo nombre, donde m es el número de productos con ese nombre.

2. Buscar productos por categoría: Busca productos por categoría y muestra sus detalles. Muestra todas las categorías disponibles, incluso si se añade una cateria distinta en registrarProducto, solicita una categoría, busca en productosPorCategoria e itera sobre la lista de productos para mostrarlos. 
Complejidad temporal: O(1) promedio para la búsqueda en el mapa, más O(m) para iterar sobre la lista de productos en la categoría, donde m es el número de productos en esa categoría.

3. Mostrar stock de productos: Muestra productos con stock mayor o menor a un umbral ingresado. Solicita el umbral y la opción (menor igual o mayor igual), itera sobre productosPorCodigo y muestra los productos que cumplen la condición. 
Complejidad temporal: O(n), donde n es el número de productos en el mapa, ya que recorre todas las entradas.

4. Mostrar ventas de productos: Muestra productos con ventas mayores o menores a un umbral. Similar a mostrarProductosStock, pero compara el campo vendidos. 
Complejidad temporal: O(n), donde n es el número de productos, ya que recorre todo el mapa.



Funciones dentro de Modificar inventario : 

1. Registrar producto: Registra un nuevo producto en el inventario. Solicita sus datos para ingresar:  nombre, marca, categoría, código, stock, precios, y verifica que el código de barras sea único, todo esto para insertar el producto en los mapas productosPorCodigo, productosPorNombre y productosPorCategoria.
Complejidad temporal: O(1) promedio para inserciones en mapas, más O(1) para operaciones de lista en la mayoría de los casos.

2. Modificar stock de producto: Solicita el código de barras, busca el producto en productosPorCodigo, muestra el stock actual y actualiza con el nuevo valor en los 3 mapas distintos.
Complejidad temporal: O(1) promedio para la búsqueda en el mapa y la actualizacion de datos.


3. Eliminar producto: Solicita el código de barras, lo busca en productosPorCodigo y lo elimina de los tres mapas, actualizando listas en productosPorNombre y productosPorCategoria
Complejidad temporal: O(1) promedio para la búsqueda en el mapa, más O(m) para eliminar de listas, donde m es el tamaño de la lista correspondiente.



Funciones dentro de Clientes:

1. Agregar al Carrito: Solicita código de barras y cantidad, verifica stock, crea una copia del producto y lo agrega a carrito, actualizando el stock en productosPorCodigo.
Complejidad temporal: O(1) promedio para la búsqueda en el mapa y operaciones de lista.

2. Eliminar del Carrito : Elimina un producto del carrito. Muestra el carrito, solicita la posición del producto y lo elimina con list_popCurrent.
Complejidad temporal: O(n) para recorrer la lista hasta la posición indicada, donde n es el número de productos en el carrito.

3. Ver Carrito (Productos):Muestra el contenido del carrito con subtotales y total. Itera sobre carrito para mostrar detalles y calcular el total.
O(n), donde n es el número de productos en el carrito.

4. Confirmar Compra: Confirma la compra, actualiza el inventario y el historial. Muestra el carrito, solicita confirmación, actualiza contadorProducto y productosPorCodigo, agrega la compra a historialCompras y limpia el carrito.
Complejidad temporal: O(n) para recorrer el carrito, donde n es el número de productos, más O(1) promedio para búsquedas en el mapa.
