// =======================
// Piano Arduino Mejorado
// =======================

// La palabra "const" significa CONSTANTE.
// Una constante es un valor que NO puede cambiar durante la ejecución del programa.

// "int" significa integer (entero), es decir, un número sin decimales.

// Creamos una constante llamada BUZZER.
// Guarda el número del pin donde está conectado el buzzer.
// En este caso, el pin digital 13.
const int BUZZER = 13;


// -----------------------
// BOTONES
// -----------------------

// En vez de escribir directamente 2, 3, 4 y 5 por todo el programa,
// les damos nombres descriptivos.
//
// Ventajas:
// - El código es más fácil de leer.
// - Si cambias el botón de pin, solo cambias este valor.
// - Evitas errores.

const int BOTON_DO = 2;
const int BOTON_RE = 3;
const int BOTON_MI = 4;
const int BOTON_FA = 5;


// -----------------------
// NOTAS MUSICALES
// -----------------------

// tone() necesita una frecuencia en Hertz (Hz).
//
// Un Hertz significa:
// "cuántas veces vibra algo por segundo".
//
// Ejemplo:
//
// 262 Hz = 262 vibraciones por segundo.
//
// Estas frecuencias corresponden aproximadamente a:
//
// DO → 262 Hz
// RE → 294 Hz
// MI → 330 Hz
// FA → 349 Hz

const int DO = 262;
const int RE = 294;
const int MI = 330;
const int FA = 349;


// -----------------------
// MEMORIA DEL ESTADO
// -----------------------

// Esta variable guarda cuál fue la última nota que estaba sonando.
//
// A diferencia de "const", esta SÍ puede cambiar.
//
// Empieza valiendo 0.
//
// ¿Qué significa 0?
//
// En nuestro programa:
// 0 = ninguna nota está sonando.

int notaActual = 0;


// ====================================================
// SETUP
// ====================================================

// setup() es una función especial.
//
// Arduino la ejecuta UNA SOLA VEZ,
// justo cuando:
// - enciendes la placa,
// - presionas RESET,
// - subes un programa nuevo.

void setup() {

  // Inicia la comunicación serial.
  //
  // Permite enviar mensajes desde Arduino
  // hacia la computadora.
  //
  // 9600 significa 9600 bits por segundo.
  //
  // Es una velocidad de comunicación.

  Serial.begin(9600);


  // pinMode configura el comportamiento de un pin.

  // INPUT_PULLUP significa:
  //
  // "Arduino activará una resistencia interna."
  //
  // Sin tocar el botón:
  // HIGH
  //
  // Tocando el botón:
  // LOW
  //
  // Esto evita que el pin quede "flotando".

  pinMode(BOTON_DO, INPUT_PULLUP);
  pinMode(BOTON_RE, INPUT_PULLUP);
  pinMode(BOTON_MI, INPUT_PULLUP);
  pinMode(BOTON_FA, INPUT_PULLUP);


  // Este pin enviará señal hacia el buzzer.

  pinMode(BUZZER, OUTPUT);


  // Envía un mensaje al Monitor Serie.
  //
  // println significa:
  // print line
  //
  // Escribe el texto y luego baja de línea.

  Serial.println("Piano listo");
}


// ====================================================
// LOOP
// ====================================================

// loop() es otra función especial.
//
// Arduino la ejecuta:
// una y otra y otra y otra vez.
//
// Cuando termina,
// vuelve automáticamente al principio.
//
// Esto ocurre cientos o miles de veces por segundo.

void loop() {


  // Creamos una variable local.
  //
  // Solo existe dentro de loop().
  //
  // Empieza valiendo 0.
  //
  // 0 significa:
  // "todavía no encontré ninguna nota."

  int nuevaNota = 0;


  // ======================================
  // DETECTAR BOTONES
  // ======================================

  // digitalRead lee un pin digital.
  //
  // Solo puede devolver:
  //
  // HIGH → 1
  // LOW  → 0

  // Como usamos INPUT_PULLUP:
  //
  // SIN tocar:
  // HIGH
  //
  // TOCANDO:
  // LOW

  if (digitalRead(BOTON_DO) == LOW) {

    // Si el botón DO está presionado,
    // guardamos la frecuencia de DO.

    nuevaNota = DO;
  }

  else if (digitalRead(BOTON_RE) == LOW) {

    nuevaNota = RE;
  }

  else if (digitalRead(BOTON_MI) == LOW) {

    nuevaNota = MI;
  }

  else if (digitalRead(BOTON_FA) == LOW) {

    nuevaNota = FA;
  }


  // ======================================
  // ¿LA NOTA CAMBIÓ?
  // ======================================

  // != significa:
  //
  // "distinto de"

  // Preguntamos:
  //
  // ¿La nota detectada ahora
  // es diferente de la que estaba sonando?

  if (nuevaNota != notaActual) {


    // ==================================
    // SILENCIO
    // ==================================

    // Si nuevaNota sigue valiendo 0,
    // significa que ningún botón está presionado.

    if (nuevaNota == 0) {

      // Detiene cualquier sonido.

      noTone(BUZZER);

      // Avisa por Monitor Serie.

      Serial.println("Silencio");
    }


    // ==================================
    // REPRODUCIR NOTA
    // ==================================

    else {

      // tone(pin, frecuencia)
      //
      // Le dice al Arduino:
      //
      // "Genera una onda cuadrada
      // de esta frecuencia."

      tone(BUZZER, nuevaNota);


      // Estas líneas sirven únicamente
      // para mostrar el nombre de la nota.

      if (nuevaNota == DO)
        Serial.println("DO");

      else if (nuevaNota == RE)
        Serial.println("RE");

      else if (nuevaNota == MI)
        Serial.println("MI");

      else if (nuevaNota == FA)
        Serial.println("FA");
    }


    // Actualizamos la memoria.
    //
    // La nota actual pasa a ser
    // la que acabamos de reproducir.
    //
    // Esto evita reiniciar el sonido
    // miles de veces por segundo.

    notaActual = nuevaNota;
  }


  // ======================================
  // DELAY
  // ======================================

  // Pausa el programa durante 10 milisegundos.
  //
  // 1000 milisegundos = 1 segundo.
  //
  // Entonces:
  //
  // 10 milisegundos = 0,01 segundos.
  //
  // Sirve para reducir rebotes mecánicos
  // de los botones y evitar lecturas demasiado rápidas.

  delay(10);
}