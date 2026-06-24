// =======================
// Piano Arduino con Inicio de Mario Bros
// =======================

const int BUZZER = 13;

// BOTONES
const int BOTON_DO = 2;
const int BOTON_RE = 3;
const int BOTON_MI = 4;
const int BOTON_FA = 5;

// NOTAS MUSICALES DEL PIANO
const int DO = 262;
const int RE = 294;
const int MI = 330;
const int FA = 349;

// NOTAS EXTRA EXCLUSIVAS PARA LA MELODÍA DE MARIO BROS
const int NOTE_G3 = 196; // Sol grave
const int NOTE_C5 = 523; // Do agudo (¡Corregido!)
const int NOTE_E5 = 659; // Mi agudo
const int NOTE_G5 = 784; // Sol agudo

// MEMORIA DEL ESTADO
int notaActual = 0;


// ====================================================
// SETUP
// ====================================================
void setup() {

  Serial.begin(9600);

  pinMode(BOTON_DO, INPUT_PULLUP);
  pinMode(BOTON_RE, INPUT_PULLUP);
  pinMode(BOTON_MI, INPUT_PULLUP);
  pinMode(BOTON_FA, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  // --------------------------------------------------
  // REPRODUCIR INICIO DE MARIO BROS (Suena una vez y para)
  // --------------------------------------------------
  Serial.println("¡It's-a me, Mario! Tocando inicio...");
  tocarMelodiaInicio();
  
  Serial.println("Piano listo");
}


// ====================================================
// LOOP (Tu piano original)
// ====================================================
void loop() {

  int nuevaNota = 0;

  // DETECTAR BOTONES
  if (digitalRead(BOTON_DO) == LOW) {
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

  // ¿LA NOTA CAMBIÓ?
  if (nuevaNota != notaActual) {

    // SILENCIO
    if (nuevaNota == 0) {
      noTone(BUZZER);
      Serial.println("Silencio");
    }
    // REPRODUCIR NOTA
    else {
      tone(BUZZER, nuevaNota);

      if (nuevaNota == DO)  Serial.println("DO");
      else if (nuevaNota == RE) Serial.println("RE");
      else if (nuevaNota == MI) Serial.println("MI");
      else if (nuevaNota == FA) Serial.println("FA");
    }

    notaActual = nuevaNota;
  }

  delay(10);
}


// ====================================================
// NUEVA FUNCIÓN: INTRO DE MARIO BROS
// ====================================================
void tocarMelodiaInicio() {
  // Las 13 posiciones con las notas y silencios (0) del inicio
  int melodia[] = { 
    NOTE_E5, NOTE_E5, 0, NOTE_E5, 0, NOTE_C5, NOTE_E5, 0, NOTE_G5, 0, 0, 0, NOTE_G3 
  };
  
  // Duración en milisegundos de cada una de las posiciones
  int duraciones[] = { 
    50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 
  };

  // Recorremos los 13 elementos de la introducción
  for (int i = 0; i < 13; i++) {
    // Si encuentra un 0, apaga el sonido brevemente para simular el silencio musical
    if (melodia[i] == 0) {
      noTone(BUZZER);
    } else {
      tone(BUZZER, melodia[i], duraciones[i]);
    }
    
    // Pausa rítmica necesaria entre nota y nota
    delay(duraciones[i] * 1.30); 
  }
  
  // Apagado obligatorio de seguridad al finalizar la intro
  noTone(BUZZER);
  
  // Retraso de medio segundo para que no empiece bruscamente el piano
  delay(500); 
}
