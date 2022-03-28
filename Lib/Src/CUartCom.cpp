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
<<<<<<< HEAD
 * @brief Starts the UART reception with interrupt
 * @note  The interrupt needs to be enabled for the UART
 * in the hardware configuration for this method to work.
 */
void CUartCom::startReception()
{
    HAL_UART_Receive_IT(mp_huart, CUartCom::s_rx_buffer, BYTE);
}

/**
 * @brief sends message via UART.
 * @param msg Message to send.
 */
=======
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
>>>>>>> feature/nu_uart_f411
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

/*
 * @brief UART Reception Complete Interrupt Handler
 * @arg p_huart Pointer to UART Handler
 */
void CUartCom::uartRxHandler(UART_HandleTypeDef *p_huart)
{
    // Store incoming char and restore interrupt
    if (m_rx_buffer.put((const char)m_rx_char))
    {
        // If end of string or rx_buffer is full, add to queue
        std::string command_string = m_rx_buffer.get();
        if (m_cmd_queue.size() <= MAX_QUEUE_SIZE)
        {
            // If the queue has reached its max size, the message will be
            // dismissed
            m_cmd_queue.push(command_string);
        }
    }
    HAL_UART_Receive_IT(p_huart, &m_rx_char, 1);
}

/*
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

std::string CUartCom::getCommand()
{
    std::string command = m_cmd_queue.front();
    m_cmd_queue.pop();
    return command;
}

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

/**
 * @brief UART Rx Complete Callback
 * @param p_huart Pointer to UART hardware control register structure
 * @note This is only to define the Callback function,
 * 		 it will be called when the Rx Complete flag is set.
 */
extern "C"
{
    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *p_huart)
    {
        // Check if we reached the end of message marked by \n
        if (*CUartCom::s_rx_buf_addr == '\n')
        {
            // Mark the end of string by replacing \n with \0
            *CUartCom::s_rx_buf_addr = '\0';

            // Push string to queue if queue is not full
            if (CUartCom::s_queue_full_flag)
            {
                // If the queue is full, messages will be ignored and lost
                CUartCom::s_rx_buf_addr = CUartCom::s_rx_buffer;
                HAL_UART_Receive_IT(p_huart, CUartCom::s_rx_buf_addr, BYTE);
                return;
            }

            std::string buffer = (char *)CUartCom::s_rx_buffer;
            CUartCom::s_queue.push(buffer);

            // Restart buffer to original address
            CUartCom::s_rx_buf_addr = CUartCom::s_rx_buffer;
        }
        else
        {
            CUartCom::s_rx_buf_addr++;
            if (++CUartCom::s_cmd_length_counter >= MAX_RX_BUF_LEN)
            {
                CUartCom::s_cmd_length_counter = 0;
                CUartCom::s_rx_buf_addr = CUartCom::s_rx_buffer;
            }
        }

        // Check queue status
        if (CUartCom::s_queue.size() >= MAX_QUEUE_SIZE)
        {
            CUartCom::s_queue_full_flag = true;
        }

        HAL_UART_Receive_IT(p_huart, CUartCom::s_rx_buf_addr, BYTE);
    }
}

/**
 * @brief get a command if available
 * @retval Command as a string
 * @note FIFO queue.
 */
std::string CUartCom::getCommand()
{
    std::string command = "Error, empty queue";
    if (!CUartCom::s_queue.empty())
    {
        command = CUartCom::s_queue.front();
        CUartCom::s_queue.pop();

        CUartCom::s_queue_full_flag = false;
    }

    return command;
}

/**
 * @retval True if the queue is not empty.
 * 		   False otherwise
 */
bool CUartCom::isCommandAvailable()
{
    if (!CUartCom::s_queue.empty())
    {
        return true;
    }
    return false;
}

/**
 * @retval True if the queue has reached the
 * 		   MAX_QUEUE_SIZE capacity.
 */
bool CUartCom::isQueueFull()
{
    return CUartCom::s_queue_full_flag;
}
