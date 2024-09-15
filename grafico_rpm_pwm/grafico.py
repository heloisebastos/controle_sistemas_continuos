import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import tkinter as tk
from tkinter import simpledialog

# Função para abrir a porta serial
def abrir_porta_serial():
    try:
        return serial.Serial('COM8', 9600, timeout=1)
    except serial.SerialException as e:
        print(f"Erro ao abrir a porta serial: {e}")
        return None

ser = abrir_porta_serial()

# Função para enviar o valor de RPM para o Arduino
def enviar_valor_rpm(valor_rpm):
    if ser:
        try:
            ser.write(f"{valor_rpm}\n".encode('utf-8'))
        except serial.SerialException as e:
            print(f"Erro ao enviar dados: {e}")

# Função para ajustar o valor de RPM usando uma caixa de diálogo
def ajustar_rpm():
    valor_rpm = simpledialog.askfloat("Ajustar RPM", "Digite o valor de RPM:")
    if valor_rpm is not None:
        enviar_valor_rpm(valor_rpm)

# Listas para armazenar os dados para o gráfico
tempos = []
rpm_values = []
pwm_values = []

# Função de inicialização da animação
def init():
    ax_rpm.set_xlim(0, 100)
    ax_rpm.set_ylim(0, 1000)
    ax_pwm.set_xlim(0, 100)
    ax_pwm.set_ylim(0, 255)
    return line_rpm, line_pwm

# Função de atualização da animação
def atualizar(i):
    if ser and ser.in_waiting > 0:
        try:
            linha = ser.readline().decode('utf-8').strip()
            dados = linha.split(',')  # Divide a linha em valores separados por vírgulas
            if len(dados) >= 2:
                tempo_atual = len(tempos)  # Use o comprimento da lista como substituto do tempo real
                rpm = float(dados[0])
                pwm = float(dados[1])

                # Adicione os valores recebidos às listas
                tempos.append(tempo_atual)
                rpm_values.append(rpm)
                pwm_values.append(pwm)

                if len(tempos) > 100:
                    tempos.pop(0)
                    rpm_values.pop(0)
                    pwm_values.pop(0)

                # Atualizar os dados nos gráficos
                line_rpm.set_data(tempos, rpm_values)
                line_pwm.set_data(tempos, pwm_values)

        except ValueError:
            pass

    # Ajustar os eixos para mostrar o intervalo de dados
    if len(tempos) > 1:
        ax_rpm.set_xlim(max(0, len(tempos) - 100), len(tempos))
        ax_pwm.set_xlim(max(0, len(tempos) - 100), len(tempos))

    return line_rpm, line_pwm

# Configuração do gráfico com dois subplots
fig, (ax_rpm, ax_pwm) = plt.subplots(2, 1, sharex=True, figsize=(10, 6))
line_rpm, = ax_rpm.plot([], [], label='RPM', color='blue')
line_pwm, = ax_pwm.plot([], [], label='PWM', color='red')

# Configurações dos eixos
ax_rpm.set_xlabel('Tempo')
ax_rpm.set_ylabel('RPM')
ax_rpm.set_title('Gráfico de RPM')
ax_rpm.legend()

ax_pwm.set_xlabel('Tempo')
ax_pwm.set_ylabel('PWM')
ax_pwm.set_title('Gráfico de PWM')
ax_pwm.legend()

# Criação da animação
ani = animation.FuncAnimation(
    fig, atualizar, init_func=init, interval=1000, blit=True, cache_frame_data=False
)

# Configuração da interface gráfica
root = tk.Tk()
root.title("Controle de RPM")
root.geometry("300x100")

ajustar_button = tk.Button(root, text="Ajustar RPM", command=ajustar_rpm)
ajustar_button.pack(pady=20)

# Mostrar a interface gráfica e o gráfico
plt.show(block=False)
root.mainloop()
