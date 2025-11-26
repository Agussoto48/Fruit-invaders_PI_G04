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
    Serial.print(",0,"); // Y no usado
    Serial.println(boton);
    lastX = x;
  } else {
    Serial.print("512,0,");
    Serial.println(boton);
  }
  
  delay(50);
}