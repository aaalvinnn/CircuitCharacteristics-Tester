#ifndef _MCP41010_H
#define _MCP41010_H

#include "stm32f4xx_hal.h"
#include "main.h"
// ------------------------- Defines -------------------------
#define MCP41010_PORT GPIOC      // PORT OF AD9833
#define MCP41010_DATA GPIO_PIN_9 // SPI DATA PIN
#define MCP41010_SCK GPIO_PIN_8  // SPI Clock PIN
#define MCP41010_CS GPIO_PIN_7   // SPI Chip Select


// ------------------ Functions  -----------------------------
void MCP41010_Init(uint8_t xRes);




#endif