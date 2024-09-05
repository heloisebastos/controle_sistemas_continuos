volatile int pulseCount = 0;  // Contador de pulsos
unsigned long lastTime = 0;
float rpm = 0;
int pwmPin = 9;  // Pino PWM onde o cooler está conectado
int pwmValue = 0;  // Valor PWM inicial
int pwmStep = 5;  // Incremento do valor PWM
int pwmDelay = 10; // Intervalo de tempo entre incrementos (em milissegundos)

void setup() {
  Serial.begin(9600);
  
  // Configura o pino do sensor de pulsos
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), countPulse, RISING); // Contar pulsos na borda de subida

  // Configura o pino PWM
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastTime >= 1000) { // Atualizar a cada 1 segundo
    detachInterrupt(digitalPinToInterrupt(2));
    
    rpm = (pulseCount * 60.0); // Calcular RPM
    pulseCount = 0;
    lastTime = currentTime;
    
    Serial.print("RPM: ");
    Serial.println(rpm);
    
    attachInterrupt(digitalPinToInterrupt(2), countPulse, RISING); // Reativar interrupção
  }

  // Controlar PWM
  analogWrite(pwmPin, pwmValue); // Define o valor do PWM

  // Atualizar o valor do PWM
  pwmValue += pwmStep; // Incrementa o valor PWM

  // Reverter o valor do PWM quando atinge o máximo ou mínimo
  if (pwmValue >= 255 || pwmValue <= 0) {
    pwmStep = -pwmStep; // Inverte a direção do incremento
    delay(pwmDelay); // Pausa para suavizar a variação
  }

  delay(pwmDelay); // Pausa para suavizar a variação
}

void countPulse() {
  pulseCount++;
}
