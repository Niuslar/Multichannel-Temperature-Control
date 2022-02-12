/**
 * @file uart_com.cpp
 *
 */

/*
 *  Created on: 12 Feb 2022
 *      Author: niuslar
 */

#include "uart_com.h"

/**
 *  @brief Constructor to configure UART communication
 */
UartCom::UartCom(UART_HandleTypeDef &huart) : m_huart(huart) {}

/**
 * @brief sends message via UART
 * @param string message
 */
void UartCom::send_msg(const char *msg)
{
    uint16_t msg_len = strlen(msg);

    // Enable UART Pin
    HAL_GPIO_WritePin(USART_DE_GPIO_Port, USART_DE_Pin, GPIO_PIN_SET);

    // Send message
    HAL_UART_Transmit(&m_huart, (uint8_t *)msg, msg_len, UART_TIMEOUT);

    // Disable UART Pin
    HAL_GPIO_WritePin(USART_DE_GPIO_Port, USART_DE_Pin, GPIO_PIN_RESET);
}
