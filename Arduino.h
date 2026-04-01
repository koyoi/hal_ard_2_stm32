#pragma once
/*
 * Arduino.h
 *
 * Minimal Arduino compatibility layer for STM32Cube projects.
 */

#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "arduino_compat_common.h"
#include "arduino_gpio.h"
#include "arduino_analog.h"

#ifndef ARDUINO
#define ARDUINO 10819
#endif

#ifndef ARDUINO_ARCH_STM32
#define ARDUINO_ARCH_STM32 1
#endif

#ifndef F_CPU
#define F_CPU (SystemCoreClock)
#endif

// Arduino headers usually typedef `byte` as `uint8_t`. FixMath expects this
// alias, so provide it here when compiling without Arduino.
#ifndef byte
typedef uint8_t byte;
#endif

#ifndef min
#define min(a, b) (( (a) < (b) ) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) (( (a) > (b) ) ? (a) : (b))
#endif

#ifndef constrain
#define constrain(amt, low, high) (( (amt) < (low) ) ? (low) : ( (amt) > (high) ? (high) : (amt) ))
#endif

#ifdef __cplusplus
extern "C" {
#endif

uint32_t millis(void);
uint32_t micros(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
void Arduino_AttachADCHandle(ADC_HandleTypeDef *handle);

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_H_ */
