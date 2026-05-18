/**
 * @file lcd_i2c_types.h
 * @brief Definições de tipos, estruturas e enumerações da biblioteca LCD.
 * @author Tiago Henrique dos Santos/Ciência Elétrica
 * @date 2026
 * 
 * Este arquivo contém as definições essenciais para a abstração de hardware,
 * incluindo o handle principal do objeto LCD e ponteiros de função para
 * integração com diferentes drivers I2C.
 */
#ifndef LCD_I2C_TYPES_H_
#define LCD_I2C_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Status de retorno das funções da biblioteca.
 * 
 * Utilizado para verificar o sucesso ou falha de operações de 
 * inicialização e comunicação.
 */
typedef enum {
    LCD_OK = 0,    /**< Operação concluída com sucesso */
    LCD_ERROR,     /**< Falha genérica na operação */
    LCD_BUSY,      /**< O dispositivo ou barramento está ocupado */
    LCD_TIMEOUT    /**< Tempo de espera excedido durante a comunicação */
} lcd_status_t;

/**
 * @brief Estados possíveis para o controle da luz de fundo (Backlight).
 * 
 * O valor associado corresponde ao bit de controle enviado ao PCF8574.
 */
typedef enum {
    LCD_BACKLIGHT_OFF = 0,    /**< Backlight desligado */
    LCD_BACKLIGHT_ON  = 0x08  /**< Backlight ligado (Bit 3 do PCF8574) */
} lcd_backlight_t;

/**
 * @brief Definição da assinatura da função de escrita I2C (Ponteiro de Função).
 * 
 * O usuário deve fornecer uma implementação que realize a escrita de um único 
 * byte no barramento I2C para o endereço especificado.
 * 
 * @param slave_addr Endereço I2C do dispositivo escravo (PCF8574).
 * @param data Byte de dado a ser transmitido.
 * @return int8_t Deve retornar 0 para sucesso ou um valor negativo para erro.
 */
typedef int8_t (*lcd_i2c_write_ptr)(uint8_t slave_addr, uint8_t data);

/**
 * @brief Estrutura de controle (Handle) do objeto LCD.
 * 
 * Armazena o estado atual do display, configurações de geometria e 
 * os ponteiros para as funções de abstração de hardware (I2C e Delay).
 */
typedef struct {
    uint8_t address;            /**< Endereço I2C configurado (Ex: 0x27 ou 0x3F) */
    uint8_t columns;            /**< Número de colunas do display (Ex: 16 ou 20) */
    uint8_t rows;               /**< Número de linhas do display (Ex: 2 ou 4) */
    uint8_t entry_mode;         /**< Configuração atual de direção do cursor/shift */
    uint8_t display_ctrl;       /**< Estado atual de ativação do display, cursor e blink */
    lcd_backlight_t backlight;  /**< Estado atual do backlight (LCD_BACKLIGHT_ON/OFF) */
    
    /** 
     * @brief Ponteiro para a função de escrita I2C da HAL.
     * Deve ser preenchido pelo usuário antes da inicialização.
     */
    lcd_i2c_write_ptr i2c_write; 
    
    /** 
     * @brief Ponteiro para função de atraso em milissegundos.
     * Utilizado para garantir a temporização correta durante o Power-on Reset.
     */
    void (*delay_ms)(uint16_t ms);
} lcd_t;

#endif /* LCD_I2C_TYPES_H_ */