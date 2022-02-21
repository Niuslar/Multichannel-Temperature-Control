/**
 * @file CUartCom.cpp
 *
 */

/*
 *  Created on: 12 Feb 2022
 *      Author: niuslar
 */

#include "CUartCom.h"

/**
 * @brief Construct UART communication object.
 *
 * @param p_huart Pointer to UART hardware control register structure.
 */
CUartCom::CUartCom(UART_HandleTypeDef *p_huart) : mp_huart(p_huart)
{
    // Check mp_huart is not null
    if (!mp_huart)
    {
        // Hang application with a toggling ALARM Pin
        while (1)
        {
            HAL_GPIO_TogglePin(ALARM_GPIO_Port, ALARM_Pin);
            HAL_Delay(200);
        }
    }
}

/**
 * @brief Construct UART communication object with half-duplex mode.
 *
 * @param p_huart Pointer to UART hardware control register structure.
 * @param uart_de_port Pointer to GPIO port.
 * @param uart_de_pin GPIO pin.
 */
CUartCom::CUartCom(UART_HandleTypeDef *p_huart,
                   GPIO_TypeDef *uart_de_port,
                   uint16_t uart_de_pin)
    : mp_huart(p_huart),
      m_uart_de_port(uart_de_port),
      m_uart_de_pin(uart_de_pin)
{
    // Check mp_huart is not null
    if (!mp_huart)
    {
        // Hang application with a toggling ALARM Pin
        while (1)
        {
            HAL_GPIO_TogglePin(ALARM_GPIO_Port, ALARM_Pin);
            HAL_Delay(200);
        }
    }
}

/**
 * @brief sends message via UART.
 * @param msg Message to send.
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
        HAL_UART_Transmit(mp_huart,
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
    HAL_UART_Transmit(mp_huart, (uint8_t *)c_msg, msg_len, UART_TIMEOUT);

    // Disable UART_DE Pin
    if (m_uart_de_port != nullptr)
    {
        HAL_GPIO_WritePin(m_uart_de_port, m_uart_de_pin, GPIO_PIN_RESET);
    }
}
