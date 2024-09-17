# Controle do RPM do Cooler utilizando Controle PID

## Objetivo

O objetivo deste projeto é controlar o valor de RPM de um cooler utilizando a técnica de controle PID, garantindo que o ventilador mantenha uma rotação estável conforme o valor definido pelo usuário. O sistema ajusta o comportamento do cooler, corrigindo a diferença entre o RPM desejado e o medido, utilizando um sensor para monitorar a rotação e PWM para aplicar as correções necessárias. O projeto também permite a configuração do setpoint e o monitoramento dos resultados em tempo real via comunicação serial.

![circuito](imagens/circuito_do_projeto.jpeg)

## Materias Utilizados 

**Tabela de materiais**


| Item | Quantidade                                | 
| ------ | ----------------------------------- | 
| Cooler GMX-WFBK BK com 9 pás   | 1 UND
| Driver de motor ULN2003    |  1 UND 
| Módulo Sensor Infravermelho Tcrt5000     |  1 UND 
| Arduino Mega 2560    |  1 UND 
|Fonte de alimentação  12 V | 1 UND
|Jumpers | 15 UND


**Software**

- Arduino IDE  
- Linguagem C++
- Biblioteca controle PID 

## Resultados e Conclusão 

Com o uso do controlador PID e do PWM, o projeto demonstrou ser capaz de controlar a velocidade do cooler de forma satisfatória. No entanto, o controle ainda apresentou erros ou instabilidade quando houve mudanças de carga. Isso indica que a sintonia do controle PID não está completamente otimizada para lidar com essas variações. Acreditamos que, para melhorar o desempenho, será necessário realizar ajustes mais refinados nos parâmetros de controle PID, a fim de minimizar as oscilações e garantir uma resposta mais estável em condições variáveis.

![projeto](imagens/projeto_controle_pid.jpeg)


### Equipe 

- Heloíse Bastos **https://github.com/heloisebastos**

- Joicy Kelly **https://github.com/Joicylara**
