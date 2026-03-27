#include "arduino_gpio.h"

#ifndef ARDUINO_GPIO_MAX_PINS
#define ARDUINO_GPIO_MAX_PINS 128U
#endif

static ArduinoGpioPinMap s_pinMap[ARDUINO_GPIO_MAX_PINS];

static uint8_t is_valid_pin(uint16_t pin) {
  return (pin < ARDUINO_GPIO_MAX_PINS) &&
         (s_pinMap[pin].port != NULL) &&
         (s_pinMap[pin].pinMask != 0U);
}

void Arduino_GPIO_RegisterPin(uint16_t arduinoPin, GPIO_TypeDef *port, uint16_t pinMask) {
  if (arduinoPin >= ARDUINO_GPIO_MAX_PINS) {
    return;
  }
  s_pinMap[arduinoPin].port = port;
  s_pinMap[arduinoPin].pinMask = pinMask;
}

void Arduino_GPIO_ClearPinMap(void) {
  for (uint16_t i = 0; i < ARDUINO_GPIO_MAX_PINS; ++i) {
    s_pinMap[i].port = NULL;
    s_pinMap[i].pinMask = 0U;
  }
}

void pinMode(uint16_t pin, uint8_t mode) {
  GPIO_InitTypeDef gpio = {0};
  if (!is_valid_pin(pin)) {
    return;
  }

  gpio.Pin = s_pinMap[pin].pinMask;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;

  switch (mode) {
    case OUTPUT:
      gpio.Mode = GPIO_MODE_OUTPUT_PP;
      gpio.Pull = GPIO_NOPULL;
      break;
    case INPUT_PULLUP:
      gpio.Mode = GPIO_MODE_INPUT;
      gpio.Pull = GPIO_PULLUP;
      break;
    case INPUT_PULLDOWN:
      gpio.Mode = GPIO_MODE_INPUT;
      gpio.Pull = GPIO_PULLDOWN;
      break;
    case INPUT:
    default:
      gpio.Mode = GPIO_MODE_INPUT;
      gpio.Pull = GPIO_NOPULL;
      break;
  }

  HAL_GPIO_Init(s_pinMap[pin].port, &gpio);
}

void digitalWrite(uint16_t pin, uint8_t value) {
  GPIO_PinState state;
  if (!is_valid_pin(pin)) {
    return;
  }
  state = (value != LOW) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(s_pinMap[pin].port, s_pinMap[pin].pinMask, state);
}

int digitalRead(uint16_t pin) {
  if (!is_valid_pin(pin)) {
    return LOW;
  }
  return (HAL_GPIO_ReadPin(s_pinMap[pin].port, s_pinMap[pin].pinMask) == GPIO_PIN_SET) ? HIGH : LOW;
}
