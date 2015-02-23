#include "lpc_types.h"
#include "lpc17xx_i2c.h"

int first_line_scroll_i;
int second_line_scroll_i;
I2C_M_SETUP_Type I2CConfigStruct;

void scroll_first_line(I2C_M_SETUP_Type * i2c_config, char *line_text, int size);
void scroll_second_line(I2C_M_SETUP_Type * i2c_config, char *line_text, int size);
void write_first_line(I2C_M_SETUP_Type * i2c_config, char *line_text, int size);
void write_second_line(I2C_M_SETUP_Type * i2c_config, char *line_text, int size);
void init_i2c(void);
void lcd_write_bytes(I2C_M_SETUP_Type * i2c_config, uint8_t bytes[], int length);
void isBusyWait(LPC_I2C_TypeDef* i2cPort, uint8_t addr);
