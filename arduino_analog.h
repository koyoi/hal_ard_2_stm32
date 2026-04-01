#ifndef ARDUINO_ANALOG_H
#define ARDUINO_ANALOG_H

#include "arduino_compat_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void Arduino_AttachADCHandle(ADC_HandleTypeDef *handle);
int analogRead(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif // ARDUINO_ANALOG_H
