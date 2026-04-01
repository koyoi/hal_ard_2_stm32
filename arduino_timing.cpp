#include "arduino_compat_common.h"

uint32_t millis(void) {
  return HAL_GetTick();
}

uint32_t micros(void) {
  uint32_t ms;
  uint32_t systick_val;
  uint32_t load = SysTick->LOAD + 1U;
  do {
    ms = HAL_GetTick();
    systick_val = SysTick->VAL;
  } while (ms != HAL_GetTick());
  uint32_t ticks = load - systick_val;
  uint32_t us = (ticks * 1000U) / load;
  return (ms * 1000U) + us;
}

void delay(uint32_t ms) {
  HAL_Delay(ms);
}

void delayMicroseconds(uint32_t us) {
  uint32_t start = micros();
  while ((micros() - start) < us) {
    __NOP();
  }
}
