// Fuente: ChatGPT
// Prompt: ayudame a establecer un canal de comunicacion entre el movimiento del jugador y el joystick arduino UNO
// La IA sugiere utilizar las librerias de conexion avr y medir los rangos de valores que proporciona el canal serial por el cual el joystick envia los datos
#include <Arduino.h>

const int pinX = A0;
const int pinBoton = 2;

int lastX = 512;
const int noiseThreshold = 5;

void setup() {
  Serial.begin(115200);
  pinMode(pinBoton, INPUT_PULLUP);
}

void loop() {
  int x = analogRead(pinX);
  int boton = digitalRead(pinBoton);
  
  if (abs(x - lastX) > noiseThreshold) {
    Serial.print(x);
    Serial.print(",0,");
    Serial.println(boton);
    lastX = x;
  } else {
    Serial.print("512,0,");
    Serial.println(boton);
  }
  
  delay(50);
}