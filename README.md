# AVR LCD I2C Library (PCF8574)

![License](https://img.shields.io/badge/license-MIT-green)
![AVR](https://img.shields.io/badge/MCU-AVR-blue)
![Language](https://img.shields.io/badge/language-C-darkblue)
![Compiler](https://img.shields.io/badge/compiler-XC8-orange)

Uma biblioteca C leve, robusta e extensível para controlar displays LCD (HD44780) via expansor de I/O I2C PCF8574 em microcontroladores AVR (ATmega328P, ATmega2560, etc.).

## Características

*   **Abstração de Hardware (HAL):** Injeção de dependência para drivers I2C e funções de Delay.
*   **Modo 4-bits:** Comunicação otimizada via PCF8574.
*   **Custom Chars:** Suporte para criação de até 8 caracteres personalizados na CGRAM.
*   **Documentação Completa:** Compatível com Doxygen.
*   **Baixo Consumo:** Gerenciamento eficiente de Backlight.

## Requisitos

*   **Compilador:** XC8
*   **Hardware:** Microcontrolador AVR e Adaptador LCD I2C (PCF8574)

## Como Usar

### 1. Configuração do Objeto
Defina uma instância da estrutura `lcd_t` e forneça as funções de ponte para I2C e tempo:

```c
#include "lcd_i2c.h"

// Exemplo de função de escrita I2C
int8_t i2c_wrapper(uint8_t addr, uint8_t data) {
    return (twi_master_write(addr, &data, 1) == TWI_OK) ? 0 : -1;
}

// Exemplo de função de delay
void delay_wrapper(uint16_t ms) {
    while(ms--) _delay_ms(1);
}

int main() {
    lcd_t lcd = {
        .address = 0x27,
        .columns = 16,
        .rows = 2,
        .backlight = LCD_BACKLIGHT_ON,
        .i2c_write = i2c_wrapper,
        .delay_ms = delay_wrapper
    };

    if (lcd_init(&lcd) == LCD_OK) {
        lcd_write_string(&lcd, "Ola, AVR!");
    }
}
```

### 2. Exemplo Incluso

1.  **Exemplo Completo:** @ref example.c "Este exemplo demonstra o fluxo completo de uso da biblioteca."

Isso gerará a documentação completa em HTML na pasta `doc/`.

## Estrutura do Projeto

*   `lcd_i2c.h/c`: API principal.
*   `lcd_i2c_types.h`: Definições de estruturas e enums.
*   `lcd_i2c_config.h`: Configurações de hardware e timings.
*   `lcd_i2c_internal.h`: Funções de baixo nível.

## Licença

Distribuído sob a licença MIT. Veja `LICENSE` para mais informações.
