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
void CUartCom::sendMessage(const std::string &msg)
{
    uint16_t msg_len = msg.length();

    // Enable UART_DE Pin
    if (m_uart_de_port != nullptr)
    {
        HAL_GPIO_WritePin(m_uart_de_port, m_uart_de_pin, GPIO_PIN_SET);
    }

    // Convert string to C style
    const char *c_msg = msg.c_str();

    // Check it is a valid string and send error message if not
    if (!(*c_msg) || c_msg == nullptr)
    {
        const uint8_t error_msg_len = 28;
        const char *error_msg = "Invalid string from c_str()\n";
        HAL_UART_Transmit(&m_huart,
                          (uint8_t *)error_msg,
                          error_msg_len,
                          UART_TIMEOUT);

        // Disable UART_DE Pin
        if (m_uart_de_port != nullptr)
        {
            HAL_GPIO_WritePin(m_uart_de_port, m_uart_de_pin, GPIO_PIN_RESET);
        }

        return;
    }

    // Send message
    HAL_UART_Transmit(&m_huart, (uint8_t *)c_msg, msg_len, UART_TIMEOUT);

    // Disable UART_DE Pin
    if (m_uart_de_port != nullptr)
    {
        HAL_GPIO_WritePin(m_uart_de_port, m_uart_de_pin, GPIO_PIN_RESET);
    }
}
