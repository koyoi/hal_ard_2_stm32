/*
 * Minimal pgmspace compatibility for non-AVR targets.
 */

#ifndef HAL_ARDUINO_PGMSPACE_H
#define HAL_ARDUINO_PGMSPACE_H

#include <stdint.h>
#include <string.h>

#define PROGMEM
#define PGM_P const char *
#define PGM_VOID_P const void *

static inline uint8_t pgm_read_byte_near(const void *addr) {
  return *(const uint8_t *)(addr);
}

static inline uint16_t pgm_read_word_near(const void *addr) {
  uint16_t value;
  memcpy(&value, addr, sizeof(value));
  return value;
}

static inline uint32_t pgm_read_dword_near(const void *addr) {
  uint32_t value;
  memcpy(&value, addr, sizeof(value));
  return value;
}

static inline float pgm_read_float_near(const void *addr) {
  float value;
  memcpy(&value, addr, sizeof(value));
  return value;
}

#define memcpy_P(dest, src, len) memcpy((dest), (src), (len))
#define strcpy_P(dest, src)      strcpy((dest), (src))
#define strlen_P(src)            strlen((src))
#define pgm_read_ptr(addr)       (*(const void * const *)(addr))

#endif /* HAL_ARDUINO_PGMSPACE_H */
