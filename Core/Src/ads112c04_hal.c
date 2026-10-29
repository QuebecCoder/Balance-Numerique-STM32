#include "ads112c04_api.h"
#include "ads112c04_hal.h"
#include "ads112c04_core_defines.h"
#include "ssd1306.h"
extern I2C_HandleTypeDef hi2c2;

void i2c_init (void){
	//alreasy init dans le main dans static void MX_I2C2_Init(void)
}

void i2c_read (uint8_t address, uint8_t *rxBuffer, uint8_t size){
	HAL_I2C_Master_Receive(&hi2c2, address,rxBuffer,size, 100);
}

void i2c_write (uint8_t address, uint8_t *txBuffer, uint8_t size){
	HAL_I2C_Master_Transmit(&hi2c2, address, txBuffer,size, 100);
}

void delay_ms(uint32_t milliseconds){
    HAL_Delay(milliseconds);
}
