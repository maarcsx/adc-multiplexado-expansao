# adc-multiplexado-expansao
## Sistema de Expansão Analógica com Multiplexação

Este projeto implementa um sistema de monitoramento de sensores (MQ2, LDR e NTC) utilizando o microcontrolador **Raspberry Pi Pico W**. O diferencial técnico é o uso de um **Multiplexador CD74HC4067**, que permite ler múltiplos sinais analógicos através de uma única porta ADC, otimizando os recursos da placa BitDogLab.

---

##  Objetivo do Projeto

O objetivo principal é demonstrar a viabilidade da expansão de portas analógicas para monitorar a **variação de tensão** nos seguintes sensores:
- **MQ2:** Detecção de variação de fumaça e gases inflamáveis.
- **LDR:** Monitoramento da variação de luminosidade ambiente.
- **NTC:** Verificação da variação de temperatura local.

O sistema processa essas variações e as exibe em tempo real em um **Display OLED 128x64** via protocolo I2C.

---

##  Hardware Necessário

- **Microcontrolador:** Raspberry Pi Pico W.
- **Placa de Expansão:** BitDogLab.
- **Multiplexador:** CD74HC4067 (16 canais).
- **Sensores:** MQ2, LDR e NTC (Termistor).
- **Display:** OLED 0.96" I2C (SSD1306).
- **Controles:** 2 Botões (A para iniciar, B para interrupção/menu) e LEDs de status.

---

##  Dependências e Requisitos

Para compilar e executar este projeto, você precisará de:

1.  **SDK do Raspberry Pi Pico:** Configurado no seu ambiente (C/C++).
2.  **Extensão Raspberry Pi Pico para VS Code:** Para facilitar o build e o flash.
3.  **Bibliotecas incluídas no projeto:**
    - `hardware/adc.h` (Leitura analógica)
    - `hardware/i2c.h` (Comunicação com o display)
    - `hardware/uart.h` (Interface com o console serial)
    - `ssd1306.h` (Driver do display OLED)

---

##  Instruções de Instalação e Execução

 1. Clonar o Repositório
```bash
git clone https://github.com/maarcsx/adc-multiplexado-expansao.git
```

2. Verifique se todos os arquivos do projeto estão na sua maquina
3. Ou tambem pode apenas simular no wokwi clicando [AQUI] (https://wokwi.com/projects/460514569439680513)