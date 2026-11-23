#ifndef FONT8X16_HPP
#define FONT8X16_HPP

#include <cstdint>

// Размеры шрифта
constexpr uint8_t FONT8X16_WIDTH = 8;
constexpr uint8_t FONT8X16_HEIGHT = 16;

// Диапазоны символов
constexpr uint16_t FONT8X16_ASCII_START = 0x20;  // Пробел
constexpr uint16_t FONT8X16_ASCII_END = 0x7F;    // DEL
constexpr uint16_t FONT8X16_CYRILLIC_START = 0x0410; // А
constexpr uint16_t FONT8X16_CYRILLIC_END = 0x044F;   // я

// Функции для работы со шрифтом
const uint8_t* Font8x16_GetChar(uint16_t unicode_char);
const uint8_t* Font8x16_GetCharUTF8(const char* utf8_char, uint8_t* bytes_consumed);

// Утилиты для UTF-8
uint16_t UTF8_ToUnicode(const char* utf8_char, uint8_t* bytes_consumed);
bool UTF8_IsMultibyte(uint8_t first_byte);

#endif