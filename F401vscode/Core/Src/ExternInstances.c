
#include <stdint.h>
#include <stdbool.h>

uint8_t cdc_global_rx_buffer[68];
uint32_t cdc_global_rx_length = 0;
bool cdc_is_something_received = false;

void cdc_is_something_received_flag_set();
void cdc_is_something_received_flag_reset();
bool is_cdc_something_received_flag_set();

void cdc_is_something_received_flag_set() {
  cdc_is_something_received = true;
  return;
}

void cdc_is_something_received_flag_reset() {
  cdc_is_something_received = false;
  return;
}

bool is_cdc_something_received_flag_set() {
	return cdc_is_something_received;
}