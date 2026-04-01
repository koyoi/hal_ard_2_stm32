#ifndef ARDUINO_HARDWARE_TIMER_H
#define ARDUINO_HARDWARE_TIMER_H

#include "arduino_compat_common.h"
#include "arduino_pinmap.h"
#include "main.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void HardwareTimer_HandleIRQ(TIM_TypeDef *instance);

#ifdef __cplusplus
}
#endif

#define TIMER_CH1 1U
#define TIMER_CH2 2U
#define TIMER_CH3 3U
#define TIMER_CH4 4U

#define TIMER_OUTPUT_COMPARE 0U
#define TIMER_OUTPUT_COMPARE_PWM1 1U

class HardwareTimer {
public:
  explicit HardwareTimer(TIM_TypeDef *instance);

  void pause();
  void resume();
  void refresh();

  void setPrescaleFactor(uint32_t factor);
  void setOverflow(uint32_t overflow);
  void setMode(uint32_t channel, uint32_t mode);
  void setMode(uint32_t channel, uint32_t mode, PinName pin);
  void setCaptureCompare(uint32_t channel, uint32_t value);
  void setCompare(uint32_t channel, uint32_t value);
  void attachInterrupt(uint32_t channel, void (*callback)(void));

  TIM_TypeDef *instance() const { return m_instance; }

private:
  TIM_TypeDef *m_instance;
  void (*m_callbacks[4])(void);

  void configureChannel(uint32_t channel, uint32_t mode, PinName pin);
  void configureGpioForPin(PinName pin);
  void enableTimerClock();
  void enableMainOutputIfNeeded();
  void handleInterrupt();

  friend void ::HardwareTimer_HandleIRQ(TIM_TypeDef *instance);
};

#endif // ARDUINO_HARDWARE_TIMER_H
