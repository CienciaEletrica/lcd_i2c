/**
 * @file example.c
 * @example example.c Exemplo prático de aplicação da biblioteca lcd_i2c em AVR.
 * @author Tiago Henrique dos Santos/Ciência Elétrica
 * @date 2026
 * 
 * Este exemplo demonstra o fluxo completo de uso da biblioteca:
 * 1. Configuração do hardware TWI (I2C) nativo do AVR.
 * 2. Injeção de dependência (ponteiros de função para I2C e Delay).
 * 3. Criação de glifos customizados na CGRAM do LCD.
 * 4. Implementação de uma barra de progresso animada.
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h> 

#include "twi.h" // Driver de hardware I2C
#include "lcd_i2c.h" // Driver de alto nível do LCD

/** @brief Flag de controle de fluxo para o barramento TWI. */
volatile uint8_t twi_busy = 0;

/** 
 * @name Definição de Caracteres Customizados
 * Matrizes de pixels 5x8 para ícones personalizados.
 * @{ 
 */

/** @brief Ícone de bateria com carga parcial. */
uint8_t icone_bateria[8] = {
    0b01110, // [ Linha 0 ] - Topo (conector)
    0b11111, // [ Linha 1 ] - Ombro da bateria
    0b10001, // [ Linha 2 ] - Lateral vazia
    0b10101, // [ Linha 3 ] - Lateral com carga central
    0b10101, // [ Linha 4 ] - Lateral com carga central
    0b10001, // [ Linha 5 ] - Lateral vazia
    0b11111, // [ Linha 6 ] - Base
    0b11111  // [ Linha 7 ] - Base (contorno inferior)
};

/** @brief Bloco sólido preenchido para barras de progresso. */
uint8_t bloco_cheio[8] = {
    0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111
};
/** @} */

/** @brief Instância global do LCD (Objeto de controle). */
lcd_t display; 

// Protótipos das funções de suporte (Callbacks e Bridges)
void on_twi_complete(TWI_Status_t status, const uint8_t* data, size_t length);
int8_t my_i2c_write(uint8_t addr, uint8_t data);
void my_delay_ms(uint16_t ms);

/**
 * @brief Ponto de entrada principal da aplicação.
 */
int main(void) {
    // 1. Configurações Iniciais de Hardware
    sei(); // Habilita interrupções globais (necessário para o TWI)
    twi_init(TWI_MODE_MASTER, 0, TWI_CLOCK_100KHZ);
    twi_master_register_callbacks(on_twi_complete, NULL);
    _delay_ms(200); // Aguarda a estabilização elétrica do LCD

    // 2. Configuração do Objeto LCD (Injeção de Dependência)
    display.address   = 0x27;           // Endereço I2C do PCF8574
    display.columns   = 16;             // LCD 16x2
    display.rows      = 2;
    display.backlight = LCD_BACKLIGHT_ON; 
    display.i2c_write = my_i2c_write;   // Vincula função de escrita I2C
    display.delay_ms  = my_delay_ms;    // Vincula função de atraso (ms)
    
    // 3. Inicialização e Lógica de Exibição
    if (lcd_init(&display) == LCD_OK) {
        // Grava os desenhos customizados nos índices 0 e 1 da CGRAM
        lcd_create_custom_char(&display, 0, icone_bateria); // Posição 0
        lcd_create_custom_char(&display, 1, bloco_cheio);   // Posição 1

        while (1) {
            lcd_clear_display(&display); // Limpa resquícios da volta anterior
            
            /// --- Linha 1: Título e Ícone ---
            lcd_set_cursor(&display, 0, 0);
            lcd_write_char(&display, 0); //Exibe o desenho da bateria (índice 0)
            lcd_write_string(&display, " CARREGANDO...");

            // --- Linha 2: Animação de Barra de Progresso ---
            for (uint8_t i = 0; i < 16; i++) {
                lcd_set_cursor(&display, i, 1); // Move para cada coluna da linha 2
                lcd_write_char(&display, 1); // Desenha o bloco (índice 1)
                _delay_ms(150); // Controla a cadência da animação
            }

            _delay_ms(1000); // Pausa final antes de reiniciar o loop
        }
    }

    return 0;
}

/* --------------------------------------------------------------------------
 * FUNÇÕES DE PONTE (BRIDGE/WRAPPER)
 * Conectam a lógica agnóstica da biblioteca ao hardware específico do AVR.
 * -------------------------------------------------------------------------- */

/**
 * @brief Callback de interrupção do driver TWI.
 * Chamado automaticamente quando uma transmissão I2C é finalizada.
 */
void on_twi_complete(TWI_Status_t status, const uint8_t* data, size_t length) {
    twi_busy = 0;
}

/**
 * @brief Ponte I2C para a biblioteca LCD.
 * 
 * Implementa uma escrita bloqueante simples para garantir que os comandos 
 * do LCD sejam processados na ordem correta.
 * 
 * @param addr Endereço do PCF8574 no barramento.
 * @param data Byte a ser enviado.
 * @return 0 para sucesso, -1 para erro.
 */
int8_t my_i2c_write(uint8_t addr, uint8_t data) {
    twi_busy = 1;
    if (twi_master_start_write(addr, &data, 1) == TWI_OK) {
    // Espera ocupada até que o driver TWI libere o barramento
        uint16_t timeout = 50000;
        while(twi_busy && timeout--);
        return 0; // Sucesso para a biblioteca
    }
    twi_busy = 0;
    return -1; // Erro para a biblioteca
}

/**
 * @brief Ponte de Tempo (Delay).
 * 
 * Converte a função macro util/delay.h em uma função de tempo dinâmico 
 * aceitável pela estrutura lcd_t.
 * 
 * @param ms Tempo de espera em milissegundos.
 */
void my_delay_ms(uint16_t ms) {
    while(ms--) _delay_ms(1);
}
