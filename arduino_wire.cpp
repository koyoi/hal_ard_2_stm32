#include "arduino_wire.h"

TwoWire Wire;

TwoWire::TwoWire()
    : m_hi2c(nullptr),
      m_targetAddr(0U),
      m_txLen(0U),
      m_rxLen(0U),
      m_rxIndex(0U),
      m_timeoutMs(100U),
      m_txBusy(false),
      m_txDone(false),
      m_txError(false),
      m_txErrorCode(HAL_I2C_ERROR_NONE) {}

void TwoWire::attachHandle(I2C_HandleTypeDef *handle) {
  m_hi2c = handle;
}

void TwoWire::begin() {
  m_txLen = 0U;
  m_rxLen = 0U;
  m_rxIndex = 0U;

  m_txBusy = false;
  m_txDone = false;
  m_txError = false;
  m_txErrorCode = HAL_I2C_ERROR_NONE;
}

void TwoWire::setClock(uint32_t clockHz) {
  (void)clockHz;
  // CubeMX generated timing is normally fixed at build-time.
}

void TwoWire::beginTransmission(uint8_t address) {
  if (m_txBusy) {
    return;
  }
  m_targetAddr = address;
  m_txLen = 0U;
  m_txDone = false;
  m_txError = false;
  m_txErrorCode = HAL_I2C_ERROR_NONE;
}

size_t TwoWire::write(uint8_t data) {
  if (m_txBusy) {
    return 0U;
  }
  if (m_txLen >= TX_BUFFER_SIZE) {
    return 0U;
  }
  m_txBuffer[m_txLen++] = data;
  return 1U;
}

size_t TwoWire::write(const uint8_t *data, size_t len) {
  size_t written = 0U;
  if (data == nullptr) {
    return 0U;
  }
  if (m_txBusy) {
    return 0U;
  }
  while ((written < len) && (m_txLen < TX_BUFFER_SIZE)) {
    m_txBuffer[m_txLen++] = data[written++];
  }
  return written;
}

uint8_t TwoWire::endTransmission(uint8_t sendStop) {
  HAL_StatusTypeDef st;
  (void)sendStop;

  if (m_hi2c == nullptr) {
    return 4U;
  }
  if (m_txBusy) {
    return 5U; // 独自: busy
  }

  m_txDone = false;
  m_txError = false;
  m_txErrorCode = HAL_I2C_ERROR_NONE;

  st = HAL_I2C_Master_Transmit(
      m_hi2c,
      (uint16_t)(m_targetAddr << 1U),
      m_txBuffer,
      (uint16_t)m_txLen,
      m_timeoutMs);

  m_txLen = 0U;

  if (st == HAL_OK) {
    m_txDone = true;
    return 0U;
  }

  m_txError = true;
  m_txErrorCode = HAL_I2C_GetError(m_hi2c);
  return 4U;
}

uint8_t TwoWire::endTransmissionAsync(uint8_t sendStop) {
  HAL_StatusTypeDef st;
  (void)sendStop;

  if (m_hi2c == nullptr) {
    return 4U;
  }
  if (m_txBusy) {
    return 5U; // 独自: busy
  }
  if (m_txLen == 0U) {
    m_txDone = true;
    m_txError = false;
    m_txErrorCode = HAL_I2C_ERROR_NONE;
    return 0U;
  }

  m_txBusy = true;
  m_txDone = false;
  m_txError = false;
  m_txErrorCode = HAL_I2C_ERROR_NONE;

  st = HAL_I2C_Master_Transmit_DMA(
      m_hi2c,
      (uint16_t)(m_targetAddr << 1U),
      m_txBuffer,
      (uint16_t)m_txLen);

  if (st != HAL_OK) {
    m_txBusy = false;
    m_txDone = false;
    m_txError = true;
    m_txErrorCode = HAL_I2C_GetError(m_hi2c);
    return 4U;
  }

  // DMA転送中は m_txBuffer が使われているので、ここでは m_txLen を消さない
  return 0U;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  HAL_StatusTypeDef st;
  uint16_t reqLen = quantity;
  (void)sendStop;

  if (m_hi2c == nullptr) {
    return 0U;
  }
  if (m_txBusy) {
    return 0U;
  }
  if (reqLen > RX_BUFFER_SIZE) {
    reqLen = RX_BUFFER_SIZE;
  }

  st = HAL_I2C_Master_Receive(
      m_hi2c,
      (uint16_t)(address << 1U),
      m_rxBuffer,
      reqLen,
      m_timeoutMs);

  if (st != HAL_OK) {
    m_rxLen = 0U;
    m_rxIndex = 0U;
    return 0U;
  }

  m_rxLen = reqLen;
  m_rxIndex = 0U;
  return (uint8_t)m_rxLen;
}

int TwoWire::available() {
  if (m_rxIndex >= m_rxLen) {
    return 0;
  }
  return (int)(m_rxLen - m_rxIndex);
}

int TwoWire::read() {
  if (m_rxIndex >= m_rxLen) {
    return -1;
  }
  return (int)m_rxBuffer[m_rxIndex++];
}

bool TwoWire::txBusy() const {
  return m_txBusy;
}

bool TwoWire::txDone() const {
  return (!m_txBusy) && m_txDone && (!m_txError);
}

bool TwoWire::txError() const {
  return m_txError;
}

uint32_t TwoWire::txErrorCode() const {
  return m_txErrorCode;
}

bool TwoWire::ownsHandle(I2C_HandleTypeDef *handle) const {
  return (m_hi2c == handle);
}

void TwoWire::onTxCplt() {
  m_txBusy = false;
  m_txDone = true;
  m_txError = false;
  m_txErrorCode = HAL_I2C_ERROR_NONE;
  m_txLen = 0U;
}

void TwoWire::onError() {
  m_txBusy = false;
  m_txDone = false;
  m_txError = true;
  m_txErrorCode = (m_hi2c != nullptr) ? HAL_I2C_GetError(m_hi2c) : HAL_I2C_ERROR_NONE;
  m_txLen = 0U;
}

extern "C" void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
  if (Wire.txBusy() && Wire.ownsHandle(hi2c)) {
    Wire.onTxCplt();
  }
}

extern "C" void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
  if (Wire.ownsHandle(hi2c)) {
    Wire.onError();
  }
}
