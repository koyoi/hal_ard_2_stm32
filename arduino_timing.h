#ifndef ARDUINO_TIMING_H
#define ARDUINO_TIMING_H  

#include "arduino_compat_common.h"
#include "main.h"
#include <stddef.h>
#include <stdint.h>

uint32_t millis(void);
uint32_t micros(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);

#endif // ARDUINO_TIMING_H