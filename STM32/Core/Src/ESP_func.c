/**
  ******************************************************************************
  * @file           : ESP8266_func.c
  * @brief          : Functions to initialize connection and run server with
  *                   ESP8266
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "ESP_func.h"
#include "string.h"

/* ESP functions */
void ESP_Init (UART_HandleTypeDef *WIFI_huart, char *SSID, char *PASS)
{
	char data[80];
	char received[80];

	// reset ESP
	HAL_UART_Transmit(WIFI_huart, (uint8_t*)"AT+RST\r\n", strlen("AT+RST\r\n"), 1000);
	HAL_Delay(1000);

	// set ESP to station mode
	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)"AT+CWMODE=1\r\n", strlen("AT+CWMODE=1\r\n"), 1000);
	HAL_Delay(1000);

	// join specified network
	sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASS);
	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)data, strlen(data), 1000);
	HAL_Delay(1000);

	// initialize ESP server on port 80
	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)"AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), 1000);
	HAL_UART_Transmit(&WIFI_huart, (uint8_t*)"AT+CIPSERVER=1,80\r\n", strlen("AT+CIPSERVER=1,80\r\n"), 1000);
	HAL_Delay(1000);
}

void ESP_Handle_Req (UART_HandleTypeDef *WIFI_huart, char *req)
{
	char res[120];
	char at_command[40];

	// format of request is "+IPD,0,n:xxxxxxxxxx", if not this, exit
	if (strncmp(req, "+IPD,") != 0)
		return;

	// get link ID
	char link_ID = req[5];

	// look for GET /sensor?new_read=val
	if (strstr(req, "?new_read") != NULL)
	{
		// TODO check if request is from arduino? (User agent field?)

		// TODO store new data to internal variables

		// Send response and close connection
		memset(at_command, 0, sizeof(at_command));
		sprintf(at_command, "AT+CIPSEND=%c,3\r\n", link_ID);
		HAL_UART_Transmit(&WIFI_huart, (uint8_t*)at_command, strlen(at_command), 1000);
		HAL_UART_Transmit(&WIFI_huart, (uint8_t*)"ACK\r\n", strlen("ACK\r\n"), 1000);

		memset(at_command, 0, sizeof(at_command));
		sprintf(at_command, "AT+CIPCLOSE=%c\r\n", link_ID);
		HAL_UART_Transmit(&WIFI_huart, (uint8_t*)at_command, strlen(at_command), 1000);
	}
	// check otherwise send webpage as response
	else
	{
		// clear old response
		// TODO set res to data
		memset(res, 0, sizeof(res));
		sprintf(res, "ACK\r\n"); // change

		// Send response and close connection
		memset(at_command, 0, sizeof(at_command));
		sprintf(at_command, "AT+CIPSEND=%c,%d\r\n", link_ID, strlen(res)); // change
		HAL_UART_Transmit(&WIFI_huart, (uint8_t*)at_command, strlen(at_command), 1000);
		HAL_UART_Transmit(&WIFI_huart, (uint8_t*)res, strlen(res), 1000);

		memset(at_command, 0, sizeof(at_command));
		sprintf(at_command, "AT+CIPCLOSE=%c\r\n", link_ID);
		HAL_UART_Transmit(&WIFI_huart, (uint8_t*)at_command, strlen(at_command), 1000);
	}

	// UART transmit calls are common, pull out of if/else?

}
