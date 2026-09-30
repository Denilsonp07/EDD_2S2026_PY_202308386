# Manual De Usuario - CinemaUsac

Este entorno permite cargar compilar y simular un sistema de cine mediante el lenguaje de programacion c++ y Qt para la UI.

## Login 
Se muestra la vista del loguin donde se puede ingresar ya sea siendo un Administrador o un Cliente. Para la administracion del sistema las credenciales son: 
Correo: AdminCine@gmail.com
Contraseña: admin123Pass

> ![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20153915.png)

## Loguin Cliente
En caso de ser un cliente sin credenciales se debe oprimir el boton de registro que desplegara un menu opcional para un registro rapido del clinte.

> ![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155154.png)

## Panel del Administrador 
En el panel de administrador se encuentran varios apartados. Con el primero que nos encontraremos es con la carga masiva de los respectivos archivos .JSON

> ![PanelAdministrador](imagenes/Captura%20de%20pantalla%202026-09-17%20155412.png)

En este apartado se encuentran dos botones cargar Peliculas que al presionarlo abrira el administrador de archivos para seleccionar el respectivo archivo.

> ![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155444.png)

Asi es como se ve cargado en la tabla respectiva de peliculas.

Para cargar los clientes es de la misma manera selecciona el boton Cargar Clientes y abrira el administrador de archivos para seleccionar el archivo y desplegara los clientes registrados.

![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155456.png)
![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155505.png)

## Gestion de Peliculas 
En este apartado se pueden gestionar todas las peliculas cargadas anteriormente. Cuenta con 3 botones y un combo box. Para seleccionar el orden en que se desea mostrar las peliculas (Inorden, Preorden, Posorden) 1. Agregar 2. Editar. Eliminar. Estos 3 botones tienen la respectiva funcionalidad de agregar una pelicula editarla y/o eliminarla. 

## Tabla con las peliculas cargadas
![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155514.png)
## Agregrar Pelicula
![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155524.png)
## Editar Pelicula
![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155536.png)
## Eliminar Pelicula
![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155548.png)

## Gestion de Clientes
En este apartado se manejan la admistracion de todos los clientes. Cuenta con un boton y un combo box que sirve para manejar el orden en que se muestran los clientes. El boton eliminar para eliminar del sistema a un cliente por numero de ID.

## Tabla con los clientes cargados
![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155558.png)
## Eliminar Cliente por ID
![Login](imagenes/Captura%20de%20pantalla%202026-09-17%20155620.png)

## Apartado de cartelera
en este apartado se muestran las peliculas en su respectiva cartelera, tiene la funcionalidad de crear una sala con un numero determinado de filas y colunmas que representan los asientos de la funcion. El boton crear funcion donde al seleccionar una pelicula se creara su sala con los asientos que fueron seleccionados anteriormente.


![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161521.png)

## Boton crear funcion 
Despliega apartados para seleccionar el horario sala y demas apartados.

![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161533.png)


## Apartado Mapa de asientos
Se vera una cuadricula con la Pelicula, sala y horario respectivo y los asientos libres en verde, mientras que los ocupados en rojo. 

## Mapa de asientos
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161548.png)


## Apartado de Cliente
Menu que contiene diferentes apartados para el manejos de Cartelera y asientos, asi como reservar asientos y asi mismo cancelarlos. Y cuenta con el apartado de Mi perfil e Historial donde se pueden ver las credenciales del usuario logueado.

## Apartado cartelera y asientos 
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161710.png)

## Manejo de reservas
Para realizar una reserva se debe seleccionar del  menu desplegable la pelicula a la cual se le haya creado una funcion anteriormente, Seleccionar el horario deseado. Se puede seleccionar el asiento deaseado escribiendo el numero de fila y de columna que se desee al presionar el boton Rervar el asiento automaticamente se pondra en rojo y con el nombre de la persona quien lo aparto.

![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161727.png)


##  Apartado Mi Perfil e Historial

Aca se muestran todas todas las credenciales del cliente logueado como Nombre, Correo, Telefono, ademas de ver los asientos reservados que posee el cliente. Tambien contine dos botones uno para la cancelacion de reservas y otro para actualizar el perfil si el usuario lo requiere.

## Apartado mi perfi e Historial
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161758.png)
## Boton cancelar Reserva
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161812.png)
## Apartado Editar Perfil
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161831.png)

## Manejo de reportes
Este apartado se encuentra en la gestion del administrador. Cuenta con 5 botones uno para cada respectivo reporte donde se muestra el png de la estructura usada. 

## Reporte 1 
Árbol binario de búsqueda (Cartelera de Películas)
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161913.png)

## Reporte 2
Árbol AVL de Funciones
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161930.png)

## Reporte 3
Árbol B de orden 4 de Clientes
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20161947.png)

## Reporte 4
Tabla Hash de Reservas
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20162005.png)

## Reporte 5
Matriz Dispersa (Mapa de Asientos por Función)
![Cartelera](imagenes/Captura%20de%20pantalla%202026-09-17%20162023.png)

