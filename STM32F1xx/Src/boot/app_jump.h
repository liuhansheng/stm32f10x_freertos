#pragma once
#include "stdint.h"

#define MAGIC_NUM_UART_BOOT   0x746F6F62    // "boot"

void app_jump_to_boot(uint32_t magic_num);