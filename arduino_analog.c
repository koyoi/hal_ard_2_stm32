#include "arduino_analog.h"

#include "arduino_pinmap.h"

static ADC_HandleTypeDef *s_adc_handle = NULL;

static uint32_t adc_sampling_time(void) {
#if defined(ADC_SAMPLETIME_3CYCLES)
  return ADC_SAMPLETIME_3CYCLES;
#elif defined(ADC_SAMPLETIME_4CYCLES)
  return ADC_SAMPLETIME_4CYCLES;
#elif defined(ADC_SAMPLETIME_6CYCLES)
  return ADC_SAMPLETIME_6CYCLES;
#elif defined(ADC_SAMPLETIME_7CYCLES_5)
  return ADC_SAMPLETIME_7CYCLES_5;
#elif defined(ADC_SAMPLETIME_15CYCLES)
  return ADC_SAMPLETIME_15CYCLES;
#elif defined(ADC_SAMPLETIME_19CYCLES_5)
  return ADC_SAMPLETIME_19CYCLES_5;
#else
  return 0;
#endif
}

void Arduino_AttachADCHandle(ADC_HandleTypeDef *handle) {
  s_adc_handle = handle;
}

static HAL_StatusTypeDef configure_adc_channel(PinName pin_name) {
  uint32_t bank = 0;
  uint32_t channel = get_adc_channel(pin_name, &bank);
  ADC_ChannelConfTypeDef config = {0};
  if (channel == 0 && (pin_name & PADC_BASE) == 0) {
    return HAL_ERROR;
  }
  config.Channel = channel;
#ifdef ADC_REGULAR_RANK_1
  config.Rank = ADC_REGULAR_RANK_1;
#else
  config.Rank = 1;
#endif
  uint32_t sample = adc_sampling_time();
  if (sample != 0U) {
    config.SamplingTime = sample;
  }
#ifdef ADC_SINGLE_ENDED
  config.SingleDiff = ADC_SINGLE_ENDED;
#endif
#ifdef ADC_OFFSET_NONE
  config.OffsetNumber = ADC_OFFSET_NONE;
  config.Offset = 0;
#endif
  return HAL_ADC_ConfigChannel(s_adc_handle, &config);
}

int analogRead(uint32_t pin) {
  if (s_adc_handle == NULL) {
    return 0;
  }
  PinName pin_name = analogInputToPinName(pin);
  if (pin_name == NC) {
    return 0;
  }
  if (configure_adc_channel(pin_name) != HAL_OK) {
    return 0;
  }
  if (HAL_ADC_Start(s_adc_handle) != HAL_OK) {
    return 0;
  }
  if (HAL_ADC_PollForConversion(s_adc_handle, 10U) != HAL_OK) {
    HAL_ADC_Stop(s_adc_handle);
    return 0;
  }
  uint32_t value = HAL_ADC_GetValue(s_adc_handle);
  HAL_ADC_Stop(s_adc_handle);
  return (int)value;
}
