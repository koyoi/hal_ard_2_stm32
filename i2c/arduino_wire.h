#ifndef ARDUINO_WIRE_H
#define ARDUINO_WIRE_H

#include "../arduino_compat_common.h"
#include "main.h"
#include <stddef.h>
#include <stdint.h>

class TwoWire {
public:
  TwoWire();

  void attachHandle(I2C_HandleTypeDef *handle);
  void begin();
  void setClock(uint32_t clockHz);

  void beginTransmission(uint8_t address);
  size_t write(uint8_t data);
  size_t write(const uint8_t *data, size_t len);
  uint8_t endTransmission(uint8_t sendStop = 1U);

  uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop = 1U);
  int available();
  int read();

private:
  static const size_t TX_BUFFER_SIZE = 64U;
  static const size_t RX_BUFFER_SIZE = 64U;

  I2C_HandleTypeDef *m_hi2c;
  uint8_t m_targetAddr;
  uint8_t m_txBuffer[TX_BUFFER_SIZE];
  uint8_t m_rxBuffer[RX_BUFFER_SIZE];
  size_t m_txLen;
  size_t m_rxLen;
  size_t m_rxIndex;
  uint32_t m_timeoutMs;
};

extern TwoWire Wire;

#endif // ARDUINO_WIRE_H
