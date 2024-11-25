#include <Wire.h>

// Pines de los LEDs (de acuerdo a la nueva disposición)
const int leds[24] = {
  13, 12, 11, 10, 9, 8,    // LEDs para la primera letra
  7, 6, 5, 4, 3, 2,        // LEDs para la segunda letra
  13, 12, 11, 10, 9, 8,    // LEDs para la tercera letra
  7, 6, 5, 4, 3, 2,        // LEDs para la cuarta letra
};

// Mapa de Braille para las letras A-Z
const byte braille[26][6] = {
  {1, 0, 0, 0, 0, 0}, // A
  {1, 1, 0, 0, 0, 0}, // B
  {1, 0, 0, 1, 0, 0}, // C
  {1, 0, 0, 1, 1, 0}, // D
  {1, 0, 0, 0, 1, 0}, // E
  {1, 1, 0, 1, 0, 0}, // F
  {1, 1, 0, 1, 1, 0}, // G
  {1, 1, 0, 0, 1, 0}, // H
  {0, 1, 0, 1, 0, 0}, // I
  {0, 1, 0, 1, 1, 0}, // J
  {1, 0, 1, 0, 0, 0}, // K
  {1, 1, 1, 0, 0, 0}, // L
  {1, 0, 1, 1, 0, 0}, // M
  {1, 0, 1, 1, 1, 0}, // N
  {1, 0, 1, 0, 1, 0}, // O
  {1, 1, 1, 1, 0, 0}, // P
  {1, 1, 1, 1, 1, 0}, // Q
  {1, 1, 1, 0, 1, 0}, // R
  {0, 1, 1, 1, 0, 0}, // S
  {0, 1, 1, 1, 1, 0}, // T
  {1, 0, 1, 0, 0, 1}, // U
  {1, 1, 1, 0, 0, 1}, // V
  {0, 1, 0, 1, 1, 1}, // W
  {1, 0, 1, 1, 0, 1}, // X
  {1, 0, 1, 1, 1, 1}, // Y
  {1, 0, 1, 0, 1, 1}  // Z
};

// Variables globales para el estado actual
char recibido[4] = ""; // Almacena las letras recibidas

void setup() {
  Wire.begin(1); // Configura como esclavo con dirección 1
  Wire.onReceive(recibirMensaje); // Configura la función de recepción
  Wire.onRequest(enviarDatosAlMaestro); // Configura la función de solicitud
  
  // Configura los pines de LEDs como salida
  for (int i = 0; i < 24; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW); // Apaga todos los LEDs inicialmente
  }
}

void loop() {
  // No se necesita código en el loop para el esclavo
}

// Función para recibir el mensaje
void recibirMensaje(int bytes) {
  for (int i = 0; i < 4 && Wire.available(); i++) {
    recibido[i] = Wire.read(); // Lee cada letra
  }
  
  // Activar LEDs para las dos primeras letras recibidas
  for (int i = 0; i < 2; i++) {  // Solo iluminamos los primeros 2 caracteres (12 LEDs)
    if (recibido[i] >= 'A' && recibido[i] <= 'Z') {
      encenderBraille(recibido[i] - 'A', i); // Enciende LEDs locales para las dos primeras letras
    }
  }
}

// Función para encender LEDs según el Braille
void encenderBraille(int letra, int posicion) {
  int offset = posicion * 6; // Desplazamiento para cada letra
  
  // Enciende los LEDs según el mapa Braille
  for (int i = 0; i < 6; i++) {
    digitalWrite(leds[offset + i], braille[letra][i]);
  }
}

// Función para enviar datos al maestro
void enviarDatosAlMaestro() {
  // Enviar los LEDs de los últimos 2 caracteres al maestro (12 LEDs restantes)
  for (int i = 2; i < 4; i++) {  // Solo los 2 últimos caracteres
    if (recibido[i] >= 'A' && recibido[i] <= 'Z') {
      for (int j = 0; j < 6; j++) {
        Wire.write(braille[recibido[i] - 'A'][j]); // Enviar el estado de cada LED de las dos últimas letras
      }
    }
  }
}
