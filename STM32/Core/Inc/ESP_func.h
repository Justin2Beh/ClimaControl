#ifndef __ESP_FUNC_H
#define __ESP_FUNC_H

#include "stm32f4xx_hal.h"

void ESP_Init (UART_HandleTypeDef *WIFI_huart, char *SSID, char *PASS);

void ESP_Handle_Req (UART_HandleTypeDef *WIFI_huart);

#endif // __ESP_FUNC_H
