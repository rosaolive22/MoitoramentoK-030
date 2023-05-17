#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>

#define SSID "BDAG"
#define SENHA "bdag2018"

#define LED 2

boolean statusLed = 14;

WiFiServer server(80);  

const int AO = A0;
int estadoAO = 0; 

void setup() {
   Serial.begin(115200);
  WiFi.begin(SSID, SENHA);

  pinMode(AO, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
}
	server.begin();
  	Serial.println("Wifi conectado e servidor inicializado");
  	Serial.print("Endereço IP da placa: ");
  	Serial.println(WiFi.localIP());
}


void loop() {
//Aciona caso capitar som
estadoAO = analogRead(AO);

  if (analogRead(AO) > 50) {
    digitalWrite(12, !digitalRead(12));
    delay(100);
  }

//Página na web com Tema do projeto
String html = "";
  html += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  html += "<DOCTYPE html>\r\n<html><head><meta charset=utf-8>";
  html += "<meta http-equiv='refresh' content='5'></head><body>";
  html += "<h1> ESP8266 Server </h1>";
  html += "<p> Este é o servidor de NodeMCU do Big Data no Agronegócio </p>";
  html += "<p> Monitoramento por som com status na web, em tempo real + acionamento manual de alarme </p>";

//Botão para acionar o alarme
  if (!statusLed) {
    html += "<p>LED INTERNO: <a href=\"ledon\"><button>DESLIGAR ALARME</button></a><p>";
  } else {
    html += "<p>LED INTERNO: <a href=\"ledoff\"><button>ACIONAR ALARME</button></a></p>";
  }

//O valor do sensor de som (estadoAO) será exibido em tempo real, na web
html += "<p><strong>Estado do AO: </strong><span id=\"estadoAO\"></span></p>";
html += "<script>";
html += "setInterval(function() {";
html += "  document.getElementById('estadoAO').textContent = " + String(estadoAO) + ";";
html += "}, 1000);";
html += "</script>";

html += "</body></html>";


  client.print(html);
  delay(100);
}