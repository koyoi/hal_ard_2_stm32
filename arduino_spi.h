#ifndef ARDUINO_SPI_H
#define ARDUINO_SPI_H

#include "arduino_compat_common.h"
#include "main.h"
#include <stddef.h>
#include <stdint.h>

class SPISettings {
public:
  SPISettings(uint32_t clock = 1000000U, uint8_t bitOrder = MSBFIRST, uint8_t dataMode = SPI_MODE0)
      : m_clock(clock), m_bitOrder(bitOrder), m_dataMode(dataMode) {}

  uint32_t m_clock;
  uint8_t m_bitOrder;
  uint8_t m_dataMode;
};

class SPIClass {
public:
  SPIClass();

  void attachHandle(SPI_HandleTypeDef *handle);
  void begin();
  void beginTransaction(const SPISettings &settings);
  void endTransaction();

  uint8_t transfer(uint8_t data);
  void transfer(void *buf, size_t count);

private:
  SPI_HandleTypeDef *m_hspi;
  uint32_t m_timeoutMs;
  SPISettings m_activeSettings;

  void applySettings(const SPISettings &settings);
};

extern SPIClass SPI;

#endif // ARDUINO_SPI_H
