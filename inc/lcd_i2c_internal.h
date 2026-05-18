/**
 * @file lcd_i2c_internal.h
 * @brief Funções internas e de baixo nível para comunicação com o PCF8574.
 * @author Tiago Henrique dos Santos/Ciência Elétrica
 * @date 2026
 * 
 * Este arquivo contém as declarações das funções de suporte que gerenciam 
 * a comunicação de baixo nível, como o envio de nibbles e a geração de 
 * pulsos de clock (Enable) para o controlador HD44780.
 */
#ifndef LCD_I2C_INTERNAL_H_
#define LCD_I2C_INTERNAL_H_

#include "lcd_i2c_types.h"

/**
 * @brief Gera o pulso de "Enable" (transição High -> Low) no pino EN.
 * 
 * Essencial para que o controlador do LCD capture os dados presentes nos 
 * pinos de dados (D4-D7). Implementa os delays necessários para o setup time.
 * 
 * @param handle Ponteiro para a estrutura de configuração do LCD.
 * @param data O byte atual que está sendo enviado (com o bit EN em nível alto).
 */
void lcd_pcf8574_pulse_enable(lcd_t *handle, uint8_t data);

/**
 * @brief Envia 4 bits (nibble) para o LCD através do expansor I2C.
 * 
 * Como a interface com o LCD é de 4 bits, esta função prepara o byte 
 * combinando os dados, o estado do pino RS e o estado do Backlight.
 * 
 * @param handle Ponteiro para a estrutura de configuração do LCD.
 * @param nibble Os 4 bits de dados (devem estar alinhados nos bits superiores D4-D7).
 * @param mode Define o propósito do envio: 0 para Comando (RS=0) ou 1 para Dado (RS=1).
 */
void lcd_send_nibble(lcd_t *handle, uint8_t nibble, uint8_t mode);

/**
 * @brief Função utilitária para enviar 8 bits, dividindo-os em dois nibbles.
 * 
 * Realiza o fracionamento do byte em parte alta (Most Significant Nibble) 
 * e parte baixa (Least Significant Nibble) para transmissão sequencial.
 * 
 * @param handle Ponteiro para a estrutura de configuração do LCD.
 * @param value O byte completo de 8 bits a ser transmitido.
 * @param mode Define o estado do pino Register Select (RS).
 */
void lcd_send_8bit(lcd_t *handle, uint8_t value, uint8_t mode);

#endif /* LCD_I2C_INTERNAL_H_ */