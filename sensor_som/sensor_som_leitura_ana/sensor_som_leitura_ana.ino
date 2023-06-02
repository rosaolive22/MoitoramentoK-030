//OBJETIVO DO PROJETO:
// - Primeiramente, queremos acionar o sensor de presença. //
// - Acionando o sensor, vamos ligar um led. //
// - Se o led estiver ligado, teremos um alerta em uma página da web.
// - Após isso terá um botão na página, para acionar ou não um alarme(led vermelho).

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const int KY037_PIN = A0;   // Pino analógico do KY-037
const int LED_PIN = D1;     // Pino GPIO do ESP8266 conectado ao LED
const int SOUND_THRESHOLD = 50;  // Limiar de sensibilidade do som

int verificar = 0;

const char* ssid = "BDAG";
const char* password = "bdag2018";

WiFiServer server(80);

void handleRoot() {
 
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");


  server.begin();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  while (!client.available()) {
    delay(1);
  }

  String requisicao = client.readStringUntil('\r');
  client.flush();
  int sensorValue = analogRead(KY037_PIN);

  if (sensorValue >= SOUND_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);  // Ligar o LED
    verificar = 1;
  } else {
    digitalWrite(LED_PIN, LOW);   // Desligar o LED
  }

  if (requisicao.indexOf("ok") != -1) {
    verificar = 0;
  }

 String html = "";
  html += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  html += "<DOCTYPE html>\r\n<html><head><meta charset=utf-8>";
  html += "<meta http-equiv='refresh' content='5'></head><body>";
  html += "<h1> ESP8266 Server </h1>";
  html += "<p> Este é o servidor de NodeMCU do Big Data no Agronegócio </p>";
  html += "<p> Monitoramento por som com status na web, em tempo real + acionamento manual de alarme </p>";
  
  if(verificar == 1){
    html += "<p>LED: <a href=\"ledon\"><button>ALERTA</button></a><p>";
    html += "<p>LED: <a href=\"ok\"><button>VERIFICADO</button></a><p>";
  }else{
    html += "<p>LED: <a href=\"ledon\"><button>ESTÁ TRANQUILO</button></a><p>";
  }

  html += "</body></html>";

client.print(html);
  delay(100);
}
