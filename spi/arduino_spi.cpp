#include "arduino_spi.h"

SPIClass SPI;

SPIClass::SPIClass() : m_hspi(nullptr), m_timeoutMs(100U), m_activeSettings() {}

void SPIClass::attachHandle(SPI_HandleTypeDef *handle) {
  m_hspi = handle;
}

void SPIClass::begin() {}

void SPIClass::beginTransaction(const SPISettings &settings) {
  m_activeSettings = settings;
  applySettings(settings);
}

void SPIClass::endTransaction() {}

uint8_t SPIClass::transfer(uint8_t data) {
  uint8_t rx = 0U;
  if (m_hspi == nullptr) {
    return 0U;
  }
  (void)HAL_SPI_TransmitReceive(m_hspi, &data, &rx, 1U, m_timeoutMs);
  return rx;
}

void SPIClass::transfer(void *buf, size_t count) {
  if ((m_hspi == nullptr) || (buf == nullptr) || (count == 0U)) {
    return;
  }
  (void)HAL_SPI_TransmitReceive(m_hspi, (uint8_t *)buf, (uint8_t *)buf, (uint16_t)count, m_timeoutMs);
}

void SPIClass::applySettings(const SPISettings &settings) {
  uint32_t polarity = SPI_POLARITY_LOW;
  uint32_t phase = SPI_PHASE_1EDGE;

  if (m_hspi == nullptr) {
    return;
  }

  switch (settings.m_dataMode) {
    case SPI_MODE0:
      polarity = SPI_POLARITY_LOW;
      phase = SPI_PHASE_1EDGE;
      break;
    case SPI_MODE1:
      polarity = SPI_POLARITY_LOW;
      phase = SPI_PHASE_2EDGE;
      break;
    case SPI_MODE2:
      polarity = SPI_POLARITY_HIGH;
      phase = SPI_PHASE_1EDGE;
      break;
    case SPI_MODE3:
    default:
      polarity = SPI_POLARITY_HIGH;
      phase = SPI_PHASE_2EDGE;
      break;
  }

  m_hspi->Init.CLKPolarity = polarity;
  m_hspi->Init.CLKPhase = phase;
  m_hspi->Init.FirstBit = (settings.m_bitOrder == LSBFIRST) ? SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;

  // Keep prescaler as configured in CubeMX unless user edits this function.
  (void)HAL_SPI_Init(m_hspi);
}
