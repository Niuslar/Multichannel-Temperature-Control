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
    if (s_uart_instances < MAX_UART_ENGINES)
    {
        sp_UART[s_uart_instances] = this;
        s_uart_instances++;
    }
    else
    {
        b_success = false;
    }

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
 * @brief Start UART Transmission and push message to the TX Queue
 * @param msg Message to send.
 * TODO: Add return value (True if there was space in the queue)
 */
void CUartCom::send(const std::string msg)
{
    bool event = NO_MESSAGE_AVAILABLE;

    if (m_tx_queue.size() <= MAX_TX_QUEUE_SIZE && (msg.empty() == false))
    {
        m_tx_queue.push(msg);
    }

    if (m_tx_queue.empty() == false)
    {
        event = MESSAGE_AVAILABLE;
    }

    if (m_status == IDLE)
    {
        if (event == NO_MESSAGE_AVAILABLE)
        {
            endTx();
        }
        else if (event == MESSAGE_AVAILABLE)
        {
            updateTxBuffer();
            transmit();
        }
    }
}

/**
 * @brief Update TX Buffer with next element on queue
 */
void CUartCom::updateTxBuffer()
{
    std::string message = m_tx_queue.front();
    m_tx_msg_length = message.length();
    strcpy(m_tx_buffer, message.c_str());
    m_status = TX;
    m_tx_queue.pop();
}

void CUartCom::transmit()
{
    // Enable USART_DE pin
    m_uart_de_pin.set(true);
    HAL_UART_Transmit_IT(mp_huart, (uint8_t *)m_tx_buffer, m_tx_msg_length);
}

/**
 * @brief Ends UART TX and disables UART DE Pin
 */
void CUartCom::endTx()
{
    // Disable interrupt, disable pin
    __HAL_UART_DISABLE_IT(mp_huart, UART_IT_TXE);
    m_uart_de_pin.set(false);
    m_status = IDLE;
}

/**
 * @brief UART Reception Complete Interrupt Handler
 * @arg p_huart Pointer to UART Handler
 */
void CUartCom::uartRxHandler(UART_HandleTypeDef *p_huart)
{
    // Store incoming char
    static uint8_t len_counter = 0;
    if (m_rx_char == '\n' || m_rx_char == '\r' || len_counter >= (BUF_SIZE - 3))
    {
        m_rx_buffer.put((char)m_rx_char);
        m_rx_buffer.put('\0');

        std::string rx_string = m_rx_buffer.getString();
        if (m_rx_queue.size() <= MAX_RX_QUEUE_SIZE && !rx_string.empty())
        {
            m_rx_queue.push(rx_string);
        }
        len_counter = 0;
    }
    else
    {
        m_rx_buffer.put((char)m_rx_char);
        len_counter++;
    }

    HAL_UART_Receive_IT(p_huart, &m_rx_char, 1);
}

/**
 * @brief UART Transmission Complete Interrupt Handler
 * @arg p_huart Pointer to UART Handler
 */
void CUartCom::uartTxHandler(UART_HandleTypeDef *p_huart)
{
    bool event = NO_MESSAGE_AVAILABLE;
    if (m_tx_queue.empty() == false)
    {
        event = MESSAGE_AVAILABLE;
    }
    // Check if queue is empty
    if (event == MESSAGE_AVAILABLE)
    {
        updateTxBuffer();
        transmit();
    }
    else
    {
        endTx();
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
    if (m_rx_queue.empty() == false)
    {
        std::string command = m_rx_queue.front();
        m_rx_queue.pop();
        return command;
    }
    return "";
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
