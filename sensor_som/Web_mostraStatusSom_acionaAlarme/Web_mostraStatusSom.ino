#include <ESP8266WiFi.h>

const char* ssid = "BDAG";
const char* password = "bdag2018";

// Pino do sensor KY-037
const int sensorPin = A0;

// Valor padrão do som
const int threshold = 50;

// Pino da LED
const int ledPin = D1;

// Variável para led aceso ou não
int led_aceso = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  server.begin();
  Serial.println("WiFi conectado e servidor inicializado");
  Serial.print("Endereço IP da placa: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Variáveis para fazer com que servidor e leitura do sensor não interfiram entre si
  static unsigned long sensorTime = 0;
  static unsigned long webTime = 0;
  unsigned long currentTime = millis();

  // Espera 100ms para leitura do sensor
  if (currentTime - sensorTime >= 100) {
    int sensorValue = analogRead(sensorPin);
    if (sensorValue > threshold) {
      digitalWrite(ledPin, HIGH);
      Serial.println(sensorValue);
      led_aceso = 1;
    }
    sensorTime = currentTime;
  }

  // Espera 100ms para subir a página web
  if (currentTime - webTime >= 100) {
    WiFiClient client = server.available();
    if (client) {
      if (client.available()) {
        String requisicao = client.readStringUntil('\r');
        client.flush();
        
        if (requisicao.indexOf("ledoff") != -1) {
          digitalWrite(ledPin, LOW);
          led_aceso = 0;
          client.print("HTTP/1.1 302 Found\r\nLocation: /");
          delay(1);
          return;
        }
        
        // Página WEB
        String html = "";
        html += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        html += "<!DOCTYPE html>\r\n<html><head><meta charset=utf-8>";
        html += "<meta http-equiv='refresh' content='5'></head><body>";
        html += "<h1>Sistema de alerta de intruso.</h1>";
        
        // Se o valor for igual a 0, ou seja, se o led estiver apagado
        if (led_aceso == 0){
          html += "<p>Não tem ninguém</p>";
        }

        // Se o valor for igual a 1, ou seja, se o led estiver aceso.
        if (led_aceso == 1) {
          html += "<p>Atenção verifique se foi alguém, se não foi clique no botão.</p>";
          html += "<p><a href=\"ledoff\"><button>Não tem ninguém</button></a></p>";
        }
        html += "</body></html>";

        // Exibe a página WEB.
        client.print(html);
        delay(1);
      }
    }
    webTime = currentTime;
  }
}