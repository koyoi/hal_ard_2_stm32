#ifndef ARDUINO_PINMAP_H
#define ARDUINO_PINMAP_H

#include "main.h"
#include <stdint.h>

typedef uint32_t PinName;

#define NC (0xFFFFFFFFu)
#ifdef __cplusplus
struct ArduinoNotAPin {
  constexpr operator uint32_t() const { return 0xFFFFFFFEu; }
  template<typename T>
  constexpr operator T *() const { return nullptr; }
};
static constexpr ArduinoNotAPin NP{};
#else
#define NP (0xFFFFFFFEu)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ADC_RESOLUTION
#define ADC_RESOLUTION 12
#endif

#ifndef PNUM_ANALOG_BASE
#define PNUM_ANALOG_BASE 0xC0
#endif

#ifndef NUM_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS 8
#endif

#define A0 (PNUM_ANALOG_BASE + 0)
#define A1 (PNUM_ANALOG_BASE + 1)
#define A2 (PNUM_ANALOG_BASE + 2)
#define A3 (PNUM_ANALOG_BASE + 3)
#define A4 (PNUM_ANALOG_BASE + 4)
#define A5 (PNUM_ANALOG_BASE + 5)
#define A6 (PNUM_ANALOG_BASE + 6)
#define A7 (PNUM_ANALOG_BASE + 7)

#define PADC_BASE       (0x80000000u)
#define ANA_START       (0x7F000000u)
#define PADC_TEMP_ADC5  (PADC_BASE | 0x5u)

extern const void *PinMap_TIM;
extern const void *PinMap_ADC;

PinName digitalPinToPinName(uint32_t pin);
PinName analogInputToPinName(uint32_t pin);
GPIO_TypeDef *Arduino_PinNameToPort(PinName pin);
uint16_t Arduino_PinNameToMask(PinName pin);
uint32_t pinmap_function(PinName pin, const void *map);
uint32_t pinmap_peripheral(PinName pin, const void *map);
uint32_t STM_PIN_CHANNEL(uint32_t encoded);
uint32_t get_adc_channel(PinName pin, uint32_t *bank);
uint32_t get_adc_internal_channel(PinName pin);

void Arduino_PinMap_RegisterTimer(PinName pin, TIM_TypeDef *instance, uint32_t channel);
void Arduino_PinMap_RegisterAdc(PinName pin, ADC_TypeDef *instance, uint32_t channel, uint32_t bank);
void Arduino_PinMap_Clear(void);

#ifdef __cplusplus
}
#endif

#include "arduino_pin_defs.h"

#endif // ARDUINO_PINMAP_H
