#include <PID_v1.h>

// Variáveis globais
volatile float rev = 0; 
double rpm, setpoint = 1000; // Valor desejado de RPM
double input, output;
unsigned long previousMillis = 0; // Tempo anterior para cálculo do RPM
unsigned long serialMillis = 0; // Tempo anterior para enviar dados pela Serial
unsigned long currentMillis; // Tempo atual
int numPas = 9; // Número de pás do ventilador
#define coolerPin 9
#define sensorPin 2

// Parâmetros PID ajustados
double Kp = 0.9, Ki = 0.02, Kd = 0.1; // Ajuste os valores conforme necessário

// Criação do objeto PID
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

#define NUM_READINGS 10
float readings[NUM_READINGS]; // Array para armazenar as leituras
int readIndex = 0;
float total = 0;
float average = 0;

volatile bool lastState = LOW; // Armazena o estado anterior do sensor

void isr() {
  bool currentState = digitalRead(sensorPin); // Lê o estado atual do pino

  // Verifica se houve uma transição de LOW para HIGH
  if (lastState == LOW && currentState == HIGH) {
    rev++; // Incrementa a contagem de pás detectadas
  }

  lastState = currentState; // Atualiza o estado anterior
}

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando..."); // Mensagem inicial para verificação
  pinMode(coolerPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  // Inicializa o PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255);  // Limita a saída para PWM (0 a 255)

  // Configura interrupção
  attachInterrupt(digitalPinToInterrupt(sensorPin), isr, RISING);

  // Inicializa o array de leituras
  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = 0;
  }
}

void loop() {
  currentMillis = millis(); // Atualiza o tempo atual

  if (Serial.available() > 0) {
    // Lê os dados da serial
    String inputString = Serial.readStringUntil('\n');
    double newSetpoint = inputString.toDouble();
    setpoint = newSetpoint;
    Serial.print("Novo setpoint (RPM): ");
    Serial.println(setpoint);
  }

  // Calcula RPM a cada 200ms
  if (currentMillis - previousMillis >= 200) {
    float rotations = rev / numPas;
    rpm = (rotations * 600); // Calcula RPM

    // Atualiza a média móvel
    total = total - readings[readIndex];
    readings[readIndex] = rpm;
    total = total + readings[readIndex];
    readIndex = (readIndex + 1) % NUM_READINGS;
    average = total / NUM_READINGS;

    rev = 0; // Reseta contador de revoluções
    previousMillis = currentMillis; // Atualiza o tempo anterior
  }

  // Atualiza o valor de entrada do PID
  input = average;

  myPID.Compute(); // Calcula a saída PID

  // Aplicar faixa de histerese
  if (output > 10) {
    output = output; // Se a saída for maior que o limiar, mantenha-a
  } else {
    output = 10; // Caso contrário, defina o PWM para o mínimo permitido
  }

  // Aplica o valor de saída ao cooler
  analogWrite(coolerPin, constrain(output, 0, 255));

  // Envia dados para a serial a cada 100ms
  if (currentMillis - serialMillis >= 100) {
    serialMillis = currentMillis;
    Serial.print("Setpoint: ");
    Serial.print(setpoint);
    Serial.print(" RPM: ");
    Serial.print(average);
    Serial.print(" Output PWM: ");
    Serial.println(output);
  }
}