# adc-multiplexado-expansao

## Sistema de Expansao Analogica com Multiplexacao

Este projeto implementa um sistema de monitoramento de sensores (MQ2, LDR e NTC) utilizando o microcontrolador Raspberry Pi Pico W. O diferencial tecnico e o uso de um Multiplexador CD74HC4067, que permite ler multiplos sinais analogicos atraves de uma unica porta ADC, otimizando os recursos da placa BitDogLab.

---

## Objetivo do Projeto

O objetivo principal e demonstrar a viabilidade da expansao de portas analogicas para monitorar a variacao de tensao nos seguintes sensores:

* **MQ2:** Deteccao de variacao de fumaca e gases inflamaveis.
* **LDR:** Monitoramento da variacao de luminosidade ambiente.
* **NTC:** Verificacao da variacao de temperatura local.

O sistema processa essas variacoes e as exibe em tempo real em um Display OLED 128x64 via protocolo I2C.

---

## Hardware Necessario

* Microcontrolador: Raspberry Pi Pico W.
* Placa de Expansao: BitDogLab.
* Multiplexador: CD74HC4067 (16 canais).
* Sensores: MQ2, LDR e NTC (Termistor).
* Display: OLED 0.96" I2C (SSD1306).
* Controles: 2 Botoes (A para iniciar, B para interrupcao/menu) e LEDs de status.

---

## Dependencias e Requisitos

Para compilar e executar este projeto, voce precisara de:

1.  **SDK do Raspberry Pi Pico:** Configurado no seu ambiente (C/C++).
2.  **Extensao Raspberry Pi Pico para VS Code:** Para facilitar o build e o flash.
3.  **Bibliotecas incluidas no projeto:**
    * `hardware/adc.h` (Leitura analogica)
    * `hardware/i2c.h` (Comunicacao com o display)
    * `hardware/uart.h` (Interface com o console serial)
    * `ssd1306.h` (Driver do display OLED)

---

## Instrucoes de Instalacao e Execucao

O projeto pode ser testado de duas maneiras: atraves do hardware real ou por meio de simulacao em ambiente virtual.

### 1. Instalacao Local (Hardware)

Para executar o codigo na placa BitDogLab ou Raspberry Pi Pico W, siga os passos abaixo:

1.  **Clonagem do Repositorio:**
    ```bash
    git clone [https://github.com/maarcsx/adc-multiplexado-expansao.git](https://github.com/maarcsx/adc-multiplexado-expansao.git)
    ```
2.  **Importacao do Projeto:** Abra a pasta no VS Code com a extensao Raspberry Pi Pico devidamente configurada.
3.  **Verificacao de Arquivos:** Certifique-se de que o arquivo `main.c` e as pastas de drivers (`mq2` e `ssd1306`) estao na raiz do projeto.
4.  **Compilacao e Flash:** Utilize as ferramentas do SDK para compilar o projeto e carregar o arquivo `.uf2` no microcontrolador.

### 2. Simulacao Online (Wokwi)

Para validar a logica de funcionamento e a variacao de tensao sem a necessidade de componentes fisicos, utilize o ambiente virtual abaixo:

![Diagrama do Projeto no Wokwi](Diagrama%20Wokwi.png)

**Link para simulacao:** [https://wokwi.com/projects/460514569439680513](https://wokwi.com/projects/460514569439680513)

---

## Licenca

Este projeto esta licenciado sob a MIT License. Para mais informacoes, consulte o arquivo [LICENSE](./LICENSE) no repositorio.

---

## Autor

**Marcos Crisostomo** 
Tecnico em Eletrotecnica | Automacao Industrial  
Projeto integrante da capacitacao em sistemas embarcados - **EmbarcaTech 2026**