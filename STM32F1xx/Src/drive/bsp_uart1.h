#pragma once

#include "bsp_uarts.h"

extern UART_HandleTypeDef huart1;

void bsp_uart1_dma_tx_cplt_cb(void);
void bsp_uart1_dma_rx_halt_cb(void);
void bsp_uart1_dma_rx_cplt_cb(void);
void bsp_uart1_rx_idle_cb(void);

void bsp_uart1_init(uint32_t baudrate);
void bsp_uart1_install_rx_callback(usart_rx_handler_cb cb);

size_t bsp_uart1_send(uint8_t *data, uint16_t len);