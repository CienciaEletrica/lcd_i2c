/**
 * @file lcd_i2c.h
 * @brief API pública para controle de displays LCD via interface I2C (PCF8574).
 * @author Tiago Henrique dos Santos/Ciência Elétrica
 * @date 2026
 * 
 * Este arquivo define os protótipos das funções utilizadas para inicializar,
 * configurar, escrever e gerenciar um display LCD de caracteres através do
 * barramento I2C/TWI em microcontroladores AVR.
 */
#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include "lcd_i2c_types.h"
#include "lcd_i2c_config.h"

/**
 * @brief Inicializa o display LCD via interface I2C (PCF8574).
 * 
 * Configura o barramento no modo de 4 bits, executa a sequência de reset
 * por software recomendada pelo fabricante, limpa o display e define o modo 
 * de entrada padrão (incremento automático do cursor).
 * 
 * @param handle Ponteiro para a estrutura de controle do LCD.
 * @return lcd_status_t Status da operação (LCD_OK ou LCD_ERROR).
 */
lcd_status_t lcd_init(lcd_t *handle);

/**
 * @brief Envia um comando de instrução para o controlador do LCD (RS=0).
 * 
 * Utilizado para configurações de baixo nível no controlador HD44780.
 * 
 * @param handle Ponteiro para a estrutura do LCD.
 * @param command Byte de comando (ex: LCD_CMD_CLEAR_DISPLAY).
 */
void lcd_send_command(lcd_t *handle, uint8_t command);

/**
 * @brief Escreve um único caractere na posição atual do cursor (RS=1).
 * 
 * @param handle Ponteiro para a estrutura do LCD.
 * @param data Caractere ASCII a ser exibido.
 */
void lcd_write_char(lcd_t *handle, char data);

/**
 * @brief Escreve uma string de caracteres a partir da posição atual do cursor.
 * 
 * @param handle Ponteiro para a estrutura do LCD.
 * @param str Ponteiro para a string (terminada em caractere nulo - '\0').
 */
void lcd_write_string(lcd_t *handle, const char *str);

/**
 * @brief Posiciona o cursor em uma coordenada específica (coluna, linha).
 * 
 * Altera o endereço de memória DDRAM para reposicionar a próxima escrita.
 * 
 * @param handle Ponteiro para a estrutura do LCD.
 * @param col Coluna destino (0 a N-1).
 * @param row Linha destino (0 a N-1).
 */
void lcd_set_cursor(lcd_t *handle, uint8_t col, uint8_t row);

/**
 * @brief Limpa todo o conteúdo do display e retorna o cursor para a posição (0,0).
 * 
 * @param handle Ponteiro para a estrutura do LCD.
 * @note Este comando necessita de um tempo de processamento maior (mínimo de 2ms).
 */
void lcd_clear_display(lcd_t *handle);

/**
 * @brief Liga ou desliga o backlight (luz de fundo) sem alterar o texto atual.
 * 
 * @param handle Ponteiro para a estrutura do LCD.
 * @param state Estado desejado (LCD_BACKLIGHT_ON ou LCD_BACKLIGHT_OFF).
 */
void lcd_backlight(lcd_t *handle, lcd_backlight_t state);

/**
 * @brief Controla o estado de exibição do cursor e o efeito de piscar (blink).
 * 
 * @param handle Ponteiro para a estrutura do LCD.
 * @param cursor_on Define se a linha inferior (sublinhado) do cursor aparece.
 * @param blink_on Define se o bloco inteiro do cursor fica piscando.
 */
void lcd_display_control(lcd_t *handle, bool cursor_on, bool blink_on);

/**
 * @brief Cria um caractere personalizado na memória CGRAM do LCD.
 * 
 * Permite criar e armazenar até 8 ícones customizados mapeados nos
 * índices de memória de 0 a 7.
 * 
 * @param handle Ponteiro para a estrutura do LCD.
 * @param location Índice da memória onde o caractere será salvo (valores válidos: 0 a 7).
 * @param charmap Array de 8 bytes representando a matriz de pixels 5x8 do caractere.
 */
void lcd_create_custom_char(lcd_t *handle, uint8_t location, uint8_t charmap[]);

#endif /* LCD_I2C_H_ */