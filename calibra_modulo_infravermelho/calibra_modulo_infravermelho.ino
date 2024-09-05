int sensorPin = 2;  // Pino digital conectado ao OUT do sensor
int sensorValue = 0;

void setup() {
  Serial.begin(9600);  // Inicializa a comunicação serial
  pinMode(sensorPin, INPUT);  // Define o pino do sensor como entrada
}

void loop() {
  sensorValue = digitalRead(sensorPin);  // Lê o valor do sensor
  Serial.println(sensorValue);  // Imprime o valor lido (0 ou 1)

  delay(500);  // Espera 500 ms antes de ler novamente
}