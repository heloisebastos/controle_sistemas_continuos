#include <PID_v1.h>

// Variáveis globais
volatile float rev = 0; 
double rpm, setpoint = 1000; // Valor desejado de RPM
double input, output;
unsigned long oldtime = 0;
unsigned long time;
int numPas = 9;
#define coolerPin 9
#define sensorPin 2

// Parâmetros PID
double Kp = 1.0, Ki = 0.2, Kd = 0.1;

// Criação do objeto PID
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

void isr() {
  rev++;
}

void setup() {
  Serial.begin(9600);
  pinMode(coolerPin, OUTPUT);
  analogWrite(coolerPin, 255);

  attachInterrupt(digitalPinToInterrupt(sensorPin), isr, RISING);

  oldtime = millis();

  // Inicializa o PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255);  // Limita a saída para PWM (0 a 255)

  Serial.println("Entre com o valor desejado de RPM.");
}

void loop() {
  time = millis() - oldtime;

  // Verifica se há dados disponíveis na porta serial
  if (Serial.available() > 0) {
    // Lê os dados da serial
    String inputString = Serial.readStringUntil('\n');
    double newSetpoint = inputString.toDouble();

    // Define limites para o RPM
    if (newSetpoint >= 500 && newSetpoint <= 3000) { // Ajuste os limites conforme necessário
      setpoint = newSetpoint;
      Serial.print("Novo setpoint (RPM): ");
      Serial.println(setpoint);
    } else {
      Serial.println("Valor de RPM fora do intervalo permitido (500-3000).");
    }
  }

  if (rev >= numPas) {
    detachInterrupt(digitalPinToInterrupt(sensorPin));

    float rotations = rev / numPas;
    rpm = (rotations * 60000) / time;
    rev = 0;
    oldtime = millis();

    // Atualiza o valor de entrada do PID
    input = rpm;

    // Calcula o novo valor de saída (PWM)
    myPID.Compute();

    // Aplica o valor de saída ao cooler
    analogWrite(coolerPin, constrain(output, 0, 255)); // Constrange o valor de saída entre 0 e 255
    

    // Imprime dados para depuração
    Serial.print("Time (ms): ");
    Serial.println(time);
    Serial.print("RPM: ");
    Serial.println(rpm);
    Serial.print("Output PWM: ");
    Serial.println(output);

    attachInterrupt(digitalPinToInterrupt(sensorPin), isr, RISING);
  }

  delay(1000); // Aumenta o delay para evitar sobrecarga e permitir mais estabilidade
}
