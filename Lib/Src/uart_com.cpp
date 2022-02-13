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
CUartCom::CUartCom(UART_HandleTypeDef &huart) : m_huart(huart) {}

/**
 *  @brief Constructor to configure UART communication
 *  with USART_DE Pin.
 */
CUartCom::CUartCom(UART_HandleTypeDef &huart,
                   GPIO_TypeDef *uart_de_port,
                   uint16_t uart_de_pin)
    : m_huart(huart),
      m_uart_de_port(uart_de_port),
      m_uart_de_pin(uart_de_pin)
{
}

/**
 * @brief sends message via UART
 * @param string message
 */
void CUartCom::sendMessage(const char *msg)
{
    uint16_t msg_len = strlen(msg);

    // Enable UART_DE Pin
    if (m_uart_de_port != NULL)
    {
        HAL_GPIO_WritePin(m_uart_de_port, m_uart_de_pin, GPIO_PIN_SET);
    }

    // Send message
    HAL_UART_Transmit(&m_huart, (uint8_t *)msg, msg_len, UART_TIMEOUT);

    // Disable UART_DE Pin
    if (m_uart_de_port != NULL)
    {
        HAL_GPIO_WritePin(m_uart_de_port, m_uart_de_pin, GPIO_PIN_RESET);
    }
}
