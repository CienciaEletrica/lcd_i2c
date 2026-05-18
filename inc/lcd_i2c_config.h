/**
 * @file lcd_i2c_config.h
 * @brief Definições de hardware, mapeamento de pinos e comandos do LCD.
 * @author Tiago Henrique dos Santos/Ciência Elétrica
 * @date 2026
 * 
 * Este arquivo contém as constantes de configuração para o barramento I2C,
 * mapeamento físico entre o PCF8574 e o controlador HD44780, além dos
 * códigos de instrução do datasheet do LCD.
 */
#ifndef LCD_I2C_CONFIG_H_
#define LCD_I2C_CONFIG_H_

/* --- Configurações de Hardware Padrão --- */

#ifndef F_CPU
/** @brief Clock do sistema padrão (16 MHz) caso não definido pelo compilador/IDE. */
#define F_CPU 16000000UL
#endif

#ifndef LCD_DEFAULT_ADDR
/** @brief Endereço I2C padrão para a maioria dos módulos PCF8574 comerciais. */
#define LCD_DEFAULT_ADDR 0x27 
#endif

/* --- Mapeamento de Pinos do PCF8574 para o LCD --- */
/** 
 * @name Mapeamento de Pinos
 * Baseado no layout padrão dos adaptadores I2C "IIC/I2C/TWI Serial Interface".
 * @{ 
 */
#define LCD_PIN_RS      (1 << 0)  /**< Registro Select (P0) */
#define LCD_PIN_RW      (1 << 1)  /**< Read/Write (P1) */
#define LCD_PIN_EN      (1 << 2)  /**< Enable (P2) */
#define LCD_PIN_BL      (1 << 3)  /**< Backlight (P3) - Controlado via firmware */
/** @} */

/* --- Comandos do Controlador HD44780 --- */
/** 
 * @name Conjunto de Instruções
 * Comandos base do controlador HD44780.
 * @{ 
 */
#define LCD_CMD_CLEAR_DISPLAY   0x01 /**< Limpa display e retorna cursor ao início */
#define LCD_CMD_RETURN_HOME     0x02 /**< Retorna cursor ao início sem limpar dados */
#define LCD_CMD_ENTRY_MODE_SET  0x04 /**< Configura direção de movimento do cursor */
#define LCD_CMD_DISPLAY_CONTROL 0x08 /**< Liga/Desliga display, cursor e blink */
#define LCD_CMD_CURSOR_SHIFT    0x10 /**< Move cursor ou desloca o conteúdo exibido */
#define LCD_CMD_FUNCTION_SET    0x20 /**< Configura barramento (4-bits) e nº de linhas */
#define LCD_CMD_SET_CGRAM_ADDR  0x40 /**< Define endereço para memória de caracteres customizados */
#define LCD_CMD_SET_DDRAM_ADDR  0x80 /**< Define endereço para memória de exibição (vídeo) */
/** @} */

/* --- Flags de Configuração (Modos de Operação) --- */
/** 
 * @name Flags de Operação
 * Bits de configuração para serem combinados com os comandos base.
 * @{ 
 */
// Entry Mode Set
#define LCD_ENTRY_LEFT           0x02 /**< Incremento automático à esquerda */
#define LCD_ENTRY_SHIFT_DECREMENT 0x00 /**< Deslocamento de tela desligado */

// Display Control
#define LCD_DISPLAY_ON           0x04 /**< Liga a exibição dos caracteres */
#define LCD_CURSOR_ON            0x02 /**< Liga a linha de sublinhado do cursor */
#define LCD_BLINK_ON             0x01 /**< Liga o efeito de bloco piscante no cursor */
/** @} */

/* --- Delays Críticos (em microssegundos ou milissegundos) --- */
/** 
 * @name Temporização (Timing)
 * Tempos mínimos baseados no datasheet para garantir a execução dos comandos.
 * @{ 
 */
#define LCD_DELAY_INIT_MS        50   /**< Tempo de estabilização do VCC (Power-on) */
#define LCD_DELAY_COMMAND_US     100  /**< Tempo padrão para comandos simples */
#define LCD_DELAY_CLEAR_MS       2    /**< Tempo estendido para Clear e Return Home */
/** @} */

#endif /* LCD_I2C_CONFIG_H_ */