// =======================================================
// Piano Arduino con Inicio de "Para Elisa" (Beethoven)
// + Salida Serial estructurada para persistencia (NOTA:XXX)
// =======================================================

const int BUZZER = 13;

// -----------------------
// BOTONES
// -----------------------
const int BOTON_DO = 2;
const int BOTON_RE = 3;
const int BOTON_MI = 4;
const int BOTON_FA = 5;

// -----------------------
// NOTAS MUSICALES DEL PIANO
// -----------------------
const int DO = 262;
const int RE = 294;
const int MI = 330;
const int FA = 349;

// -----------------------
// NOTAS EXTRA PARA "PARA ELISA"
// -----------------------
const int NOTE_E5  = 659;
const int NOTE_DS5 = 622;
const int NOTE_B4  = 494;
const int NOTE_D5  = 587;
const int NOTE_C5  = 523;
const int NOTE_A4  = 440;

int notaActual = 0;

void setup() {
  Serial.begin(9600);
  configurarPines();
  Serial.println("Tocando bienvenida: Para Elisa (Beethoven)...");
  tocarMelodiaInicio();
  Serial.println("Piano listo");
}

void loop() {
  int nuevaNota = leerBotones();
  actualizarSonido(nuevaNota);
  delay(10);
}

void configurarPines() {
  pinMode(BOTON_DO, INPUT_PULLUP);
  pinMode(BOTON_RE, INPUT_PULLUP);
  pinMode(BOTON_MI, INPUT_PULLUP);
  pinMode(BOTON_FA, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}

int leerBotones() {
  if (digitalRead(BOTON_DO) == LOW) return DO;
  else if (digitalRead(BOTON_RE) == LOW) return RE;
  else if (digitalRead(BOTON_MI) == LOW) return MI;
  else if (digitalRead(BOTON_FA) == LOW) return FA;
  return 0;
}

void actualizarSonido(int nuevaNota) {
  if (nuevaNota != notaActual) {
    if (nuevaNota == 0) {
      noTone(BUZZER);
      // IMPORTANTE: este prefijo "NOTA:" es lo que el script Python
      // va a leer para saber que se trata de un evento de nota.
      Serial.println("NOTA:SILENCIO");
    } else {
      tone(BUZZER, nuevaNota);
      imprimirNombreNota(nuevaNota);
    }
    notaActual = nuevaNota;
  }
}

// ====================================================
// FUNCIÓN: Mostrar el nombre de la nota por Serial
// Ahora imprime en formato "NOTA:DO" para que el
// puente Python (serial_bridge.py) lo pueda parsear
// fácilmente con split(":").
// ====================================================
void imprimirNombreNota(int nota) {
  if (nota == DO)       Serial.println("NOTA:DO");
  else if (nota == RE)  Serial.println("NOTA:RE");
  else if (nota == MI)  Serial.println("NOTA:MI");
  else if (nota == FA)  Serial.println("NOTA:FA");
}

void tocarMelodiaInicio() {
  int melodia[] = {
    NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5,
    NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4
  };
  int duraciones[] = {
    200, 200, 200, 200, 200,
    200, 200, 200, 400
  };
  int cantidadNotas = sizeof(melodia) / sizeof(melodia[0]);

  for (int i = 0; i < cantidadNotas; i++) {
    if (melodia[i] == 0) {
      noTone(BUZZER);
    } else {
      tone(BUZZER, melodia[i], duraciones[i]);
    }
    delay(duraciones[i] * 1.30);
  }
  noTone(BUZZER);
  delay(500);
}
