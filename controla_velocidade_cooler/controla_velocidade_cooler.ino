int coolerPin = 9; // Pin PWM conectado à entrada IN1 do ULN2003

void setup() {
  Serial.begin(9600);
  pinMode(coolerPin, OUTPUT);
}

void loop() {
  // Aumentar a velocidade
  for (int i = 0; i <= 255; i++) {
    analogWrite(coolerPin, i); // Envia um sinal PWM de 0 a 255
    Serial.print("PWM Value: ");
    Serial.println(i); // Imprime o valor do PWM
    delay(500); // Aguarda 500 ms para ver a mudança de forma mais visível
  }

  // Diminuir a velocidade
  for (int i = 255; i >= 0; i--) {
    analogWrite(coolerPin, i);
    Serial.print("PWM Value: ");
    Serial.println(i); // Imprime o valor do PWM
    delay(500); // Aguarda 500 ms para ver a mudança de forma mais visível
  }
}