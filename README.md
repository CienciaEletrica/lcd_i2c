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
#include "twi.h"
#include "lcd_i2c.h"
// 1. Criamos as pontes de hardware
int8_t bridge_i2c(uint8_t addr, uint8_t data) {
    return (twi_write_byte(addr, data) == TWI_OK) ? 0 : -1;
}
void bridge_delay(uint16_t ms) {
    while(ms--) _delay_ms(1);
}
int main() {
    // 2. Criamos e configuramos o objeto (Handle)
    lcd_t lcd1;
    lcd1.address = 0x27;
    lcd1.columns = 16;
    lcd1.rows = 2;
    lcd1.backlight = LCD_BACKLIGHT_ON;
    lcd1.i2c_write = bridge_i2c; // Injeção da função I2C
    lcd1.delay_ms = bridge_delay; // Injeção da função Delay
    // 3. Inicializamos
    if (lcd_init(&lcd1) == LCD_OK) {
        lcd_write_string(&lcd1, "Status: OK!");
    }
    while(1);
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
