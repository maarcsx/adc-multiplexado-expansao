/*Projeto Final Embarcatech (Abril 2026)- Sistema de Expansão Analógica com Multiplexação*/

// =========================================================================
// BIBLIOTECAS
// =========================================================================
#include "pico/stdlib.h"   
#include "hardware/uart.h" 
#include "hardware/i2c.h"  
#include "ssd1306.h"       
#include "hardware/adc.h"  
#include "stdio.h"         

// =========================================================================
// DEFINIÇÕES DE PINOS (HARDWARE)
// =========================================================================
#define I2C_PORT i2c1 
#define SDA_PIN 14 
#define SCL_PIN 15 
#define SENSOR 28     // Pino ADC principal
#define S0 19         // Seleção MUX bit 0
#define S1 18         // Seleção MUX bit 1
#define S2 17         // Seleção MUX bit 2
#define S3 16         // Seleção MUX bit 3
#define LED_R 13      // Led de Status (Vermelho)
#define LED_G 11      // Led de Status (Verde)
#define BOTAO_A 5     // Botão de Inicialização
#define BOTAO_B 6     // Botão de Interrupção

// =========================================================================
// VARIÁVEIS GLOBAIS E ESTADOS
// =========================================================================
float TEMP = 0.0; 
float MQ2 = 0.0; 
float LDR = 0.0; 

// Flags de controle de fluxo (Volatile para garantir leitura correta)
volatile bool FUNCAO_MQ2 = false;
volatile bool FUNCAO_LDR = false;
volatile bool FUNCAO_TEMP = false;
volatile bool PRINCIPAL = false;

bool ESTADO_ATUAL = false;  // Controla se o sistema está ligado/desligado
bool SISTEMA = false;       // Gatilho para o menu do console

// Buffers para exibição no Display
char TENSAO_TEMP [10];  
char TENSAO_MQ2 [10];  
char TENSAO_LDR [10];  

// Configurações de UART
#define UART0_ID uart0
#define UART0_TX_PIN 0
#define UART0_RX_PIN 1
#define UART1_ID uart1
#define UART1_TX_PIN 8
#define UART1_RX_PIN 9
#define BAUD_RATE 4800

// =========================================================================
// FUNÇÕES DE SUPORTE UART
// =========================================================================

void uart_config(uart_inst_t *uart) {
    uart_set_format(uart, 8, 1, UART_PARITY_NONE);
    uart_set_hw_flow(uart, false, false);
    uart_set_fifo_enabled(uart, false);
}

void clean_uart_buffer(uart_inst_t *uart) {
    while (uart_is_readable(uart)) {
        uart_getc(uart);
    }
}

// =========================================================================
// INICIALIZAÇÃO DO SISTEMA
// =========================================================================

void inicializa() {
    // Inicialização do Multiplexador (Saídas)
    gpio_init(S0); gpio_set_dir(S0, GPIO_OUT);
    gpio_init(S1); gpio_set_dir(S1, GPIO_OUT);
    gpio_init(S2); gpio_set_dir(S2, GPIO_OUT);
    gpio_init(S3); gpio_set_dir(S3, GPIO_OUT);

    // Inicialização de LEDs e Botões
    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT); gpio_put(LED_R, true);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT); gpio_put(LED_G, false);
    gpio_init(BOTAO_A); gpio_set_dir(BOTAO_A, GPIO_IN); gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B); gpio_set_dir(BOTAO_B, GPIO_IN); gpio_pull_up(BOTAO_B);

    // Inicialização UARTs
    uart_init(UART0_ID, BAUD_RATE);
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);
    uart_config(UART0_ID);

    uart_init(UART1_ID, BAUD_RATE);
    gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART);
    uart_config(UART1_ID);

    // Inicialização I2C e Display OLED
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    ssd1306_init(I2C_PORT);
    ssd1306_clear();
}

void inter_callback(uint gpio, uint32_t events) {
    SISTEMA = true; // Callback para retornar ao menu pelo botão B
}

// =========================================================================
// FUNÇÕES DE LEITURA DOS SENSORES
// =========================================================================

void DADOS_MQ2() {
    gpio_put(S3, 0); gpio_put(S2, 0); gpio_put(S1, 0); gpio_put(S0, 0); // Canal 0
    sleep_ms(100);
    MQ2 = adc_read() * 3.3f / 4095.0f;
    sleep_ms(100);
    sprintf(TENSAO_MQ2, "MQ2: %.3fV", MQ2);
    ssd1306_clear();
    ssd1306_draw_string(0, 0, "SISTEMA LIGADO: ", true);
    ssd1306_draw_string(0, 10, TENSAO_MQ2, true);
    ssd1306_draw_string(0, 45, "PARA PARAR", true);
    ssd1306_draw_string(0, 55, "PRECIONE O BOTAO B", true);
    ssd1306_update(I2C_PORT);
}

void DADOS_LDR() {
    gpio_put(S3, 0); gpio_put(S2, 0); gpio_put(S1, 0); gpio_put(S0, 1); // Canal 1
    sleep_ms(100);
    LDR = adc_read() * 3.3f / 4095.0f;
    sleep_ms(100);
    sprintf(TENSAO_LDR, "LDR: %.3fV", LDR);
    ssd1306_clear();
    ssd1306_draw_string(0, 0, "SISTEMA LIGADO: ", true);
    ssd1306_draw_string(0, 20, TENSAO_LDR, true);
    ssd1306_draw_string(0, 45, "PARA PARAR", true);
    ssd1306_draw_string(0, 55, "PRECIONE O BOTAO B", true);
    ssd1306_update(I2C_PORT);
}

void DADOS_TEMP() {
    gpio_put(S3, 0); gpio_put(S2, 0); gpio_put(S1, 1); gpio_put(S0, 0); // Canal 2
    sleep_ms(100);
    TEMP = adc_read() * 3.3f / 4095.0f;
    sleep_ms(100);
    sprintf(TENSAO_TEMP, "TEMP: %.3fV", TEMP);
    ssd1306_clear();
    ssd1306_draw_string(0, 0, "SISTEMA LIGADO: ", true);
    ssd1306_draw_string(0, 30, TENSAO_TEMP, true);
    ssd1306_draw_string(0, 45, "PARA PARAR", true);
    ssd1306_draw_string(0, 55, "PRECIONE O BOTAO B", true);
    ssd1306_update(I2C_PORT);
}

void DADOS_PRINCIPAIS() {
    // Leitura Sequencial via MUX para o modo "TODOS"
    gpio_put(S3, 0); gpio_put(S2, 0); gpio_put(S1, 0); gpio_put(S0, 0);
    sleep_ms(100);
    MQ2 = adc_read() * 3.3f / 4095.0f;
    sleep_ms(50);

    gpio_put(S3, 0); gpio_put(S2, 0); gpio_put(S1, 0); gpio_put(S0, 1);
    sleep_ms(100);
    LDR = adc_read() * 3.3f / 4095.0f;
    sleep_ms(50);

    gpio_put(S3, 0); gpio_put(S2, 0); gpio_put(S1, 1); gpio_put(S0, 0);
    sleep_ms(100);
    TEMP = adc_read() * 3.3f / 4095.0f;
    sleep_ms(50);

    sprintf(TENSAO_MQ2, "MQ2: %.3fV", MQ2);
    sprintf(TENSAO_LDR, "LDR: %.3fV", LDR);
    sprintf(TENSAO_TEMP, "TEMP: %.3fV", TEMP);

    ssd1306_clear();
    ssd1306_draw_string(0, 0, "SISTEMA LIGADO: ", true);
    ssd1306_draw_string(0, 10, TENSAO_MQ2, true);
    ssd1306_draw_string(0, 20, TENSAO_LDR, true);
    ssd1306_draw_string(0, 30, TENSAO_TEMP, true);
    ssd1306_draw_string(0, 45, "PARA PARAR", true);
    ssd1306_draw_string(0, 55, "PRECIONE O BOTAO B", true);
    ssd1306_update(I2C_PORT);
}

// =========================================================================
// LÓGICA DO CONSOLE E UART
// =========================================================================

void console() {
    char input_data;
    if (scanf(" %c", &input_data) == 1) {
        clean_uart_buffer(UART1_ID);
        uart_putc(UART0_ID, input_data);
        uart_tx_wait_blocking(UART0_ID);
        sleep_ms(100);

        if (uart_is_readable(UART1_ID)) {
            char recebido = uart_getc(UART1_ID);
            printf("\nComando recebido: %c\n", recebido);

            if ((recebido == 'a') || (recebido == 'A')) {
                printf("Executando leitura de todos os Sensores...\nVisualizar no display OLED.");
                PRINCIPAL = true; FUNCAO_MQ2 = false; FUNCAO_LDR = false; FUNCAO_TEMP = false;
            }
            else if ((recebido == 'b') || (recebido == 'B')) {
                printf("Executando leitura do Sensor MQ2...\nVisualizar no display OLED.");
                PRINCIPAL = false; FUNCAO_MQ2 = true; FUNCAO_LDR = false; FUNCAO_TEMP = false;
            }
            else if ((recebido == 'c') || (recebido == 'C')) {
                printf("Executando leitura do Sensor LDR...\nVisualizar no display OLED.");
                PRINCIPAL = false; FUNCAO_MQ2 = false; FUNCAO_LDR = true; FUNCAO_TEMP = false;
            }
            else if ((recebido == 'd') || (recebido == 'D')) {
                printf("Executando leitura do Sensor de Temperatura...\nVisualizar no display OLED.");
                PRINCIPAL = false; FUNCAO_MQ2 = false; FUNCAO_LDR = false; FUNCAO_TEMP = true;
            }
            else {
                printf("\nNenhum comando para isto...");
                printf("\nVoltando ao último comando presente na memória. Aguarde...");
                printf("\nCaso não tenha nada na memoria precione o botão B ");
                sleep_ms(1000);
            }
        }
    }
}

// =========================================================================
// FUNÇÃO PRINCIPAL
// =========================================================================

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(SENSOR);
    adc_select_input(2); // Inicia ADC no canal 2 (GP28)

    inicializa();

    // Habilita interrupção no Botão B
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &inter_callback);

    // Mensagem de Boas-vindas
    ssd1306_draw_string(17, 0, "EMBARCATECH 2026", true);
    ssd1306_draw_string(15, 50, "MARCOS CRISOSTOMO", true);
    ssd1306_update(I2C_PORT);
    printf("SEJA BEM-VINDO, AGUARDE A INICIALIZAÇÃO.\n");
    sleep_ms(1000);

    while (true) {
        // Lógica para ligar o sistema pelo Botão A
        if ((gpio_get(BOTAO_A) == 0) && (!ESTADO_ATUAL)) {
            ESTADO_ATUAL = true;
            SISTEMA = true;
            printf("INICIANDO...\n");
            ssd1306_clear();
            ssd1306_draw_string(0, 0, "SISTEMA INICIANDO", true);
            ssd1306_draw_string(0, 10, "AGUARDE ...", true);
            ssd1306_update(I2C_PORT);
            sleep_ms(1000);
            gpio_put(LED_G, true);
            gpio_put(LED_R, false);
        }

        // Se SISTEMA for true, abre o menu de comandos no console
        if (SISTEMA) {
            printf("\nSistema inicializado. Digite um caractere no console:\n");
            printf("'A': LER TODOS\n'B': LER MQ2\n'C': LER LDR\n'D': LER TEMP\n");
            ssd1306_clear();
            ssd1306_draw_string(0, 0, "SIGA AS INSTRUCOES", true);
            ssd1306_draw_string(0, 10, "NO CONSOLE", true);
            ssd1306_update(I2C_PORT);
            console();
            SISTEMA = false;
        }

        // Execução Contínua baseada na escolha do usuário
        if (PRINCIPAL)   DADOS_PRINCIPAIS();
        if (FUNCAO_MQ2)  DADOS_MQ2();
        if (FUNCAO_LDR)  DADOS_LDR();
        if (FUNCAO_TEMP) DADOS_TEMP();

        // Se o sistema estiver desligado (Standby)
        if (ESTADO_ATUAL == false) {
            ssd1306_clear();
            ssd1306_draw_string(0, 0, "SISTEMA DESLIGADO", true);
            ssd1306_draw_string(0, 10, "PARA LIGAR PRECIONE A", true);
            ssd1306_update(I2C_PORT);
            gpio_put(LED_G, false);
            gpio_put(LED_R, true);
        }
    }
    return 0;
}