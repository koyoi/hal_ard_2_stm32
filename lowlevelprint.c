#include "stm32f4xx_hal.h"  // Change to your MCU's HAL header
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// External UART handle (must be initialized in main.c or elsewhere)
extern UART_HandleTypeDef huart2; // Example: using USART2

/**
 * @brief Send debug message over UART.
 * @param msg  Null-terminated string to send.
 * @param len  Length of the message to send.
 */
void uart_debug_write(const uint8_t *msg, const size_t len) {
    if (msg == NULL) {
        return; // Avoid null pointer dereference
    }

    // size_t len = strlen((const char *)msg);
    if (len == 0) {
        return; // Nothing to send
    }

    // Transmit with timeout (blocking)
    if (HAL_UART_Transmit(&huart2, (uint8_t *)msg, len, HAL_MAX_DELAY) != HAL_OK) {
        // Optional: handle transmission error
    }
}

/**
 * @brief Redirect printf to UART (optional).
 */
int __io_putchar(int ch) {
    uint8_t c = (uint8_t)ch;
    HAL_UART_Transmit(&huart2, &c, 1, HAL_MAX_DELAY);
    return ch;
}
