#include <PID_v1.h>

// Variáveis do estado do sensor de RPM
int estadoAtual = 0;
int estadoAnterior = 0;
unsigned long tempoAnterior = 0;
unsigned long tempoAtual = 0;
unsigned long tempoDeGiro = 0;
float RPM = 0;
int cont1 = 0;

// Variáveis de controle PID
double Setpoint, Input, Output;
double Kp = 0.5, Ki = 0.1 , Kd = 0;  // Constantes PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Variável de controle do cooler (PWM)
const int pinCooler = 9; // Pino de controle PWM do cooler

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); // Sensor de RPM
  pinMode(pinCooler, OUTPUT); // Pino do cooler como saída

  
  
  tempoAnterior = micros();

  // Inicializa o PID
  Setpoint = 70; // Valor alvo de RPM, inicialmente 0
  myPID.SetMode(AUTOMATIC); // Modo automático do PID
  myPID.SetOutputLimits(0, 255); // Limite do PWM (0-255)
}

void loop() {
  // Leitura do sensor de RPM
  estadoAtual = digitalRead(2);
  if (estadoAtual != estadoAnterior) {
    delayMicroseconds(1000); // Debounce
    if (estadoAtual == HIGH) {
      tempoAtual = micros();
      tempoDeGiro = tempoAtual - tempoAnterior;
      RPM = (float) (60000000 / tempoDeGiro) / 9; // Cálculo de RPM
      tempoAnterior = tempoAtual;
    }
  }
  estadoAnterior = estadoAtual;

  // Leitura do valor alvo de RPM pelo monitor serial
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    Setpoint = inputString.toFloat(); // Converte para número o valor de RPM alvo
    Serial.print("RPM Alvo Atualizado: ");
    Serial.println(Setpoint);
  }

  // Atualiza o valor de entrada para o PID
  Input = RPM;

  // Calcula o PID
  myPID.Compute();

  // Atualiza a velocidade do cooler com o valor calculado pelo PID usando analogWrite
  analogWrite(pinCooler, (int)Output);  // Converte a saída do PID para PWM (0-255)

  // Envia dados no formato CSV para o monitor serial
  if(cont1 == 3000){
    Serial.print(RPM);
    Serial.print(",");
    Serial.println(Output);
    cont1 = 0;
  } else {
    cont1++;
  }
  delayMicroseconds(1000);  // Pequeno delay para não sobrecarregar a serial
}
