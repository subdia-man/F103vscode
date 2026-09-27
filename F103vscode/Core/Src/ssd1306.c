
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_spi.h"

#define SSD1306_RES_PIN GPIO_PIN_0
#define SSD1306_DC_PIN GPIO_PIN_1
#define SSD1306_CS_PIN GPIO_PIN_3
#define SSD1306_PORT GPIOB

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

extern SPI_HandleTypeDef hspi1;

void SSD1306_SendCommand(uint8_t command) {
    HAL_GPIO_WritePin(SSD1306_PORT, SSD1306_DC_PIN, GPIO_PIN_RESET); // Set DC low for command
    HAL_GPIO_WritePin(SSD1306_PORT, SSD1306_CS_PIN, GPIO_PIN_RESET); // Set CS low to select the device
    HAL_SPI_Transmit(&hspi1, &command, 1, HAL_MAX_DELAY); // Transmit command
    HAL_GPIO_WritePin(SSD1306_PORT, SSD1306_CS_PIN, GPIO_PIN_SET); // Set CS high to deselect the device
    return;
}

void SSD1306_SendData(uint8_t* data, size_t size) {
    HAL_GPIO_WritePin(SSD1306_PORT, SSD1306_DC_PIN, GPIO_PIN_SET); // Set DC high for data
    HAL_GPIO_WritePin(SSD1306_PORT, SSD1306_CS_PIN, GPIO_PIN_RESET); // Set CS low to select the device
    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY); // Transmit data
    HAL_GPIO_WritePin(SSD1306_PORT, SSD1306_CS_PIN, GPIO_PIN_SET); // Set CS high to deselect the device
    return;
}

void SSD1306_Init(void) {
    // Reset the display
    HAL_GPIO_WritePin(SSD1306_PORT, SSD1306_RES_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(SSD1306_PORT, SSD1306_RES_PIN, GPIO_PIN_SET);
    HAL_Delay(10);

    // Initialization sequence for SSD1306
    SSD1306_SendCommand(0xAE); // Display off
    SSD1306_SendCommand(0xD5); // Set display clock divide ratio/oscillator frequency
    SSD1306_SendCommand(0x80); // Suggested value
    SSD1306_SendCommand(0xA8); // Set multiplex ratio
    SSD1306_SendCommand(0x3F); // 1/64 duty
    SSD1306_SendCommand(0xD3); // Set display offset
    SSD1306_SendCommand(0x00); // No offset
    SSD1306_SendCommand(0x40); // Set start line address
    SSD1306_SendCommand(0x8D); // Charge pump setting
    SSD1306_SendCommand(0x14); // Enable charge pump
    SSD1306_SendCommand(0x20); // Memory addressing mode
    SSD1306_SendCommand(0x00); // Horizontal addressing mode
    SSD1306_SendCommand(0xA1); // Set segment re-map
    SSD1306_SendCommand(0xC8); // Set COM output scan direction
    SSD1306_SendCommand(0xDA); // Set COM pins hardware configuration
    SSD1306_SendCommand(0x12); // Set COM pins hardware configuration
    SSD1306_SendCommand(0x81); // Set contrast control
    SSD1306_SendCommand(0xCF); // Set contrast control
    SSD1306_SendCommand(0xD9); // Set pre-charge period
    SSD1306_SendCommand(0xF1); // Set pre-charge period
    SSD1306_SendCommand(0xDB); // Set VCOMH deselect level
    SSD1306_SendCommand(0x40); // Set VCOMH deselect level
    SSD1306_SendCommand(0xA4); // Entire display ON
    SSD1306_SendCommand(0xA6); // Set normal display
    SSD1306_SendCommand(0xAF); // Display ON
    return;
}