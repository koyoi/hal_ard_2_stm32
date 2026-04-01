#include "arduino_gpio.h"
#include "arduino_pinmap.h"

static uint8_t is_valid_pin(PinName pin) {
  return (pin != NC) && (Arduino_PinNameToPort(pin) != NULL);
}

void pinMode(uint32_t pin, uint8_t mode) {
  GPIO_InitTypeDef gpio = {0};
  GPIO_TypeDef *port;
  uint16_t mask;
  PinName pin_name = digitalPinToPinName(pin);

  if (!is_valid_pin(pin_name)) {
    return;
  }

  port = Arduino_PinNameToPort(pin_name);
  mask = Arduino_PinNameToMask(pin_name);

  gpio.Pin = mask;
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

  HAL_GPIO_Init(port, &gpio);
}

void digitalWrite(uint32_t pin, uint8_t value) {
  GPIO_TypeDef *port;
  uint16_t mask;
  GPIO_PinState state;
  PinName pin_name = digitalPinToPinName(pin);

  if (!is_valid_pin(pin_name)) {
    return;
  }

  port = Arduino_PinNameToPort(pin_name);
  mask = Arduino_PinNameToMask(pin_name);
  state = (value != LOW) ? GPIO_PIN_SET : GPIO_PIN_RESET;

  HAL_GPIO_WritePin(port, mask, state);
}

int digitalRead(uint32_t pin) {
  GPIO_TypeDef *port;
  uint16_t mask;
  PinName pin_name = digitalPinToPinName(pin);

  if (!is_valid_pin(pin_name)) {
    return LOW;
  }

  port = Arduino_PinNameToPort(pin_name);
  mask = Arduino_PinNameToMask(pin_name);

  return (HAL_GPIO_ReadPin(port, mask) == GPIO_PIN_SET) ? HIGH : LOW;
}
