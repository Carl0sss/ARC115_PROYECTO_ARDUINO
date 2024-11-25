#include <Wire.h>

// Pines de los LEDs del maestro
const int ledsMaestro[12] = {
  13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2
};

char mensaje[4]; // Arreglo para almacenar los 4 caracteres

void setup() {
  Wire.begin(); // Configura como maestro
  Serial.begin(9600); // Monitor serial para depuración
  
  // Configurar los pines de los LEDs del maestro como salida
  for (int i = 0; i < 12; i++) {
    pinMode(ledsMaestro[i], OUTPUT);
    digitalWrite(ledsMaestro[i], LOW); // Inicialmente apagados
  }
  
  Serial.println("Por favor, ingrese 4 caracteres (A-Z): ");
}

void loop() {
  // Leer 4 caracteres desde el teclado
  if (Serial.available() >= 4) {
    for (int i = 0; i < 4; i++) {
      mensaje[i] = Serial.read(); // Leer cada carácter ingresado por el usuario
      mensaje[i] = toupper(mensaje[i]); // Asegurarse de que sea mayúscula
    }
    
    // Mostrar los caracteres leídos
    Serial.print("Caracteres ingresados: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(mensaje[i]);
      Serial.print(" ");
    }
    Serial.println();
    
    // Enviar el mensaje al esclavo
    Wire.beginTransmission(1); // Dirección del esclavo
    for (int i = 0; i < 4; i++) {
      Wire.write(mensaje[i]); // Enviar cada letra
    }
    Wire.endTransmission(); // Termina la transmisión
    
    // Solicitar datos de vuelta del esclavo
    Wire.requestFrom(1, 12); // Solicitar 12 bytes (1 por LED) del esclavo
    recibirDatosDelEsclavo(); // Procesar los datos recibidos

    Serial.println("Mensaje procesado."); // Depuración en el monitor serial
  }

  delay(2000); // Pausa de 2 segundos antes de aceptar otro conjunto de caracteres
}

void recibirDatosDelEsclavo() {
  int i = 0; // Índice de LEDs del maestro
  
  while (Wire.available() && i < 12) {
    int estado = Wire.read();   // Leer el estado de cada LED
    digitalWrite(ledsMaestro[i], estado); // Controlar los LEDs del maestro
    i++;
  }
}
