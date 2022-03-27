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
CUartCom::CUartCom(UART_HandleTypeDef *p_huart, const std::string name)
    : IComChannel(name),
      mp_huart(p_huart),
      m_uart_de_pin(USART1_DE_GPIO_Port, USART1_DE_Pin)
{
    // Check mp_huart is not null
    if (!mp_huart)
    {
        Error_Handler();
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
                   const std::string name,
                   GPIO_TypeDef *uart_de_port,
                   uint16_t uart_de_pin)
    : IComChannel(name),
      mp_huart(p_huart),
      m_uart_de_pin(uart_de_port, uart_de_pin)
{
    // Check mp_huart is not null
    if (!mp_huart)
    {
        Error_Handler();
    }
}

/**
 * @brief sends message via UART.
 * @param msg Message to send.
 */
void CUartCom::send(std::string msg)
{
    uint16_t msg_len = msg.length();

    // Enable UART_DE Pin
    m_uart_de_pin.set(GPIO_PIN_SET);

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

        m_uart_de_pin.set(GPIO_PIN_RESET);

        return;
    }

    // Send message
    HAL_UART_Transmit(mp_huart, (uint8_t *)c_msg, msg_len, UART_TIMEOUT);

    // Disable UART_DE Pin
    m_uart_de_pin.set(GPIO_PIN_RESET);
}

bool CUartCom::isCommandAvailable()
{
    return false;
}

std::string CUartCom::getCommand()
{
    return "False";
}
