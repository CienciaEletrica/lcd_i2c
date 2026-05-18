/**
 * @file lcd_i2c.c
 * @brief Implementação da biblioteca para controle de LCD HD44780 via I2C/TWI.
 * @author Tiago Henrique dos Santos/Ciência Elétrica
 * @date 2026
 * 
 * Esta biblioteca utiliza o expansor de I/O PCF8574 para interfacear microcontroladores 
 * AVR com displays LCD de texto usando apenas 2 pinos (SDA/SCL).
 */
#include "lcd_i2c.h"
#include "lcd_i2c_internal.h"
#include <util/delay.h>

/**
 * @note Utiliza tempos de subida (2us) e descida (40us) para garantir a estabilidade.
 */
void lcd_pcf8574_pulse_enable(lcd_t *handle, uint8_t data) {
    // Sobe o pino EN mantendo o Backlight e RS/RW
    handle->i2c_write(handle->address, data | LCD_PIN_EN);
    _delay_us(2); 
    
    // Desce o pino EN (Transição de captura do HD44780)
    handle->i2c_write(handle->address, data & ~LCD_PIN_EN);
    _delay_us(40); 
}

void lcd_send_nibble(lcd_t *handle, uint8_t nibble, uint8_t mode) {
    // Define RS: 1 para dados, 0 para comandos
    uint8_t rs_bit = (mode) ? LCD_PIN_RS : 0;
    
    // Monta o byte: Parte alta (D4-D7) | RS | Backlight
    uint8_t data = (nibble & 0xF0) | rs_bit | (uint8_t)handle->backlight;
    
    // Executa o ciclo de escrita com pulso de Enable
    lcd_pcf8574_pulse_enable(handle, data);
}

void lcd_send_8bit(lcd_t *handle, uint8_t value, uint8_t mode) {
    lcd_send_nibble(handle, value & 0xF0, mode);        // Envia parte alta
    lcd_send_nibble(handle, (value << 4) & 0xF0, mode); // Envia parte baixa
}

/* --- API PÚBLICA --- */

/**
 * @return lcd_status_t Status da operação (LCD_OK em caso de sucesso).
 */
lcd_status_t lcd_init(lcd_t *handle) {
    _delay_ms(50); // Aguarda estabilização do VCC

    // Sequência de Reset de Software (conforme datasheet HD44780)
    lcd_send_nibble(handle, 0x30, 0); 
    _delay_ms(5);
    
    lcd_send_nibble(handle, 0x30, 0);
    _delay_us(200);
    
    lcd_send_nibble(handle, 0x30, 0);
    _delay_ms(1);

    // Define interface de 4 bits
    lcd_send_nibble(handle, 0x20, 0); 
    _delay_ms(1);

    // Configuração inicial: 2 linhas, 5x8, Display ON, Cursor OFF
    lcd_send_command(handle, 0x28); // 2 linhas, matriz 5x8
    lcd_send_command(handle, 0x0C); // Display ON, Cursor OFF
    lcd_clear_display(handle);
    lcd_send_command(handle, 0x06); // Incremento automático do cursor

    return LCD_OK;
}

void lcd_send_command(lcd_t *handle, uint8_t command) {
    lcd_send_8bit(handle, command, 0); // RS = 0
}

void lcd_write_char(lcd_t *handle, char data) {
    lcd_send_8bit(handle, (uint8_t)data, 1); // RS = 1
}

void lcd_write_string(lcd_t *handle, const char *str) {
    while (*str) lcd_write_char(handle, *str++);
}

void lcd_set_cursor(lcd_t *handle, uint8_t col, uint8_t row) {
    uint8_t addr = (row == 0) ? 0x80 + col : 0xC0 + col;
    lcd_send_command(handle, addr);
}

void lcd_clear_display(lcd_t *handle) {
    lcd_send_command(handle, 0x01);
    _delay_ms(2); // Comando Clear exige delay maior
}

void lcd_backlight(lcd_t *handle, lcd_backlight_t state) {
    handle->backlight = state;
    // Atualiza o estado no expansor sem alterar o conteúdo do LCD
    handle->i2c_write(handle->address, (uint8_t)handle->backlight);
}

void lcd_display_control(lcd_t *handle, bool cursor_on, bool blink_on) {
    handle->display_ctrl = 0x0C; // Base: Display ON
    if (cursor_on) handle->display_ctrl |= 0x02;
    if (blink_on)  handle->display_ctrl |= 0x01;
    lcd_send_command(handle, handle->display_ctrl);
}

void lcd_create_custom_char(lcd_t *handle, uint8_t location, uint8_t charmap[]) {
    location &= 0x07; // Limita a 8 posições (0-7)
    // Aponta para o endereço inicial da CGRAM para aquela posição
    lcd_send_command(handle, 0x40 | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        lcd_send_8bit(handle, charmap[i], 1); // Grava os 8 bytes da matriz
    }
    // Retorna o endereçamento para DDRAM (Memória de Vídeo)
    lcd_send_command(handle, 0x80);
}