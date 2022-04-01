/**
 * @file CUartCom.cpp
 *
 */

/*
 *  Created on: 12 Feb 2022
 *      Author: niuslar
 */

#include "CUartCom.h"

CUartCom *CUartCom::sp_UART[MAX_UART_ENGINES] = {nullptr};
uint8_t CUartCom::s_uart_instances = 0;

/**
 * @brief Construct UART communication instance
 * @param name Name of the instance
 * @note An instance needs to be initialised using the init() method to
 * send and receive data
 */
CUartCom::CUartCom(const std::string name)
    : IComChannel(name),
      mp_huart(nullptr),
      m_uart_de_pin()
{
}

/**
 * @brief Initalise UART communication object
 * @param p_huart Pointer to UART hardware control register structure.
 */
bool CUartCom::init(UART_HandleTypeDef *p_huart)
{
    return init(p_huart, nullptr, 100);
}

/**
 * @brief Initalise UART communication object with half-duplex mode.
 * @param p_huart Pointer to UART hardware control register structure.
 * @param uart_de_port Pointer to GPIO port.
 * @param uart_de_pin GPIO pin.
 */
bool CUartCom::init(UART_HandleTypeDef *p_huart,
                    GPIO_TypeDef *uart_de_port,
                    uint16_t uart_de_pin)
{
    bool b_success = true;
    // Check mp_huart is not null
    if (!p_huart)
    {
        Error_Handler();
        b_success = false;
        return b_success;
    }
    else
    {
        mp_huart = p_huart;
        m_uart_de_pin.init(uart_de_port, uart_de_pin);
    }
    // Check this hardware is not assigned to another instance
    for (uint8_t i = 0; i < s_uart_instances; i++)
    {
        if (p_huart->Instance == sp_UART[i]->mp_huart->Instance)
        {
            b_success = false;
            return b_success;
        }
    }

    // Assign hardware to instance
    sp_UART[s_uart_instances] = this;
    s_uart_instances++;

    return b_success;
}

/**
 * @brief Enables UART reception with interrupts
 * @note UART Interrupts must be enabled in the hardware configuration in order
 * to use this method
 *
 */
void CUartCom::startRx()
{
    HAL_UART_Receive_IT(mp_huart, &m_rx_char, 1);
}

/**
 * @brief sends message via UART.
 * @param msg Message to send.
 * @return True if there was space in the transmission queue
 */
void CUartCom::send(const std::string msg)
{
    bool event = NON_EMPTY_MSG;
    // Check event (empty message or not)
    if (msg.empty())
    {
        event = EMPTY_MSG;
    }
    // Push msg to queue
    if (m_tx_queue.size() <= MAX_TX_QUEUE_SIZE && event == NON_EMPTY_MSG)
    {
        m_tx_queue.push(msg);
    }

    // Check state
    if (mb_state == IDLE)
    {
        if (event == EMPTY_MSG)
        {
            // Disable interrupt, disable pin
            __HAL_UART_DISABLE_IT(mp_huart, UART_IT_TXE);
            m_uart_de_pin.set(false);
        }
        else
        {
            // Enable USART_DE pin
            m_uart_de_pin.set(true);

            // Start transmission with interrupts
            std::string message = m_tx_queue.front();
            uint8_t msg_length = message.length();
            strcpy(m_tx_buffer, message.c_str());
            m_tx_queue.pop();
            HAL_UART_Transmit_IT(mp_huart, (uint8_t *)m_tx_buffer, msg_length);

            // Change state to TX
            mb_state = TX;
        }
    }
    else
    {
        if (event == EMPTY_MSG)
        {
            // Transmit next element in queue
            std::string message = m_tx_queue.front();
            uint8_t msg_length = message.length();
            strcpy(m_tx_buffer, message.c_str());
            m_tx_queue.pop();
            HAL_UART_Transmit_IT(mp_huart, (uint8_t *)m_tx_buffer, msg_length);
        }
    }
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
        if (m_rx_queue.size() <= MAX_RX_QUEUE_SIZE && !command_string.empty())
        {
            m_rx_queue.push(command_string);
        }
    }

    HAL_UART_Receive_IT(p_huart, &m_rx_char, 1);
}

/**
 * @brief UART Transmission Complete Interrupt Handler
 * @arg p_huart Pointer to UART Handler
 */
void CUartCom::uartTxHandler(UART_HandleTypeDef *p_huart)
{
    // Check if queue is empty
    if (m_tx_queue.empty())
    {
        // Stop tx
        mb_state = IDLE;
        send("");
    }
    else
    {
        // Keep sending data
        send("");
    }
}

/**
 * @brief Check if the queue has any commands to read
 * @return True if the queue is not empty
 */
bool CUartCom::isCommandAvailable()
{
    if (!m_rx_queue.empty())
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
    std::string command = m_rx_queue.front();
    m_rx_queue.pop();
    return command;
}

/**
 * @brief UART Rx complete callback
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *p_huart)
{
    for (uint8_t i = 0; i < CUartCom::s_uart_instances; i++)
    {
        if (p_huart->Instance == CUartCom::sp_UART[i]->mp_huart->Instance)
        {
            CUartCom::sp_UART[i]->uartRxHandler(p_huart);
        }
    }
}

/**
 * @brief UART Tx complete callback
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *p_huart)
{
    for (uint8_t i = 0; i < CUartCom::s_uart_instances; i++)
    {
        if (p_huart->Instance == CUartCom::sp_UART[i]->mp_huart->Instance)
        {
            CUartCom::sp_UART[i]->uartTxHandler(p_huart);
        }
    }
}
