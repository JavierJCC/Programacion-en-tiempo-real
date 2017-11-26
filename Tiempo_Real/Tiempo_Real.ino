#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // Agregamos a FreeRTOS funciones de semaforo o banderas

// Configuramos los pines del sensor Trigger y Echo
  const int PinTrig = 7;
  const int PinEcho = 6;

// Declaramos un mutex Semafore el cual será una administrador para el puerto serial 
// Esto para aegurar que solo una tarea tendra acceso a este recurso en cualquierparte del tiempo 

SemaphoreHandle_t xSerialSemaphore;

// Definimos tres Tareas 
void TaskOne( void *pvParameters );
void TaskTwo( void *pvParameters );
void iniciarTrigger();

// La función de configuración corre una vez when you press reset or power the board
void setup() {    
  // inicializamos la comunicacion serial en 9600 bits por segundo:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // espera por al puerto serial para connectar.
  }

  // Semaforos sun usualmente usado para parar una tarea en proceso, donde estas pueden ser pausadas para esperar,
  // porque esta compartiendo un mismo recurso, como por ejemplo el puerto serial.
  // Semaforos solo pueden ser usados mientras el planificador esta corriendo, pero nosotros podemos colocarlo aquí.
  if ( xSerialSemaphore == NULL )  // Chequeo para confirmar que el semaforo serial no ha sido creado ya.
  {
    xSerialSemaphore = xSemaphoreCreateMutex();  // Creamos un mutex semaphore que usaremos para administrar el puerto serial.
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // Hacemos el puerto serial disponible para usar dando el semaforo.
  }

  // Ahora ponemos las dos tareas corriendo independientemente 
  xTaskCreate(
    TaskOne
    ,  (const portCHAR *)"TareaUno"  // Un nombre solo para humanos
    ,  256  // Este tamaño de la pila puede ser comprobado y ajustado, leyendo la pila HIGHWATER
    ,  NULL
    ,  0  // Prioridad, con 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskTwo
    ,  (const portCHAR *) "TareaDos"
    ,  256  // Tamaño de la pila 
    ,  NULL
    ,  1  //Prioridad 
    ,  NULL );
  // Ahora el planificador de tareas, el cual toma el control de planificador individual de tareas, se inicia automaticamente.
}

void loop()
{
  // Vacio. Ya que las cosas se realizaran en las tareas.
}

/*-------------------------Tarea Numero 1-------------------------*/
void TaskOne( void *pvParameters __attribute__((unused)) )
{
    // Constante velocidad sonido en cm/s
    const float VelSon = 34000.0; 
 
    // Número de muestras
    const int numLecturas = 10;
 
    // Distancia a los 100 ml y vacío
    const float distancia100 = 2.3;
    const float distanciaVacio = 11.5;
     
    float lecturas[numLecturas]; // Array para almacenar lecturas
    int lecturaActual = 0; // Lectura por la que vamos
    float total = 0; // Total de las que llevamos
    float media = 0; // Media de las medida6s
    bool primeraMedia = false; // Para saber que ya hemos calculado por lo menos una
 
  // Ponemos el pin Trig en modo salida
  pinMode(PinTrig, OUTPUT);
  // Ponemos el pin Echo en modo entrada
  pinMode(PinEcho, INPUT);
  // Inicializamos el array
  for (int i = 0; i < numLecturas; i++)
  {
    lecturas[i] = 0;
  }
  for (;;) // Una tarea nunca retornara o saldra.
  { 
          // Eliminamos la última medida
          total = total - lecturas[lecturaActual];
          iniciarTrigger();
          // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
          unsigned long tiempo = pulseIn(PinEcho, HIGH);
         
          // Obtenemos la distancia en cm, hay que convertir el tiempo en segudos ya que está en microsegundos
          // por eso se multiplica por 0.000001
          float distancia = tiempo * 0.000001 * VelSon / 2.0;
         
          // Almacenamos la distancia en el array
          lecturas[lecturaActual] = distancia;
         
          // Añadimos la lectura al total
          total = total + lecturas[lecturaActual];
         
          // Avanzamos a la siguiente posición del array
          lecturaActual = lecturaActual + 1;
         
          // Comprobamos si hemos llegado al final del array
          if (lecturaActual >= numLecturas)
          {
            primeraMedia = true;
            lecturaActual = 0;
          }
         
          // Calculamos la media
          media = total / numLecturas;
                 if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 10 ) == pdTRUE )
                 {
                     if(primeraMedia){
                     float distanciaLleno = distanciaVacio - media;
                     float cantidadLiquido = distanciaLleno * 100 / distancia100;
                         Serial.println(cantidadLiquido);
                         //Serial.println("ml en Tarea 1");
                     }
                 xSemaphoreGive( xSerialSemaphore ); // Ahora liberamos o damos el serial port a otras.
                 }  
  vTaskDelay(34);
  } //fin del for
} //Fin de taskOne

/*-------------------------Tarea Numero 2 -------------------------*/
void TaskTwo( void *pvParameters __attribute__((unused)) ) 
{   
    // Constante velocidad sonido en cm/s
    const float VelSon = 34000.0;
    
    // Número de muestras
    const int numLecturasDos = 10;
 
    // Distancia a los 100 ml y vacío
    const float distancia100dos = 2.3;
    const float distanciaVaciodos = 11.5;
     
    float lecturas[numLecturasDos]; // Array para almacenar lecturas
    int lecturaActual = 0; // Lectura por la que vamos
    float totaldos = 0; // Totaldos de las que llevamos
    float mediados = 0; // Mediados de las medidas
    bool segundaMediados = false; // Para saber que ya hemos calculado por lo menos una
 
  // Ponemos el pin Trig en modo salida
  pinMode(PinTrig, OUTPUT);
  // Ponemos el pin Echo en modo entrada
  pinMode(PinEcho, INPUT);
  // Inicializamos el array
  for (int i = 0; i < numLecturasDos; i++)
  {
    lecturas[i] = 0;
  }
  for (;;)
  {
          // Eliminamos la última medida
          totaldos = totaldos - lecturas[lecturaActual];
          iniciarTrigger();
          // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
          unsigned long tiempo = pulseIn(PinEcho, HIGH);
         
          // Obtenemos la distancia en cm, hay que convertir el tiempo en segudos ya que está en microsegundos
          // por eso se multiplica por 0.000001
          float distancia = tiempo * 0.000001 * VelSon / 2.0;
         
          // Almacenamos la distancia en el array
          lecturas[lecturaActual] = distancia;
         
          // Añadimos la lectura al totaldos
          totaldos = totaldos + lecturas[lecturaActual];
         
          // Avanzamos a la siguiente posición del array
          lecturaActual = lecturaActual + 1;
         
          // Comprobamos si hemos llegado al final del array
          if (lecturaActual >= numLecturasDos)
          {
            segundaMediados = true;
            lecturaActual = 0;
          }
         
          // Calculamos la mediados
          mediados = totaldos / numLecturasDos;
    // Ver si nosotros podemos obtener o tomar el semaforo serial
    // Si el semaforo no esta disponible, espera 5 tictacs de el planificador para ver si este esta libre.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 10 ) == pdTRUE )
    {
          if(segundaMediados){
             float distanciaL = distanciaVaciodos - mediados;
             float cantidadL = distanciaL * 100 / distancia100dos;
             Serial.println(cantidadL);
             //Serial.println("ml en tarea 2");         
            }
      xSemaphoreGive( xSerialSemaphore ); // Ahora liberamos o damos el serial port a otras.
    }
    vTaskDelay(60);
  }
}

// Método que inicia la secuencia del Trigger para comenzar a medir
void iniciarTrigger()
{
  // Ponemos el Triiger en estado bajo y esperamos 2 ms
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2);
 
  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);
 
  // Comenzamos poniendo el pin Trigger en estado bajo
  digitalWrite(PinTrig, LOW);
}
