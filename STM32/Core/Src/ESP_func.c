/**
  ******************************************************************************
  * @file           : ESP8266_func.c
  * @brief          : Functions to initialize connection and run server with
  *                   ESP8266
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "ESP_func.h"

void ESP_Init (UART_HandleTypeDef *WIFI_huart, char *SSID, char *PASS)
{
	char data[80];
	char received[80];

	// reset ESP
	HAL_UART_Transmit(WIFI_huart, (uint8_t*)"AT+RST\r\n", strlen("AT+RST\r\n"), 1000);
	HAL_Delay(1000);

//	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)"AT\r\n", strlen("ATr\n"), 1000);
//	HAL_Delay(2000);

	// set ESP to station mode
	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)"AT+CWMODE=1\r\n", strlen("AT+CWMODE=1\r\n"), 1000);
	HAL_Delay(1000);

	// join specified network
	sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", *SSID, *PASS);
	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)data, strlen(data), 1000);
	HAL_Delay(1000);

	// initialize ESP server on port 80
	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)"AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), 1000);
	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)"AT+CIPSERVER=1,80\r\n", strlen("AT+CIPSERVER=1,80\r\n"), 1000);
	HAL_Delay(1000);
}

void ESP_Start_Server (void)
{
	// TODO: Setup responses for various GET methods (PUT too for arduino?)
	//
}
