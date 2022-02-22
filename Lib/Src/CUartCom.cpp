/**
 * @file CUartCom.cpp
 *
 */

/*
 *  Created on: 12 Feb 2022
 *      Author: niuslar
 */

#include "CUartCom.h"

#define BYTE 1

uint8_t CUartCom::s_rx_buffer[MAX_RX_BUF_LEN] = {0};
uint8_t *CUartCom::s_rx_buf_addr = CUartCom::s_rx_buffer;
std::queue<std::string> CUartCom::s_queue;
bool CUartCom::s_queue_full_flag = false;
/**
 *  @brief Constructor to configure UART communication
 */
CUartCom::CUartCom(std::string name, UART_HandleTypeDef *p_huart)
    : IComChannel(name),
      mp_huart(p_huart)
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
 *  @brief Constructor to configure UART communication
 *  with USART_DE Pin.
 */
CUartCom::CUartCom(const std::string name,
                   UART_HandleTypeDef *p_huart,
                   GPIO_TypeDef *uart_de_port,
                   uint16_t uart_de_pin)
    : IComChannel(name),
      mp_huart(p_huart),
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

void CUartCom::startReception()
{
    // Start UART reception with 1 byte length
    HAL_UART_Receive_IT(mp_huart, CUartCom::s_rx_buffer, BYTE);
}

/**
 * @brief sends message via UART
 * @param string message
 */
void CUartCom::send(const std::string &msg)
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

/**
 * @brief UART Rx Complete Callback
 * @param huart pointer to huart handler
 * @note This is only to define the Callback function,
 * 		 it will be called when the Rx Complete flag is set.
 */
extern "C"
{
    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
    {
        // Check if we reached the end of message marked by \n
        if (*CUartCom::s_rx_buf_addr == '\n')
        {
            // Mark the end of string by replacing \n with \0
            *CUartCom::s_rx_buf_addr = '\0';

            // Push string to queue
            if (CUartCom::s_queue_full_flag)
            {
                CUartCom::s_rx_buf_addr = CUartCom::s_rx_buffer;
                HAL_UART_Receive_IT(huart, CUartCom::s_rx_buf_addr, BYTE);
                return;
            }
            CUartCom::s_queue_full_flag = false;
            std::string buffer = (char *)CUartCom::s_rx_buffer;
            CUartCom::s_queue.push(buffer);

            // Restart buffer to original address
            CUartCom::s_rx_buf_addr = CUartCom::s_rx_buffer;

            // Restart interrupt with 1 byte
            HAL_UART_Receive_IT(huart, CUartCom::s_rx_buf_addr, BYTE);
        }
        else
        {
            // Increase buffer address
            CUartCom::s_rx_buf_addr++;

            // Restart interrupt with 1 byte
            HAL_UART_Receive_IT(huart, CUartCom::s_rx_buf_addr, BYTE);
        }

        // Check queue status
        if (CUartCom::s_queue.size() >= MAX_QUEUE_SIZE)
        {
            // Set queue full flag
            CUartCom::s_queue_full_flag = true;
        }
    }
}

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
bool CUartCom::isCommandAvailable()
{
    if (!CUartCom::s_queue.empty())
    {
        return true;
    }
    return false;
}

bool CUartCom::isQueueFull()
{
    return CUartCom::s_queue_full_flag;
}
