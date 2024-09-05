// Definições de pinos
const int sensorPin = 2;  // Pino digital para o módulo infravermelho
const int coolerPin = 9;  // Pino PWM para o cooler

// Variáveis para armazenar os tempos
volatile unsigned long pulseCount = 0;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
unsigned long period = 0;  // Tempo para uma volta completa em milissegundos
int pwmValue = 0;  // Valor PWM para controlar a velocidade do cooler
const int pulsesPerRevolution = 9;  // Número de pulsos por volta completa

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(coolerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, RISING); // Interrupção para contar pulsos
  startTime = millis(); // Inicializa o tempo de início
}

void loop() {
  // A cada 1 segundo, calcule o tempo para uma volta completa e ajuste a velocidade
  if (millis() - startTime >= 1000) {
    noInterrupts();  // Desabilite interrupções enquanto calcula
    unsigned long count = pulseCount;
    pulseCount = 0;
    interrupts();  // Reabilite interrupções

    elapsedTime = millis() - startTime;
    if (count >= pulsesPerRevolution) {
      period = (elapsedTime * 1000) / (count / pulsesPerRevolution);  // Calcula o período em milissegundos
    } else {
      period = 0;  // Evita divisão por zero
    }

    Serial.print("Tempo para uma volta completa: ");
    Serial.print(period);
    Serial.println(" ms");

    // Ajusta a velocidade do cooler com base no tempo de uma volta completa
    if (period > 0) {
      pwmValue = map(period, 0, 10000, 255, 0);  // Mapeia o tempo para o intervalo de PWM
      pwmValue = constrain(pwmValue, 0, 255);  // Garante que o valor está dentro dos limites válidos
    } else {
      pwmValue = 0;  // Se period é zero, desliga o cooler
    }

    analogWrite(coolerPin, pwmValue);  // Ajusta a velocidade do cooler
    Serial.print("Velocidade do cooler: ");
    Serial.print(pwmValue);
    Serial.println(" (0 a 255)");

    startTime = millis(); // Atualiza o tempo de início
  }

  // Aumentar a velocidade do cooler gradualmente
  for (int i = 0; i <= 255; i++) {
    analogWrite(coolerPin, i); // Envia um sinal PWM de 0 a 255
    Serial.print("PWM Value: ");
    Serial.println(i); // Imprime o valor do PWM
    delay(500); // Aguarda 500 ms para ver a mudança de forma mais visível
  }
}

// Função chamada na interrupção para contar pulsos
void countPulse() {
  pulseCount++;
}
