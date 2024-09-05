volatile int pulseCount = 0;  // Contador de pulsos
unsigned long lastPulseTime = 0;  // Tempo do último pulso
unsigned long lastTime = 0;  // Tempo do último cálculo
float rpmPerMinute = 0;
float rpmPerRevolution = 0;  // RPM para uma volta (baseado em um pulso)

int pwmPin = 9;  // Pino PWM onde o cooler está conectado
int pwmValue = 0;  // Valor PWM inicial
int pwmStep = 5;  // Incremento do valor PWM
int pwmDelay = 10; // Intervalo de tempo entre incrementos (em milissegundos)

int numBlades = 9;  // Número de pás do cooler

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
  
  // Cálculo do RPM para uma volta
  if (pulseCount > 0 && (currentTime - lastPulseTime) >= 100) { // Atualiza a cada 100 ms para maior precisão
    float timePerPulse = (currentTime - lastPulseTime) / 1000.0; // Tempo em segundos
    rpmPerRevolution = (60.0 / timePerPulse) / numBlades; // Calcular RPM para uma volta
    lastPulseTime = currentTime; // Atualiza o tempo do último pulso
    pulseCount = 0; // Zera o contador de pulsos
    Serial.print("RPM para uma volta: ");
    Serial.println(rpmPerRevolution);
  }
  
  // Cálculo do RPM por minuto
  if (currentTime - lastTime >= 60000) { // Atualiza a cada 1 minuto
    detachInterrupt(digitalPinToInterrupt(2));
    
    rpmPerMinute = (pulseCount * 60.0) / numBlades; // Calcula RPM por minuto
    pulseCount = 0;
    lastTime = currentTime;
    
    Serial.print("RPM por minuto: ");
    Serial.println(rpmPerMinute);
    
    attachInterrupt(digitalPinToInterrupt(2), countPulse, RISING); // Reativar interrupção
  }
  
  // Controle PWM
  analogWrite(pwmPin, pwmValue); // Define o valor do PWM
  
  // Atualizar o valor do PWM
  pwmValue += pwmStep; // Incrementa o valor PWM

  // Reverter o valor do PWM quando atinge o máximo ou mínimo
  if (pwmValue >= 255 || pwmValue <= 0) {
    pwmStep = -pwmStep; // Inverte a direção do incremento
  }

  delay(pwmDelay); // Pausa para suavizar a variação
}

void countPulse() {
  pulseCount++;
}
