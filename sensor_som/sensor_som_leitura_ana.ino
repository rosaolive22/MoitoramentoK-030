
// Sensor de som leitura analógica
//tudo certo! 16/05/2022

#include <SPI.h>
#include <Wire.h>

const int AO = A0;
int estadoAO = 0; 

void setup() {
  pinMode(AO, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
}

void loop() {
  estadoAO = analogRead(AO);

  if (analogRead(AO) > 50) {
    digitalWrite(12, !digitalRead(12));
    delay(100);
  }
}
