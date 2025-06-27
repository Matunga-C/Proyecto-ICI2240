### Sistema de Gestión de Inventario para Supermercado

## Sinopsis código 
Este sistema permite a los usuarios gestionar el inventario de un supermercado. Los usuarios pueden registrar nuevos productos, actualizar stock y consultar el inventario actual. El objetivo es facilitar la administración de productos y asegurar un control eficiente sobre los productos del supermercado. 

## Requisitos previos:

• Tener instalado Visual Studio Code.  
• Instalar la extensión C/C++.  
• Tener instalado un compilador de C, como gcc.

## Pasos para compilar y ejecutar:

1. Descarga y descomprime el archivo .zip del repositorio o ingresa a través del link.
2. Abre el proyecto en Visual Studio Code  
• Inicia Visual Studio Code.  
• Selecciona archivo > abrir carpeta... y elige la carpeta donde descomprimiste el proyecto.
3. Compila el código  
• Abre el terminal integrado  
• Compila el programa con el siguiente comando (gcc -o App Code/*.c -IDeclarations)
4. Ejecutar el programa  
• Una vez compilado, ejecuta la aplicación con: ./App
5. Cargar el csv llamado "inventario.csv"

## Funciones Principales

Función Principal: Este programa tiene una estructura principal "main" que es:

1. Administrador.
2. Cliente.
3. Salir.

Función "Administrador": La función Administrador es la principal encargada de permitir la gestión del programa, capaz de realizar búsquedas, modificar el inventario, cargar inventario y generar reporte.

1. Cargar inventario
2. Métodos de búsqueda
3. Modificar inventario
4. Guardar inventario
5. Generar reporte completo
0. Salir

Función "Métodos de Búsqueda" y "Modificar inventario": Ambas son funciones con menús independientes, con el objetivo de reforzar el funcionamiento de cada una. En el caso de Método de búsqueda permite realizar búsquedas precisas de lo deseado, por otra parte, tenemos Modificar Inventario, que permite una mejor gestión a la hora de registrar, eliminar o modificar datos.

"Métodos de Búsqueda"
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

Función "Cliente": La función cliente tiene el propósito de realizar compras, para la satisfacción de los clientes, y obtener información de los productos vendidos para realizar el reporte.

1. Agregar al Carrito
2. Eliminar del Carrito
3. Ver Carrito (Productos)
4. Confirmar Compra
0. Salir

## Descripción de las funciones

# Funciones de Administrador:

1. Cargar inventario: La función primordial para ejecutar el programa, necesita el ingreso escrito del csv a cargar. Esta función lee, al inicio, "inventario.csv" para almacenar cada dato del csv en su respectiva tabla Hash (Nombre, Categoría, Código), luego puede leer "inventario_guardado.csv", esto depende si se ejecuta alguna modificación en el inventario, por el uso de "Modificar Inventario".  
Complejidad Temporal es de O(n), n es la cantidad de datos en el csv.

2. Métodos de búsqueda: La función métodos de búsqueda abre los submenús que permiten encontrar los productos, ya sea por nombre o por su categoría, cuenta con más submenús que se explicarán más adelante.  
Complejidad Temporal es de O(1) al ser solo un printf.

3. Modificar inventario: Muestra un submenú para modificar el inventario (registrar, modificar stock, eliminar producto). Limpia la pantalla y ofrece cuatro opciones. En main.c, se llama cuando el administrador selecciona la opción 3, ejecutando funciones como registrarProducto, modificarStock o eliminarProducto según la selección.  
Complejidad temporal: O(1), porque imprime un menú fijo.

4. Guardar inventario: Guarda el inventario en "inventario_guardado.csv". Escribe un encabezado y recorre productosPorCodigo para guardar los datos de cada producto en formato CSV. En main.c, se usa en la opción 4 del menú de administrador.  
Complejidad temporal: O(n), donde n es el número de productos.

5. Generar reporte completo:



# Funciones dentro de Métodos de Búsqueda:

1. Buscar producto por nombre: Busca productos por nombre y muestra sus detalles. El terminal debe escribir un nombre para que luego se busque en productosPorNombre, e iterar sobre la lista de productos con ese nombre para mostrar sus datos. Si no encuentra el producto, muestra un mensaje.  
Complejidad temporal: O(1) en promedio para la búsqueda en el hashMap, más O(m) para iterar sobre la lista de productos con el mismo nombre, donde m es el número de productos con ese nombre.

2. Buscar productos por categoría: Busca productos por categoría y muestra sus detalles. Muestra todas las categorías disponibles, incluso si se añade una categoría distinta en registrarProducto, solicita una categoría, busca en productosPorCategoria e itera sobre la lista de productos para mostrarlos.  
Complejidad temporal: O(1) promedio para la búsqueda en el mapa, más O(m) para iterar sobre la lista de productos en la categoría, donde m es el número de productos en esa categoría.

3. Mostrar stock de productos: Muestra productos con stock mayor o menor a un umbral ingresado. Solicita el umbral y la opción (menor igual o mayor igual), itera sobre productosPorCodigo y muestra los productos que cumplen la condición.  
Complejidad temporal: O(n), donde n es el número de productos en el mapa, ya que recorre todas las entradas.

4. Mostrar ventas de productos: Muestra productos con ventas mayores o menores a un umbral. Similar a mostrarProductosStock, pero compara el campo vendidos.  
Complejidad temporal: O(n), donde n es el número de productos, ya que recorre todo el mapa.

# Funciones dentro de Modificar inventario: 

1. Registrar producto: Registra un nuevo producto en el inventario. Solicita sus datos para ingresar: nombre, marca, categoría, código, stock, precios, y verifica que el código de barras sea único, todo esto para insertar el producto en los mapas productosPorCodigo, productosPorNombre y productosPorCategoria.  
Complejidad temporal: O(1) promedio para inserciones en mapas, más O(1) para operaciones de lista en la mayoría de los casos.

2. Modificar stock de producto: Solicita el código de barras, busca el producto en productosPorCodigo, muestra el stock actual y actualiza con el nuevo valor en los 3 mapas distintos.  
Complejidad temporal: O(1) promedio para la búsqueda en el mapa y la actualización de datos.


3. Eliminar producto: Solicita el código de barras, lo busca en productosPorCodigo y lo elimina de los tres mapas, actualizando listas en productosPorNombre y productosPorCategoria.  
Complejidad temporal: O(1) promedio para la búsqueda en el mapa, más O(m) para eliminar de listas, donde m es el tamaño de la lista correspondiente.



# Funciones dentro de Clientes:

1. Agregar al Carrito: Solicita código de barras y cantidad, verifica stock, crea una copia del producto y lo agrega a carrito, actualizando el stock en productosPorCodigo.  
Complejidad temporal: O(1) promedio para la búsqueda en el mapa y operaciones de lista.

2. Eliminar del Carrito: Elimina un producto del carrito. Muestra el carrito, solicita la posición del producto y lo elimina con list_popCurrent.  
Complejidad temporal: O(n) para recorrer la lista hasta la posición indicada, donde n es el número de productos en el carrito.

3. Ver Carrito (Productos): Muestra el contenido del carrito con subtotales y total. Itera sobre carrito para mostrar detalles y calcular el total.  
O(n), donde n es el número de productos en el carrito.

4. Confirmar Compra: Confirma la compra, actualiza el inventario y el historial. Muestra el carrito, solicita confirmación, actualiza contadorProducto y productosPorCodigo, agrega la compra a historialCompras y limpia el carrito.  
Complejidad temporal: O(n) para recorrer el carrito, donde n es el número de productos, más O(1) promedio para búsquedas en el mapa.

## Ejemplo de uso

# Registrar un producto:
=== Registro de Producto ===
 Ingrese el nombre del producto: Pan 
 Ingrese la marca del producto: Axion 
 Ingrese la categoría del producto: Comestibles 
 Ingrese el código de barras del producto: 4664430222248 
 Ingrese el stock del producto: 83 
 Ingrese el precio de venta del producto: 81.88 
 Ingrese el precio de mercado del producto: 92.95 
 Ingrese el precio de costo del producto: 64.23
 Producto registrado exitosamente.
 Presione una tecla para continuar...

# Buscar producto por nombre:
Ingrese el nombre del producto a buscar: pan

Nombre: pan 
Marca: axion 
Categoría: comestibles 
Código de barras: 4664430222248 
Stock: 83 
Precio de venta: 81.88 
Precio de mercado: 92.95 
Precio de costo: 64.23 
Vendidos: 0
Presione una tecla para continuar...

# Agregar al carrito y confirmar compra

Ingrese el código de barras del producto a agregar: 4664430222248 
Ingrese la cantidad a agregar: 2 
Producto agregado al carrito. 
Presione una tecla para continuar...

Productos en el carrito:

pan | Marca: axion | Cantidad: 2 | Precio unitario: 81.88 | Subtotal: 163.76 Total: 163.76 
Presione una tecla para continuar...

Productos en el carrito:

pan | Marca: axion | Cantidad: 2 | Precio unitario: 81.88
Confirmar compra? (s/n): s 
Compra confirmada exitosamente. 
Presione una tecla para continuar...


## Notas y consideraciones
- El sistema utiliza estructuras hash para búsquedas eficientes.
- Los archivos CSV deben tener el formato adecuado.
- Si tienes problemas al compilar, revisa que las rutas y nombres de archivos sean correctos.
- Si se quiere leer nuevamente el csv o leer el csv guardado se debe reiniciar el programa.

## Mejoras sugeridas
- Implementar exportación/importación avanzada de inventario.
- Añadir interfaz gráfica.
- Permitir edición masiva de productos.