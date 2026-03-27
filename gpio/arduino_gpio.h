#ifndef ARDUINO_GPIO_H
#define ARDUINO_GPIO_H

#include "../arduino_compat_common.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pinMask;
} ArduinoGpioPinMap;

void Arduino_GPIO_RegisterPin(uint16_t arduinoPin, GPIO_TypeDef *port, uint16_t pinMask);
void Arduino_GPIO_ClearPinMap(void);

void pinMode(uint16_t pin, uint8_t mode);
void digitalWrite(uint16_t pin, uint8_t value);
int digitalRead(uint16_t pin);

#ifdef __cplusplus
}
#endif

#endif // ARDUINO_GPIO_H
