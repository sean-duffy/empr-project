#include <string.h>
#include <stdio.h>
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "../I2C/i2c.h"

int debu = 1;
#define debug_print(n, x) if(debu) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debu) { write_serial(n, x); }

int first_line_scroll_i = 0;
int second_line_scroll_i = 0;
I2C_M_SETUP_Type I2CConfigStruct;

int map_char(char c) {
    if (c == '#') {
        return 78;
    } else if (c == 19) {
        return ' ';
    } else if (c == '_') {
        return 9;
    } else if (c < 91) {
        return c + 128;
    } else {
        return c;
    }
}

uint8_t readBACRegister(LPC_I2C_TypeDef* i2cPort, uint8_t addr) {
    uint8_t result[1];
    i2cRead(i2cPort, addr, NULL, 0, result, 1);
    return result[0];
}

void isBusyWait(LPC_I2C_TypeDef* i2cPort, uint8_t addr) {
    while(readBACRegister(i2cPort, addr) & 0x80);
}

void init_i2c(void) {
    PINSEL_CFG_Type PinCfg; // declare data struct with param members
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Funcnum = 3;
    PinCfg.Pinnum = 0;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg); // configure pin 0 of port0
    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg); // configure pin 1 of port0
    I2C_Init(LPC_I2C1, 100000); // Initialize I2C peripheral
    I2C_Cmd(LPC_I2C1, ENABLE); // Enable I2C1 operation 
}

void lcd_write_bytes(I2C_M_SETUP_Type * i2c_config, uint8_t bytes[], int length) {
    i2c_config->tx_data = bytes;
    i2c_config->tx_length = length;
    I2C_MasterTransferData(LPC_I2C1, i2c_config, I2C_TRANSFER_POLLING);
    isBusyWait(LPC_I2C1, I2CConfigStruct.sl_addr7bit);
}

void lcd_write_message(I2C_M_SETUP_Type * i2c_config, char message[], int length) {
    uint8_t data_write[17] = { [0 ... 16] = 0xA0};
    int i, j = 0;
    int c;

    data_write[0] = 0x40;
    for (i = 1; i < length+1; i++) {
        c = map_char(message[i-1]);
        if (c < 122 || c > 154) {
            data_write[i-j] = c;
        } else {
            j++;
        }
    }

    lcd_write_bytes(i2c_config, data_write, sizeof(data_write));
}

void write_first_line(I2C_M_SETUP_Type * i2c_config, char *line_text, int size) {
    uint8_t addr_write[] = {0x00, 0x02};
    lcd_write_bytes(i2c_config, addr_write, sizeof(addr_write));
    lcd_write_message(i2c_config, line_text, size);
}

void write_second_line(I2C_M_SETUP_Type * i2c_config, char *line_text, int size) {
    uint8_t addr_write[] = {0x00, 0xC0};
    lcd_write_bytes(i2c_config, addr_write, sizeof(addr_write));
    lcd_write_message(i2c_config, line_text, size); 
}

void scroll_second_line(I2C_M_SETUP_Type * i2c_config, char *line_text, int size) {
    if (second_line_scroll_i >= size) {
        second_line_scroll_i = 0;
    }

    write_second_line(i2c_config, &line_text[second_line_scroll_i], size-second_line_scroll_i);
    second_line_scroll_i++;
}

void scroll_first_line(I2C_M_SETUP_Type * i2c_config, char *line_text, int size) {
    if (first_line_scroll_i >= size) {
        first_line_scroll_i = 0;
    }

    write_first_line(i2c_config, &line_text[first_line_scroll_i], size-first_line_scroll_i);
    first_line_scroll_i++;
}
