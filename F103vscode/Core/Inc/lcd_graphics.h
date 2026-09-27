/*
 * lcd_graphics.h
 *
 *  Created on: Sep 18, 2026
 *      Author: subdia
 */

#ifndef INC_LCD_GRAPHICS_H_
#define INC_LCD_GRAPHICS_H_

void fill_out_whole_screen(void);
void fill_out_screen_data_with_zeroes(void);
void fill_out_screen_data_with_ff(void);
void prepare_screen_data(uint8_t* data, size_t size);
void out_data_to_the_screen(uint8_t* data, size_t size);
void to_my_wife(void);

#endif /* INC_LCD_GRAPHICS_H_ */
