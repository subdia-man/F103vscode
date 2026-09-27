
#ifdef __cplusplus
extern "C" {
#endif

void SSD1306_SendCommand(uint8_t command);
void SSD1306_SendData(uint8_t* data, size_t size);
void SSD1306_Init(void);

#ifdef __cplusplus
}
#endif