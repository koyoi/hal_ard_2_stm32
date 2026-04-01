#include "HardwareTimer.h"

#ifndef HARDWARE_TIMER_MAX_INSTANCES
#define HARDWARE_TIMER_MAX_INSTANCES 8
#endif

#ifndef HARDWARE_TIMER_MAX_PINS
#define HARDWARE_TIMER_MAX_PINS 128
#endif

namespace {
struct TimerInstanceEntry {
  TIM_TypeDef *instance;
  HardwareTimer *timer;
};

TimerInstanceEntry g_timerInstances[HARDWARE_TIMER_MAX_INSTANCES];

struct TimerCallbackMask {
  uint32_t flag;
  uint32_t enable;
};

const TimerCallbackMask kChannelMasks[4] = {
    {TIM_SR_CC1IF, TIM_DIER_CC1IE},
    {TIM_SR_CC2IF, TIM_DIER_CC2IE},
    {TIM_SR_CC3IF, TIM_DIER_CC3IE},
    {TIM_SR_CC4IF, TIM_DIER_CC4IE},
};

IRQn_Type timer_irq_from_instance(TIM_TypeDef *instance) {
#if defined(TIM1)
  if (instance == TIM1) {
    return TIM1_CC_IRQn;
  }
#endif
#if defined(TIM2)
  if (instance == TIM2) {
    return TIM2_IRQn;
  }
#endif
#if defined(TIM3)
  if (instance == TIM3) {
    return TIM3_IRQn;
  }
#endif
#if defined(TIM4)
  if (instance == TIM4) {
    return TIM4_IRQn;
  }
#endif
#if defined(TIM5)
  if (instance == TIM5) {
    return TIM5_IRQn;
  }
#endif
#if defined(TIM8)
  if (instance == TIM8) {
    return TIM8_CC_IRQn;
  }
#endif
  return (IRQn_Type)(-1);
}

void register_timer_instance(HardwareTimer *timer) {
  for (auto &entry : g_timerInstances) {
    if ((entry.instance == nullptr) || (entry.instance == timer->instance())) {
      entry.instance = timer->instance();
      entry.timer = timer;
      return;
    }
  }
}

HardwareTimer *find_timer(TIM_TypeDef *instance) {
  for (auto &entry : g_timerInstances) {
    if (entry.instance == instance) {
      return entry.timer;
    }
  }
  return nullptr;
}

void enable_timer_clock(TIM_TypeDef *instance) {
#if defined(TIM1)
  if (instance == TIM1) { __HAL_RCC_TIM1_CLK_ENABLE(); return; }
#endif
#if defined(TIM2)
  if (instance == TIM2) { __HAL_RCC_TIM2_CLK_ENABLE(); return; }
#endif
#if defined(TIM3)
  if (instance == TIM3) { __HAL_RCC_TIM3_CLK_ENABLE(); return; }
#endif
#if defined(TIM4)
  if (instance == TIM4) { __HAL_RCC_TIM4_CLK_ENABLE(); return; }
#endif
#if defined(TIM5)
  if (instance == TIM5) { __HAL_RCC_TIM5_CLK_ENABLE(); return; }
#endif
#if defined(TIM8)
  if (instance == TIM8) { __HAL_RCC_TIM8_CLK_ENABLE(); return; }
#endif
}

} // namespace

HardwareTimer::HardwareTimer(TIM_TypeDef *instance)
    : m_instance(instance), m_callbacks{nullptr, nullptr, nullptr, nullptr} {
  enable_timer_clock(m_instance);
  register_timer_instance(this);
}

void HardwareTimer::pause() {
  if (m_instance == nullptr) {
    return;
  }
  m_instance->CR1 &= ~TIM_CR1_CEN;
}

void HardwareTimer::resume() {
  if (m_instance == nullptr) {
    return;
  }
  m_instance->CR1 |= TIM_CR1_CEN;
}

void HardwareTimer::refresh() {
  if (m_instance == nullptr) {
    return;
  }
  m_instance->EGR |= TIM_EGR_UG;
}

void HardwareTimer::setPrescaleFactor(uint32_t factor) {
  if ((m_instance == nullptr) || (factor == 0U)) {
    return;
  }
  m_instance->PSC = (uint16_t)(factor - 1U);
}

void HardwareTimer::setOverflow(uint32_t overflow) {
  if ((m_instance == nullptr) || (overflow == 0U)) {
    return;
  }
  m_instance->ARR = (uint32_t)(overflow - 1U);
}

void HardwareTimer::setMode(uint32_t channel, uint32_t mode) {
  (void)channel;
  (void)mode;
  // For TIMER_OUTPUT_COMPARE we do not need special configuration in this minimal wrapper.
}

void HardwareTimer::setMode(uint32_t channel, uint32_t mode, PinName pin) {
  (void)channel;
  (void)mode;
  (void)pin;
  // PWM configuration not implemented in this minimal wrapper.
}

void HardwareTimer::setCaptureCompare(uint32_t channel, uint32_t value) {
  if (m_instance == nullptr) {
    return;
  }
  switch (channel) {
    case TIMER_CH1:
      m_instance->CCR1 = value;
      break;
    case TIMER_CH2:
      m_instance->CCR2 = value;
      break;
    case TIMER_CH3:
      m_instance->CCR3 = value;
      break;
    case TIMER_CH4:
      m_instance->CCR4 = value;
      break;
    default:
      break;
  }
}

void HardwareTimer::setCompare(uint32_t channel, uint32_t value) {
  setCaptureCompare(channel, value);
}

void HardwareTimer::attachInterrupt(uint32_t channel, void (*callback)(void)) {
  if ((channel < 1U) || (channel > 4U) || (m_instance == nullptr)) {
    return;
  }
  m_callbacks[channel - 1U] = callback;
  const TimerCallbackMask &mask = kChannelMasks[channel - 1U];
  m_instance->DIER |= mask.enable;

  IRQn_Type irq = timer_irq_from_instance(m_instance);
  if ((int32_t)irq >= 0) {
    HAL_NVIC_SetPriority(irq, 1, 0);
    HAL_NVIC_EnableIRQ(irq);
  }
}

void HardwareTimer::handleInterrupt() {
  if (m_instance == nullptr) {
    return;
  }
  for (uint32_t i = 0; i < 4; ++i) {
    if ((m_callbacks[i] != nullptr) && (m_instance->SR & kChannelMasks[i].flag)) {
      m_instance->SR &= ~kChannelMasks[i].flag;
      m_callbacks[i]();
    }
  }
}

extern "C" void HardwareTimer_HandleIRQ(TIM_TypeDef *instance) {
  HardwareTimer *timer = find_timer(instance);
  if (timer != nullptr) {
    timer->handleInterrupt();
  }
}
