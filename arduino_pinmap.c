#include "arduino_pinmap.h"
#include <stddef.h>
#include <string.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef ARDUINO_PINMAP_MAX_ENTRIES
#define ARDUINO_PINMAP_MAX_ENTRIES 32
#endif

typedef struct {
  PinName pin;
  TIM_TypeDef *timer;
  uint32_t timerChannel;
  ADC_TypeDef *adc;
  uint32_t adcChannel;
  uint32_t adcBank;
} PinPeripheralEntry;

static PinPeripheralEntry s_entries[ARDUINO_PINMAP_MAX_ENTRIES];
static uint8_t s_initialized = 0;
static uint8_t s_defaults_registered = 0;

static const PinName kDigitalPinMap[] = {
    PA0,  PA1,  PA2,  PA3,  PA4,  PA5,  PA6,  PA7,
    PA8,  PA9,  PA10, PA11, PA12, PC13, PC14, PC15,
    PB0,  PB1,  PB2,  PB3,  PB4,  PB5,  PB6,  PB7,
    PB8,  PB9,  PB10, PB11, PB12, PB13, PB14, PB15};

static const PinName kAnalogPinMap[NUM_ANALOG_INPUTS] = {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
};

typedef struct {
  PinName pin;
  TIM_TypeDef *timer;
  uint32_t channel;
} TimerMapEntry;

typedef struct {
  PinName pin;
  ADC_TypeDef *adc;
  uint32_t channel;
  uint32_t bank;
} AdcMapEntry;

static const TimerMapEntry kTimerDefaults[] = {
    {PA8, TIM1, 1},
    {PA9, TIM1, 2},
};

static const AdcMapEntry kAdcDefaults[] = {
    {PA0, ADC1, ADC_CHANNEL_0, 0},
    {PA1, ADC1, ADC_CHANNEL_1, 0},
    {PA2, ADC1, ADC_CHANNEL_2, 0},
    {PA3, ADC1, ADC_CHANNEL_3, 0},
    {PA4, ADC1, ADC_CHANNEL_4, 0},
    {PA5, ADC1, ADC_CHANNEL_5, 0},
    {PA6, ADC1, ADC_CHANNEL_6, 0},
    {PA7, ADC1, ADC_CHANNEL_7, 0},
};

static void register_default_pinmaps(void) {
  if (s_defaults_registered) {
    return;
  }
  s_defaults_registered = 1;
  for (size_t i = 0; i < ARRAY_SIZE(kTimerDefaults); ++i) {
    Arduino_PinMap_RegisterTimer(kTimerDefaults[i].pin, kTimerDefaults[i].timer,
                                 kTimerDefaults[i].channel);
  }
  for (size_t i = 0; i < ARRAY_SIZE(kAdcDefaults); ++i) {
    Arduino_PinMap_RegisterAdc(kAdcDefaults[i].pin, kAdcDefaults[i].adc,
                               kAdcDefaults[i].channel, kAdcDefaults[i].bank);
  }
}

static PinPeripheralEntry *find_entry(PinName pin, uint8_t create) {
  if (!s_initialized) {
    Arduino_PinMap_Clear();
    s_initialized = 1;
  }
  for (uint32_t i = 0; i < ARDUINO_PINMAP_MAX_ENTRIES; ++i) {
    if (s_entries[i].pin == pin) {
      return &s_entries[i];
    }
    if (create && (s_entries[i].pin == NC)) {
      s_entries[i].pin = pin;
      return &s_entries[i];
    }
  }
  return NULL;
}

void Arduino_PinMap_Clear(void) {
  for (uint32_t i = 0; i < ARDUINO_PINMAP_MAX_ENTRIES; ++i) {
    s_entries[i].pin = NC;
    s_entries[i].timer = NULL;
    s_entries[i].timerChannel = 0;
    s_entries[i].adc = NULL;
    s_entries[i].adcChannel = 0;
    s_entries[i].adcBank = 0;
  }
}

void Arduino_PinMap_RegisterTimer(PinName pin, TIM_TypeDef *instance, uint32_t channel) {
  PinPeripheralEntry *entry = find_entry(pin, 1);
  if (entry == NULL) {
    return;
  }
  entry->timer = instance;
  entry->timerChannel = channel;
}

void Arduino_PinMap_RegisterAdc(PinName pin, ADC_TypeDef *instance, uint32_t channel, uint32_t bank) {
  PinPeripheralEntry *entry = find_entry(pin, 1);
  if (entry == NULL) {
    return;
  }
  entry->adc = instance;
  entry->adcChannel = channel;
  entry->adcBank = bank;
}

static const int s_tim_token;
static const int s_adc_token;

const void *PinMap_TIM = &s_tim_token;
const void *PinMap_ADC = &s_adc_token;

PinName digitalPinToPinName(uint32_t pin) {
  register_default_pinmaps();
  if ((pin & 0xFFFFFF00u) != 0u) {
    return (PinName)pin;
  }
  if (pin < ARRAY_SIZE(kDigitalPinMap)) {
    return kDigitalPinMap[pin];
  }
  return NC;
}

PinName analogInputToPinName(uint32_t pin) {
  register_default_pinmaps();
  if (pin >= PNUM_ANALOG_BASE) {
    uint32_t index = pin - PNUM_ANALOG_BASE;
    if (index < ARRAY_SIZE(kAnalogPinMap)) {
      return kAnalogPinMap[index];
    }
  }
  return digitalPinToPinName(pin);
}

static GPIO_TypeDef *port_from_index(uint32_t index) {
#if defined(GPIOA)
  if (index == STM_PORT_A) {
    return GPIOA;
  }
#endif
#if defined(GPIOB)
  if (index == STM_PORT_B) {
    return GPIOB;
  }
#endif
#if defined(GPIOC)
  if (index == STM_PORT_C) {
    return GPIOC;
  }
#endif
#if defined(GPIOD)
  if (index == STM_PORT_D) {
    return GPIOD;
  }
#endif
#if defined(GPIOE)
  if (index == STM_PORT_E) {
    return GPIOE;
  }
#endif
#if defined(GPIOF)
  if (index == STM_PORT_F) {
    return GPIOF;
  }
#endif
#if defined(GPIOG)
  if (index == STM_PORT_G) {
    return GPIOG;
  }
#endif
#if defined(GPIOH)
  if (index == STM_PORT_H) {
    return GPIOH;
  }
#endif
#if defined(GPIOI)
  if (index == STM_PORT_I) {
    return GPIOI;
  }
#endif
#if defined(GPIOJ)
  if (index == STM_PORT_J) {
    return GPIOJ;
  }
#endif
#if defined(GPIOK)
  if (index == STM_PORT_K) {
    return GPIOK;
  }
#endif
  return NULL;
}

GPIO_TypeDef *Arduino_PinNameToPort(PinName pin) {
  if (pin == NC) {
    return NULL;
  }
  uint32_t portIndex = (pin >> 8) & 0xFFu;
  return port_from_index(portIndex);
}

uint16_t Arduino_PinNameToMask(PinName pin) {
  if (pin == NC) {
    return 0;
  }
  uint32_t pinIndex = pin & 0xFFu;
  if (pinIndex >= 16u) {
    return 0;
  }
  return (uint16_t)(1u << pinIndex);
}

uint32_t pinmap_peripheral(PinName pin, const void *map) {
  register_default_pinmaps();
  PinPeripheralEntry *entry = find_entry(pin, 0);
  if (entry == NULL) {
    return 0u;
  }
  if (map == PinMap_TIM) {
    return (uint32_t)(uintptr_t)entry->timer;
  }
  if (map == PinMap_ADC) {
    return (uint32_t)(uintptr_t)entry->adc;
  }
  return 0u;
}

uint32_t pinmap_function(PinName pin, const void *map) {
  register_default_pinmaps();
  PinPeripheralEntry *entry = find_entry(pin, 0);
  if (entry == NULL) {
    return 0u;
  }
  if (map == PinMap_TIM) {
    return entry->timerChannel;
  }
  return 0u;
}

uint32_t STM_PIN_CHANNEL(uint32_t encoded) {
  return encoded;
}

uint32_t get_adc_channel(PinName pin, uint32_t *bank) {
  register_default_pinmaps();
  PinPeripheralEntry *entry = find_entry(pin, 0);
  if (entry == NULL) {
    return 0u;
  }
  if (bank != NULL) {
    *bank = entry->adcBank;
  }
  return entry->adcChannel;
}

uint32_t get_adc_internal_channel(PinName pin) {
  (void)pin;
  return 0u;
}
