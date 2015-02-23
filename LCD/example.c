#include <string.h>

#include "LPC17xx.h"
#include "lcd.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"

int main(void) {
    init_i2c();

    I2CConfigStruct.retransmissions_max = 3;
    I2CConfigStruct.sl_addr7bit = 59;

    uint8_t lcd_init[] = {0x00, 0x35, 0x9F, 0x34, 0x0C, 0x02};
    lcd_write_bytes(&I2CConfigStruct, lcd_init, sizeof(lcd_init));

    uint8_t clear[] = {0x00, 0x01};
    lcd_write_bytes(&I2CConfigStruct, clear, sizeof(clear));

    isBusyWait(LPC_I2C1, I2CConfigStruct.sl_addr7bit);

    char message_1[] = "Hello World";
    write_first_line(&I2CConfigStruct, message_1, strlen(message_1));

    char message_2[] = "abcdefghijklmnopqrstuvwxyz";

    while (1) {
        while (duration_passed != 3);
        duration_passed = 0;
        
        scroll_second_line(&I2CConfigStruct, message_2, strlen(message_2));
    }

    return 0;
}
