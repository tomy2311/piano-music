// =======================================================
// Piano Arduino con Inicio de "Para Elisa" (Beethoven)
// =======================================================

// "const" significa CONSTANTE: un valor que no cambia mientras el programa corre.
// "int" significa entero (sin decimales).
// BUZZER guarda el número de pin donde está conectado el buzzer (pin digital 13).
const int BUZZER = 13;


// -----------------------
// BOTONES
// -----------------------

// Le damos un nombre descriptivo a cada pin de botón,
// en vez de usar directamente los números 2, 3, 4 y 5.
// Esto hace el código más legible y fácil de modificar.

const int BOTON_DO = 2; // Botón conectado al pin digital 2
const int BOTON_RE = 3; // Botón conectado al pin digital 3
const int BOTON_MI = 4; // Botón conectado al pin digital 4
const int BOTON_FA = 5; // Botón conectado al pin digital 5


// -----------------------
// NOTAS MUSICALES DEL PIANO
// -----------------------

// tone() necesita una frecuencia en Hertz (Hz).
// 1 Hz = 1 vibración por segundo.
// Estas frecuencias corresponden aproximadamente a las notas DO-RE-MI-FA.

const int DO = 262; // Frecuencia de la nota DO
const int RE = 294; // Frecuencia de la nota RE
const int MI = 330; // Frecuencia de la nota MI
const int FA = 349; // Frecuencia de la nota FA


// -----------------------
// NOTAS EXTRA PARA "PARA ELISA"
// -----------------------

// Estas notas NO tienen botón asignado.
// Se usan únicamente para reproducir automáticamente
// la melodía de bienvenida al encender el Arduino.

const int NOTE_E5  = 659; // Mi agudo
const int NOTE_DS5 = 622; // Re sostenido / Mi bemol agudo
const int NOTE_B4  = 494; // Si
const int NOTE_D5  = 587; // Re agudo
const int NOTE_C5  = 523; // Do agudo
const int NOTE_A4  = 440; // La


// -----------------------
// MEMORIA DEL ESTADO
// -----------------------

// A diferencia de "const", esta variable SÍ puede cambiar.
// Guarda cuál fue la última nota que sonó.
// Empieza en 0, que significa "ninguna nota sonando".

int notaActual = 0;


// ====================================================
// SETUP
// ====================================================

// setup() se ejecuta UNA SOLA VEZ al encender el Arduino,
// al presionar RESET, o al subir un programa nuevo.

void setup() {

  // Inicia la comunicación serial a 9600 bits por segundo.
  // Permite enviar mensajes desde Arduino a la computadora.
  Serial.begin(9600);

  // Llama a la función que configura los pines de entrada y salida.
  configurarPines();

  // Avisa por el Monitor Serie que va a sonar la melodía de bienvenida.
  Serial.println("Tocando bienvenida: Para Elisa (Beethoven)...");

  // Llama a la función que reproduce la melodía de bienvenida.
  tocarMelodiaInicio();

  // Avisa por el Monitor Serie que el piano ya está listo para usarse.
  Serial.println("Piano listo");
}


// ====================================================
// LOOP
// ====================================================

// loop() se ejecuta una y otra vez, sin parar,
// mientras el Arduino esté encendido.
// Solo contiene llamadas a funciones: ninguna lógica suelta acá.

void loop() {

  // Llama a la función que lee los 4 botones
  // y guarda en "nuevaNota" la nota detectada (o 0 si no hay ninguna).
  int nuevaNota = leerBotones();

  // Llama a la función que decide si hay que sonar,
  // callar, o no hacer nada, según la nota detectada.
  actualizarSonido(nuevaNota);

  // Pausa de 10 milisegundos.
  // Reduce rebotes mecánicos de los botones
  // y evita lecturas demasiado rápidas.
  delay(10);
}


// ====================================================
// FUNCIÓN: Configurar pines de entrada y salida
// ====================================================

void configurarPines() {

  // INPUT_PULLUP activa una resistencia interna del Arduino.
  // Sin tocar el botón: el pin lee HIGH.
  // Tocando el botón: el pin lee LOW.
  // Esto evita que el pin quede "flotando" (lecturas erráticas).

  pinMode(BOTON_DO, INPUT_PULLUP); // Configura el pin del botón DO como entrada con pull-up
  pinMode(BOTON_RE, INPUT_PULLUP); // Configura el pin del botón RE como entrada con pull-up
  pinMode(BOTON_MI, INPUT_PULLUP); // Configura el pin del botón MI como entrada con pull-up
  pinMode(BOTON_FA, INPUT_PULLUP); // Configura el pin del botón FA como entrada con pull-up

  // Configura el pin del buzzer como salida,
  // para que el Arduino pueda enviarle señal.
  pinMode(BUZZER, OUTPUT);
}


// ====================================================
// FUNCIÓN: Leer botones y devolver la nota detectada
// ====================================================

// Esta función revisa los 4 botones uno por uno.
// Devuelve la frecuencia de la nota correspondiente,
// o 0 si ningún botón está presionado.

int leerBotones() {

  // digitalRead lee el estado de un pin: HIGH (1) o LOW (0).
  // Como usamos INPUT_PULLUP, presionado = LOW.

  if (digitalRead(BOTON_DO) == LOW) {
    return DO; // Si el botón DO está presionado, devuelve la frecuencia de DO
  }
  else if (digitalRead(BOTON_RE) == LOW) {
    return RE; // Si el botón RE está presionado, devuelve la frecuencia de RE
  }
  else if (digitalRead(BOTON_MI) == LOW) {
    return MI; // Si el botón MI está presionado, devuelve la frecuencia de MI
  }
  else if (digitalRead(BOTON_FA) == LOW) {
    return FA; // Si el botón FA está presionado, devuelve la frecuencia de FA
  }

  // Si llegamos hasta acá, ningún botón está presionado.
  return 0;
}


// ====================================================
// FUNCIÓN: Comparar nota nueva con la actual y actuar
// ====================================================

// Esta función decide qué hacer según si la nota cambió o no,
// evitando reiniciar el sonido miles de veces por segundo.

void actualizarSonido(int nuevaNota) {

  // != significa "distinto de".
  // Pregunta: ¿la nota detectada ahora es diferente de la que sonaba antes?

  if (nuevaNota != notaActual) {

    // Si nuevaNota vale 0, significa que no hay ningún botón presionado.
    if (nuevaNota == 0) {
      noTone(BUZZER); // Detiene cualquier sonido del buzzer
      Serial.println("Silencio"); // Avisa por el Monitor Serie
    }
    else {
      // tone(pin, frecuencia) genera una onda cuadrada de esa frecuencia.
      tone(BUZZER, nuevaNota);

      // Llama a la función que muestra el nombre de la nota por Serial.
      imprimirNombreNota(nuevaNota);
    }

    // Actualiza la memoria: la nota actual pasa a ser la que acabamos de procesar.
    notaActual = nuevaNota;
  }
}


// ====================================================
// FUNCIÓN: Mostrar el nombre de la nota por Serial
// ====================================================

// Esta función solo sirve para mostrar en el Monitor Serie
// qué nota está sonando. No afecta el sonido en sí.

void imprimirNombreNota(int nota) {

  if (nota == DO)       Serial.println("DO"); // Si la nota es DO, lo imprime
  else if (nota == RE)  Serial.println("RE"); // Si la nota es RE, lo imprime
  else if (nota == MI)  Serial.println("MI"); // Si la nota es MI, lo imprime
  else if (nota == FA)  Serial.println("FA"); // Si la nota es FA, lo imprime
}


// ====================================================
// FUNCIÓN: Melodía de bienvenida "Para Elisa" (Beethoven)
// Fragmento inicial, dominio público
// ====================================================

// Esta función reproduce automáticamente, una sola vez,
// la frase de apertura de "Para Elisa" al encender el Arduino.
// No depende de ningún botón.

void tocarMelodiaInicio() {

  // Array con la secuencia de notas de la frase de apertura.
  int melodia[] = {
    NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5,
    NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4
  };

  // Array con la duración (en milisegundos) de cada nota,
  // en el mismo orden que el array "melodia".
  int duraciones[] = {
    200, 200, 200, 200, 200,
    200, 200, 200, 400
  };

  // Calcula automáticamente cuántas notas hay en el array,
  // dividiendo el tamaño total entre el tamaño de un elemento.
  // Así evitamos contar a mano y equivocarnos.
  int cantidadNotas = sizeof(melodia) / sizeof(melodia[0]);

  // Recorre cada posición del array, de la primera a la última.
  for (int i = 0; i < cantidadNotas; i++) {

    // Si la posición actual es 0, representa un silencio.
    if (melodia[i] == 0) {
      noTone(BUZZER); // Apaga el sonido para simular el silencio
    } else {
      // tone(pin, frecuencia, duración) suena esa nota
      // durante la cantidad de milisegundos indicada.
      tone(BUZZER, melodia[i], duraciones[i]);
    }

    // Pausa el programa un poco más que la duración de la nota (1.30x),
    // para que se note una pequeña separación entre nota y nota.
    delay(duraciones[i] * 1.30);
  }

  // Apaga el buzzer por seguridad al terminar la melodía,
  // por si quedó algún sonido residual.
  noTone(BUZZER);

  // Pausa de medio segundo antes de que arranque el piano,
  // para que la transición no sea brusca.
  delay(500);
}
