//Sensor de som leitura digital
//não deu

int pinoLed = 12; //PINO DIGITAL UTILIZADO PELO LED
int pinoSensor = 4; //PINO DIGITAL UTILIZADO PELO SENSOR
int estadoSensor; //VARIÁVEL QUE ARMAZENA O ESTADO DA SAÍDA DO SENSOR (HIGH / LOW)
 
void setup(){
  pinMode(pinoSensor, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(pinoLed, OUTPUT); //DEFINE O PINO COMO SAÍDA
}
 
void loop(){
  if (digitalRead(pinoSensor) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
      estadoSensor = digitalRead(pinoLed); //LÊ O ESTADO ATUAL DO PINO E ARMAZENA NA VARIÁVEL
      digitalWrite(pinoLed, !estadoSensor); //MUDA O ESTADO ATUAL DO LED COMPARANDO COM O ESTADO
      //ARMAZENADO NA VARIÁVEL estadoSensor
      delay(500); //INTERVALO DE 500 MILISSEGUNDOS
  }
}