#ifndef ARDUINO_GPIO_H
#define ARDUINO_GPIO_H

#include "arduino_compat_common.h"
#include "arduino_pinmap.h"

#ifdef __cplusplus
extern "C" {
#endif

void pinMode(uint32_t pin, uint8_t mode);
void digitalWrite(uint32_t pin, uint8_t value);
int digitalRead(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif // ARDUINO_GPIO_H
