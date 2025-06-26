## Sinospsis codigo 
Este sistema permite a los usuarios gestionar el inventario de un supermercado. Los usuarios pueden registrar nuevos productos, actualizar stock y consultar el inventario actual. El objetivo es facilitar la administración de productos y asegurar un control eficiente sobre los productos del supermercado. 

Requisitos previos:

• Tener instalado Visual Studio Code.
• Instalar la extensión C/C++
• Tener instalado un compilador de C, como gcc.

Pasos para compilar y ejecutar:

1. Descarga y descomprime el archivo .zip del repositorio.
2. Abre el proyecto en Visual Studio Code
• Inicia Visual Studio Code.
• Selecciona archivo > abrir carpeta... y elige la carpeta donde descomprimiste el proyecto.
3.   Compila el código
• Abre el terminal integrado
• Compila el programa con el siguiente comando ( gcc -o App Code/*.c -IDeclarations)
4.   Ejecutar el programa
• Una vez compilado, ejecuta la aplicación con: ./App

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

1. Cargar inventario: La funcion primordial para ejecutar el programa, necesita el ingreso escrito del csv. a cargar, esta función lee, al inicio, "inventario.csv" para almacenar cada dato del csv en su respectiva tabla Hash (Nombre, Categoria, Codigo), luego puede leer "inventario_guardado.csv", esto depende si se ejecutar alguna modificacion en el inventario, por el uso de "Modificar Inventario". Su complejidad Temporal es de O(n) n es la cantidad de datos en el csv.

2. Métodos de búsqueda: La funcion metodos de busqueda abre los submenus que permiten encontrar los productos, ya sea por nombre o por su categoria, cuenta con mas submenus que se explicara mas adelante. 

3. Modificar inventario
4. Guardar inventario
5. Generar reporte completo