# Manual Tecnico - CinemaUsac

El Sistema de Gestión CINEMA es una plataforma de software de alto rendimiento desarrollada en C++ con la interfaz gráfica Qt. Este proyecto trasciende la funcionalidad básica de una taquilla electrónica, consolidándose como una solución integral que maneja eficientemente el flujo completo de una cadena de cines: desde la carga masiva de carteleras hasta la selección interactiva de asientos. Su diseño separa de manera segura las operaciones administrativas del lado del servidor (backend) y la experiencia de usuario del lado del cliente (frontend).

Módulos Principales de Operación
Módulo de Acceso (Login): Actúa como la puerta de seguridad lógica del sistema. Clasifica instantáneamente el tipo de usuario mediante validación de credenciales. Los administradores ingresan mediante credenciales estáticas predefinidas, mientras que los clientes son buscados y validados dinámicamente dentro de la base de datos estructurada. Además, proporciona una vía rápida para el registro de nuevos usuarios en el sistema.

Panel de Administrador: Es el centro de mando operativo. Permite la ingesta de grandes volúmenes de datos mediante la lectura de archivos JSON, automatizando la creación del catálogo de películas, horarios, clientes y reservas históricas. Facilita la auditoría del sistema mediante la generación de reportes visuales arquitectónicos (Graphviz) que muestran el estado exacto de la memoria y la distribución de los datos en tiempo real.

Panel de Cliente: Representa el entorno interactivo del usuario final. Permite navegar por la cartelera activa, revisar las promociones vigentes según la fecha del sistema, y gestionar su perfil personal. Su característica más importante es el módulo de taquilla, donde el cliente puede seleccionar una función, visualizar el mapa de la sala en una cuadrícula interactiva, realizar nuevas reservas en tiempo real y cancelar boletos previamente adquiridos.


# Estructuras:

La verdadera fortaleza de este desarrollo radica en la implementación nativa y manual de estructuras de datos no lineales. En lugar de depender de librerías estándar genéricas, el sistema construye su propia arquitectura de gestión de memoria dinámica para resolver problemas algorítmicos específicos con la máxima eficiencia posible.La selección de cada estructura responde a un requisito técnico riguroso:

Árbol Binario de Búsqueda (Películas): Mantiene el catálogo principal organizado jerárquicamente. Su importancia radica en reducir drásticamente el tiempo de búsqueda secuencial, permitiendo localizar cualquier cinta cinematográfica descartando la mitad de las opciones en cada paso del recorrido.

Árbol AVL (Funciones y Horarios): Las carteleras cambian constantemente. El algoritmo auto-balanceable de este árbol garantiza matemáticamente que ninguna rama crecerá desproporcionadamente. Esto asegura que el tiempo de consulta de un horario siempre será estrictamente logarítmico, sin importar cuántas funciones se agreguen o eliminen.

Árbol B de Orden 4 (Clientes): Diseñado para manejar volúmenes masivos de usuarios. Al agrupar múltiples clientes en nodos anchos en lugar de profundos, reduce la altura general del árbol. Es la estructura estándar en bases de datos profesionales porque minimiza los saltos de memoria requeridos para autenticar a un usuario durante el login.

Tabla Hash (Reservaciones): Al procesar la compra o cancelación de boletos, la velocidad es crítica. La función matemática de la tabla convierte el código del boleto directamente en una dirección de memoria, otorgando un acceso instantáneo de tiempo constante O(1). Las listas enlazadas internas resuelven eficientemente cualquier colisión de códigos.

Matriz Dispersa Ortogonal (Salas y Asientos): Optimiza agresivamente el consumo de memoria RAM. En lugar de crear matrices bidimensionales gigantes llenas de espacios vacíos para cada sala del cine, esta estructura interconecta únicamente los asientos que han sido vendidos. Representa una solución matemática elegante que escala perfectamente sin desperdiciar recursos del sistema.


# Árbol Binario de Búsqueda de Películas
ste archivo de cabecera implementa la estructura central para la gestión del catálogo de películas del cine. Se diseñó basándose en un Árbol Binario de Búsqueda (BST) para garantizar tiempos de acceso, inserción y eliminación eficientes.


## Metodos principales:

![MetodosPrincipales](imagenes/Captura%20de%20pantalla%202026-09-17%20171802.png)

### Clase Principal: ArbolBinarioDeBusquedaPeliculas

Controla la lógica matemática y de persistencia del árbol. Utiliza el paradigma de encapsulamiento: métodos públicos simples que invocan métodos recursivos privados complejos.

### NodoArbolPeliculas* insertarNodo(NodoArbolPeliculas* nodo, const Pelicula& pelicula);

Recorre el árbol recursivamente evaluando si el código de la nueva película es mayor o menor que el nodo actual. Retorna el nodo modificado para re-enlazar la estructura correctamente.

### NodoArbolPeliculas* eliminarNodo(NodoArbolPeliculas* nodo, int codigoNumerico, bool& eliminado)

Localiza y elimina un nodo aplicando las tres reglas del BST:

1. Si es hoja: se elimina directamente.
2. Si tiene un hijo: el hijo sube a reemplazar al padre.
3. Si tiene dos hijos: busca el sucesor inorden (mediante obtenerNodoMinimo), copia sus    datos al nodo actual y luego elimina el sucesor.

### void recolectarInorden / Preorden / Postorden (...)

Funciones recursivas que recorren el árbol en sus tres variantes algorítmicas (Izquierda-Raíz-Derecha, Raíz-Izquierda-Derecha, Izquierda-Derecha-Raíz) y van inyectando cada película encontrada dentro del objeto referenciado ListaPeliculas.

### void cargarJSON(const string& ruta, ArbolAVLFunciones& arbolFunciones);

Abre un archivo JSON físico y extrae arreglos masivos de películas. Además de guardar las películas en el BST, extrae el sub-arreglo anidado de "funciones" y las inyecta directamente en la estructura AVL que recibe como parámetro, conectando ambos módulos del backend.

### Pelicula* buscarPorCodigo(const string& codigo)

Convierte el código string (ej. "P005") a entero (5) e itera el árbol. Si encuentra una coincidencia absoluta, utiliza getPunteroPelicula() para devolver el acceso directo al objeto; si no, retorna nullptr.

### bool generarReporteCartelera(const string& rutaDot, const string& rutaPng, const string& fechaActual) const

Automatiza la creación del reporte visual usando Graphviz. Genera un archivo .dot con la sintaxis requerida, coloreando o marcando los nodos según las reglas de negocio (como alertas de fechas), y ejecuta un comando de consola (system()) para compilarlo en PNG.



# Árbol Árbol AVL de Funciones
Este archivo define la arquitectura para gestionar las "Funciones" (horarios y salas asignadas) de las películas. Se implementó como un Árbol AVL (un árbol binario de búsqueda auto-balanceable) porque las carteleras de cine requieren búsquedas instantáneas y tiempos de ejecución garantizados de O(log n) en todos sus casos (inserción, eliminación y búsqueda).A diferencia del Árbol Binario tradicional, esta estructura monitorea matemáticamente la "altura" de sus ramas. Si la diferencia de altura entre la rama izquierda y derecha de cualquier nodo excede el límite permitido (factor de equilibrio mayor a 1 o menor a -1), el árbol ejecuta "rotaciones" automáticas para reestructurarse y no perder su eficiencia.


## Metodos principales:

![MetodosPrincipales](imagenes/Captura%20de%20pantalla%202026-09-17%20172707.png)

### Componente Estructural:
Define la unidad básica del árbol.

class NodoAVL {
public:
    Funcion funcion;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;

    NodoAVL(const Funcion& fun) : funcion(fun), izq(nullptr), der(nullptr), altura(1) {}
};

Además de almacenar el objeto Funcion y los punteros clásicos de navegación (izq, der), introduce el atributo crítico altura. Todo nodo nuevo nace con una altura de 1, la cual se va recalculando dinámicamente conforme se agregan o eliminan ramas debajo de él.

### Clase Principal: ArbolAVLFunciones

Controla la matemática del balanceo y la persistencia en memoria.

### int obtenerEquilibrio(NodoAVL* nodo)

Es el sensor central del árbol. Resta la altura del hijo izquierdo menos la altura del hijo derecho. Retorna un entero (usualmente -1, 0 o 1). Si devuelve valores como 2 o -2, detona inmediatamente las funciones de rotación.

### odoAVL* rotacionDerecha(NodoAVL* y) / rotacionIzquierda(NodoAVL* x)

Son los algoritmos de corrección. Si el árbol se desbalancea hacia un lado, estas funciones reasignan los punteros izq y der entre tres nodos involucrados, haciendo que el hijo suba a la posición del padre y el padre baje a ser hijo, restaurando el equilibrio físico sin alterar el orden lógico de los códigos. Actualizan la altura de los nodos afectados al finalizar.

### int extraerNumero(const string& codigo)

Función auxiliar vital que toma cadenas como "F025", elimina la letra inicial y parsea el resto a un entero (25). Permite que el árbol ordene los datos matemáticamente y no alfabéticamente.

### NodoAVL* insertarNodo(NodoAVL* nodo, const Funcion& funcion)

1. Método recursivo que realiza la inserción en tres fases:

2. Inserción normal de BST guiándose por el valor numérico del código.

3. Actualización de la variable altura del nodo padre.

Evaluación del factor de equilibrio. Si se rompe, determina qué tipo de desbalance ocurrió (Izquierda-Izquierda, Derecha-Derecha, Izquierda-Derecha o Derecha-Izquierda) y aplica la rotación correspondiente.

### Funcion* buscar(const string& codigo);

Recibe el código string ("F001"), extrae el número y desciende por el árbol AVL comparando valores. Al encontrar el nodo exacto, devuelve un puntero a la dirección de memoria de la Funcion. Esto permite que la interfaz gráfica u otras estructuras interactúen con la función real y no con una copia temporal. Retorna nullptr si no existe.

### bool eliminarPorCodigo(const string& codigo);

Función "envoltorio" (wrapper) que llama al método privado eliminarNodo. Además de buscar y borrar la función liberando la memoria, este método garantiza que el árbol se rebalancee completamente desde el punto de eliminación hasta la raíz. Devuelve true si se eliminó con éxito.



# Árbol B de Clientes

Este archivo define la estructura de almacenamiento principal para los perfiles de los usuarios del cine. Se implementó utilizando un Árbol B de Orden 4. A diferencia de los árboles binarios, el Árbol B permite almacenar múltiples claves (clientes) y punteros dentro de un mismo nodo.Esta estructura se eligió específicamente porque es el estándar en bases de datos reales: al ser "chato y ancho", reduce drásticamente la altura del árbol, permitiendo búsquedas, inserciones y eliminaciones en tiempo logarítmico O(log n) incluso con volúmenes masivos de datos. Al ser de orden 4, la regla matemática dicta que cada nodo puede contener un máximo de 3 clientes y tener hasta 4 hijos.

## Metodos Principales

![MetodosPrincipales](imagenes/Captura%20de%20pantalla%202026-09-17%20173723.png)

### Componente Estructural: NodoB

Define el bloque de construcción del árbol adaptado a la restricción del Orden 4.

class NodoB {
public:
    Cliente* clientes[3]; // Max 3 claves por nodo para orden 4
    NodoB* hijos[4];      // Max 4 punteros a hijos
    int numClaves;        // Cuantos clientes hay actualmente en este nodo
    bool esHoja;          // Verdadero si no tiene hijos
    // ... constructor ...
};

En lugar de tener un solo dato, utiliza un arreglo de punteros clientes[3] para guardar las claves ordenadas (por el DPI/ID numérico) y un arreglo hijos[4] para las ramificaciones. La variable numClaves controla cuántos espacios del arreglo están ocupados actualmente.

### Clase Principal: ArbolBClientes

Gestiona la compleja matemática de divisiones y fusiones de los nodos para garantizar que el árbol permanezca perfectamente balanceado en todas sus hojas.

### void dividirHijo(NodoB* padre, int i, NodoB* hijoLleno)

Es el corazón de la inserción en un Árbol B. Si al intentar insertar un cliente el nodo destino ya tiene 3 claves (está lleno), esta función "rompe" el nodo por la mitad. El elemento central sube al nodo padre, y los elementos restantes se separan en un nuevo nodo hermano.

### void fusionar(NodoB* nodo, int indice) y void pedirPrestado...

ontrolan el underflow durante la eliminación. Si al borrar un cliente un nodo se queda con menos claves del mínimo permitido (1 clave para orden 4), el árbol primero intenta "robarle" (pedir prestado) un cliente a su nodo hermano. Si el hermano también está al límite, se ejecuta fusionar(), que une a los dos hermanos y baja al padre para formar un solo nodo lleno.

### void cargarJSON(const string& ruta, TablaHash& tablaReservas);

Lee el archivo de datos masivos. Extrae el objeto JSON del cliente, lo inserta en el Árbol B, e inmediatamente extrae el arreglo anidado de "reservas", enviando esos datos secundarios a la TablaHash global que recibe como parámetro, vinculando ambos sistemas.

### bool eliminarCliente(const string& id, TablaHash& tablaReservas);

Ejecuta un borrado en cascada (Drop Cascade). Primero busca al cliente por su ID. Si lo encuentra, extrae la lista de todos sus códigos de reserva y llama al método eliminar de la tablaReservas para liberar esos asientos. Finalmente, elimina el nodo del cliente en el Árbol B y rebalancea la estructura.

### Cliente* buscarPorCorreo(const std::string& correo);

l Árbol B está ordenado matemáticamente por el ID (DPI), pero el sistema de login del cine exige buscar por correo. Este método realiza un recorrido exhaustivo por todas las ramas buscando una coincidencia exacta de correo, devolviendo el puntero directo al objeto Cliente para validar la contraseña o editar el perfil en la interfaz.

### void generarReporte() y generarDotAux(...)

Crea la representación visual de la estructura para Graphviz. En un Árbol B, exige el uso del atributo shape=record en la sintaxis DOT para dibujar los nodos como cajas divididas en compartimentos (mostrando los 3 espacios disponibles para clientes), enlazando visualmente a los hijos desde compartimentos específicos del nodo padre.

# Tabla Hash de Reservas

Propósito e Implementación:

GeneralEste archivo define la estructura de almacenamiento ultra-rápida para las reservaciones del cine. Se implementó utilizando una Tabla Hash con manejo de colisiones por Listas Separadas (encadenamiento).Las tablas hash son ideales para este propósito porque ofrecen tiempos de acceso, inserción y eliminación de O(1) (tiempo constante) en su caso promedio. Cuando un cliente o el administrador necesita cancelar o consultar una reserva (ej. "R005"), el sistema no tiene que recorrer toda la estructura; una función matemática calcula instantáneamente la posición exacta en la memoria. El uso de un número primo (cap = 11) como tamaño inicial es una práctica matemática estándar para evitar agrupamientos (clustering) y distribuir los datos uniformemente.

## Metodos Principales:![MetodosPrincipales](imagenes/Captura%20de%20pantalla%202026-09-17%20174606.png)


### Componente Estructural Auxiliar: NodoHash** tabla

Aunque la clase NodoHash no está definida en este bloque, la declaración NodoHash** tabla (un puntero a un puntero) nos indica exactamente cómo funciona la arquitectura de colisiones:

Es un arreglo dinámico principal (los buckets o cubetas).

Cada espacio del arreglo no guarda una reserva directamente, sino que guarda un puntero hacia el inicio de una lista enlazada simple de tipo NodoHash. Si dos reservas arrojan el mismo índice matemático, simplemente se encadenan en esa lista.


### Clase Principal: TablaHash

Gestiona la conversión matemática de las claves alfanuméricas y vigila el rendimiento de la estructura para evitar la degradación del tiempo de búsqueda.


### int funcionHash(const string& clave);

Es el núcleo de la estructura. Toma el codigo_reserva (ej. "R015"), lo procesa a nivel de caracteres (generalmente sumando el valor ASCII de sus letras/números y aplicando la operación módulo % capacidad) para generar y retornar un índice numérico válido dentro de los límites actuales del arreglo.


### void rehash();

Es el mecanismo de autoprotección del rendimiento. Cuando la cantidadElementos supera un porcentaje límite respecto a la capacidad (el Factor de Carga, usualmente mayor al 70%), las listas enlazadas se vuelven muy largas y se pierde el O(1). Este método crea un nuevo arreglo dinámico más grande (típicamente el doble del tamaño actual, buscando el siguiente número primo), recalcula el hash para cada reserva existente y las transfiere a la nueva tabla, liberando la antigua.


### void insertar(const Reserva& reserva)

Recibe el objeto Reserva, extrae su código y lo pasa a la funcionHash para obtener un índice. Instancia un nuevo NodoHash y lo inserta al inicio de la lista enlazada ubicada en ese índice. Incrementa el contador global y evalúa si es necesario disparar el rehash().

### Reserva* buscar(const string& codigo_reserva);

Convierte el código buscado en un índice matemático, salta directamente a esa posición del arreglo y recorre la pequeña lista enlazada correspondiente hasta encontrar una coincidencia exacta de código. Retorna el puntero directo al objeto Reserva para que la interfaz pueda consultar qué película o asiento contiene.

### bool eliminar(const string& codigo_reserva);

Localiza el índice del código. Recorre la lista enlazada de esa posición llevando un puntero al nodo anterior. Al encontrar la reserva, actualiza los enlaces para saltar el nodo eliminado, libera la memoria del nodo y decrementa el contador global. Se utiliza desde la vista del cliente cuando este cancela un boleto.

### void generarReporte();

Traduce la tabla a código Graphviz. A diferencia de los árboles, exige el atributo rankdir=LR (orientación de Izquierda a Derecha) en el archivo .dot. Dibuja primero el arreglo vertical enumerando los índices de la tabla (los buckets), y luego dibuja nodos horizontales enlazados con flechas para representar físicamente las colisiones resueltas en listas enlazadas.

# Matriz Dispersa de Asientos 

Este archivo define una Matriz Dispersa Ortogonal diseñada para gestionar el mapa de asientos de cada función en el cine. Se eligió esta estructura de datos avanzada en lugar de un arreglo bidimensional estándar (ej. Asiento sala[50][50]) para optimizar drásticamente el uso de memoria y los tiempos de procesamiento.

En una matriz dispersa, solo se instancian y almacenan en memoria los asientos que están ocupados. Si una sala de 100 asientos solo tiene 3 boletos vendidos, el sistema crea únicamente 3 nodos, asumiendo matemáticamente que las coordenadas inexistentes están libres. Para lograr esto, utiliza un sistema de listas enlazadas entrelazadas: cabeceras maestras para las filas y columnas, y nodos internos conectados simultáneamente de forma horizontal y vertical.

## Metodos Principales:

![MetodosPrincipales](imagenes/Captura%20de%20pantalla%202026-09-17%20175550.png)

### Componentes Estructurales

NodoAsiento (El Asiento Ocupado)

class NodoAsiento {
private:
    int fila;
    int columna;
    string cliente;
    NodoAsiento* derecha; // siguiente asiento ocupado en la misma fila
    NodoAsiento* abajo;   // siguiente asiento ocupado en la misma columna
    // ...
};

Representa una reserva activa en la sala. Guarda sus coordenadas exactas (fila, columna), el identificador o código de la reserva (guardado en la variable cliente), y posee dos punteros direccionales. El puntero derecha lo enlaza con el siguiente asiento ocupado en esa misma fila, y el puntero abajo lo enlaza con el siguiente asiento ocupado en esa misma columna.

### NodoCabecera (El Índice Ortogonal)

class NodoCabecera {
private:
    int indice;              // numero de fila o de columna
    NodoAsiento* primero;    // primer asiento ocupado de esa fila/columna
    NodoCabecera* siguiente; // siguiente cabecera de fila/columna
    // ...
};

Son los puntos de acceso a la cuadrícula. Si hay un asiento reservado en la fila 3, existirá un NodoCabecera con el índice 3. Este nodo guarda un puntero primero que apunta al primer NodoAsiento de su línea.

### Clase Principal: MatrizDispersa

Coordina la interconexión de punteros en dos dimensiones simultáneamente, asegurando que la cuadrícula lógica no se rompa al insertar o eliminar nodos en medio de las listas.

### NodoCabecera* obtenerOCrearCabeceraFila(int fila) y Columna(int columna)

Son el motor de escalabilidad de la estructura. Cuando un usuario reserva un asiento (ej. Fila 5, Columna 8), estos métodos buscan si ya existen las cabeceras maestras para esa fila y columna. Si no existen, instancian los nodos cabecera y los insertan de forma matemáticamente ordenada (ej. la cabecera 5 se ancla después de la 2 y antes de la 7) para mantener la integridad geométrica.

### bool reservar(int fila, int columna, const string& codigo_reserva);

Es el método de inserción ortogonal. Ejecuta los siguientes pasos lógicos:

1. Valida que las coordenadas numéricas no superen los límites totales de la sala.

2. Llama a los métodos obtenerOCrearCabecera para garantizar los puntos de entrada.

3. Instancia el nuevo NodoAsiento en memoria.

5. Recorre la fila correspondiente para enlazar el nodo horizontalmente, respetando el orden numérico de las columnas.

6. Recorre la columna correspondiente para enlazar el nodo verticalmente, respetando el orden numérico de las filas.


### bool cancelar(int fila, int columna);

Realiza la desvinculación bidimensional. Localiza el nodo exacto usando las coordenadas, lo desenlaza de su fila (conectando el asiento izquierdo directo con el asiento derecho) y lo desenlaza de su columna (conectando el asiento superior directo con el asiento inferior). Al terminar de puentear las conexiones, ejecuta un delete para liberar la memoria RAM de ese asiento.

### bool estaOcupado(int fila, int columna) y string getClienteAsiento(int fila, int columna)

Métodos de consulta de estado. Intentan localizar un nodo en la coordenada solicitada. Si al recorrer la estructura el puntero llega a nullptr sin encontrar la coordenada exacta, significa que el nodo nunca fue creado, devolviendo false (o un string vacío ""). La interfaz gráfica (Qt) consume estos métodos para decidir si pinta un botón de color rojo (ocupado) o verde (libre).


### void cargarDesdeJSON(...) y void guardarAJSON(...)

Aseguran la persistencia del mapa individual de la sala. cargarDesdeJSON reconstruye la matriz leyendo un archivo al arrancar el programa o abrir una función específica, mientras que guardarAJSON sobreescribe el archivo para reflejar de manera permanente las nuevas reservas o cancelaciones.

### bool generarReporteAsientos(const string& tituloFuncion, const string& rutaDot, const string& rutaPng)

Genera el script de visualización para Graphviz. En una matriz dispersa ortogonal, el reporte visual exige el uso de los atributos de agrupamiento estructural en sintaxis DOT: utiliza group para forzar la alineación de los nodos en columnas rectas, y rank=same para forzar la alineación en filas horizontales. Se encarga de graficar únicamente las cabeceras y los nodos que existen físicamente en la memoria.