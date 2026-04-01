#ifndef ARDUINO_COMPAT_COMMON_H
#define ARDUINO_COMPAT_COMMON_H

#include <stdint.h>
#include "arduino_pinmap.h"

#if defined(STM32F411xE) && !defined(STM32F4xx)
#define STM32F4xx
#endif

// Arduino headers usually typedef `byte` as `uint8_t`. FixMath expects this
// alias, so provide it here when compiling without Arduino.
#ifndef byte
typedef uint8_t byte;
#endif

// Arduino-like constants
#ifndef HIGH
#define HIGH 0x1U
#endif

#ifndef LOW
#define LOW 0x0U
#endif

#ifndef INPUT
#define INPUT 0x0U
#endif

#ifndef OUTPUT
#define OUTPUT 0x1U
#endif

#ifndef INPUT_PULLUP
#define INPUT_PULLUP 0x2U
#endif

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN 0x3U
#endif

#ifndef LSBFIRST
#define LSBFIRST 0U
#endif

#ifndef MSBFIRST
#define MSBFIRST 1U
#endif

#ifndef SPI_MODE0
#define SPI_MODE0 0x00U
#endif

#ifndef SPI_MODE1
#define SPI_MODE1 0x04U
#endif

#ifndef SPI_MODE2
#define SPI_MODE2 0x08U
#endif

#ifndef SPI_MODE3
#define SPI_MODE3 0x0CU
#endif

#endif // ARDUINO_COMPAT_COMMON_H
