/**
 * @file CUartCom.cpp
 *
 */

/*
 *  Created on: 12 Feb 2022
 *      Author: niuslar
 */

#include "CUartCom.h"

CUartCom *CUartCom::sp_UART_1 = NULL;
CUartCom *CUartCom::sp_UART_2 = NULL;

/**
 * @brief Construct UART communication object.
 * @param p_huart Pointer to UART hardware control register structure.
 * @param name Name of the instance
 * @note If no USART_DE Pin is needed, the BREATHING Pin will turn on when UART
 * is sending data
 */
CUartCom::CUartCom(UART_HandleTypeDef *p_huart, const std::string name)
    : IComChannel(name),
      mp_huart(p_huart),
      m_uart_de_pin(BREATHING_GPIO_Port, BREATHING_Pin)
{
    // Check mp_huart is not null
    if (!mp_huart)
    {
        Error_Handler();
    }
}

/**
 * @brief Construct UART communication object with half-duplex mode.
 * @param p_huart Pointer to UART hardware control register structure.
 * @param name Name of the CUartCom Instance
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
 * @brief Enables UART reception with interrupts
 * @note Interrupts must be enabled for the UART port in order to use this
 * function
 *
 */
void CUartCom::startReception()
{
    HAL_UART_Receive_IT(mp_huart, &m_rx_char, 1);
    if (mp_huart->Instance == USART1)
    {
        sp_UART_1 = this;
    }
    if (mp_huart->Instance == USART2)
    {
        sp_UART_2 = this;
    }
}

/**
 * @brief sends message via UART.
 * @param msg Message to send.
 */
void CUartCom::send(const std::string &msg)
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

/**
 * @brief UART Reception Complete Interrupt Handler
 * @arg p_huart Pointer to UART Handler
 */
void CUartCom::uartRxHandler(UART_HandleTypeDef *p_huart)
{
    // Store incoming char
    if (m_rx_buffer.put((const char)m_rx_char))
    {
        // If end of string or rx_buffer is full, add to queue
        std::string command_string = m_rx_buffer.get();

        // If the queue has reached its max size, the message will be
        // dismissed
        if (m_cmd_queue.size() <= MAX_QUEUE_SIZE)
        {
            m_cmd_queue.push(command_string);
        }
    }

    HAL_UART_Receive_IT(p_huart, &m_rx_char, 1);
}

/**
 * @brief Check if the queue has any commands to read
 * @return True if the queue is not empty
 */
bool CUartCom::isCommandAvailable()
{
    if (!m_cmd_queue.empty())
    {
        return true;
    }
    return false;
}

/**
 * @brief Gets and deletes the first command in the queue
 * @return Command as string
 */
std::string CUartCom::getCommand()
{
    std::string command = m_cmd_queue.front();
    m_cmd_queue.pop();
    return command;
}

/**
 * @brief UART Rx complete callback
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *p_huart)
{
    if (p_huart->Instance == USART1)
    {
        CUartCom::sp_UART_1->uartRxHandler(p_huart);
    }
    if (p_huart->Instance == USART2)
    {
        CUartCom::sp_UART_2->uartRxHandler(p_huart);
    }
}
