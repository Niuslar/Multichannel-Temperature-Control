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
 * @note An instance needs to be initialised using the init() method
 */
CUartCom::CUartCom(const etl::string<MAX_STRING_SIZE> name)
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
    bool b_success = false;

    if (!p_huart)
    {
        Error_Handler();
        b_success = false;
    }
    else
    {
        mp_huart = p_huart;
        if (uart_de_port != nullptr)
        {
            // When UART_DE pin is used, half-duplex is assumed
            mb_half_duplex = true;
        }
        m_uart_de_pin.init(uart_de_port, uart_de_pin);

        // Check this hardware is not assigned to another instance
        int8_t index = CUartCom::getIndex(p_huart);
        if (index < 0)
        {
            // Assign hardware to instance
            if (s_uart_instances < MAX_UART_ENGINES)
            {
                sp_UART[s_uart_instances] = this;
                s_uart_instances++;
                b_success = true;
            }
        }
    }

    return b_success;
}

/**
 * @brief Enable UART reception with interrupts
 * @note UART Interrupts must be enabled in the device configuration in order
 * to use this method
 */
void CUartCom::startRx()
{
    HAL_UART_Receive_IT(mp_huart, &m_rx_byte, 1);
}

/**
 * @brief Disable UART RX Interrupts
 * @note used when using half-duplex
 */
void CUartCom::stopRx()
{
    HAL_UART_AbortReceive_IT(mp_huart);
}

/**
 * @brief Send string message
 * @param msg Message to send.
 * @return True if successful
 */
bool CUartCom::send(etl::string<MAX_STRING_SIZE> msg)
{
    uint32_t msg_len = msg.length();
    bool b_success = send((uint8_t *)msg.c_str(), msg_len);
    return b_success;
}

/**
 * @brief Add data to the TX Queue and start transmission if possible
 * @param Pointer to data buffer
 * @param Length of the data buffer (in bytes)
 * @return True if message was added to the queue
 */
bool CUartCom::send(uint8_t *p_data_buf, uint32_t len)
{
    bool b_success = false;

    CUartCom::tx_data_t data_obj;

    data_obj.length = len;
    // Store data in internal data_obj buffer
    for (uint32_t i = 0; i < len; i++)
    {
        data_obj.buffer[i] = p_data_buf[i];
    }

    // Insert data_obj into TX queue
    if (m_tx_queue.put(data_obj))
    {
        b_success = true;
    }

    // Start transmission only if UART is not transmitting already
    if (m_status == IDLE)
    {
        transmit();
    }
    return b_success;
}

/**
 * @brief Start UART transmission with interrupt
 * @return True if starts transmission successfully
 */
bool CUartCom::transmit()
{
    bool b_success = false;
    if (m_tx_queue.size() > 0)
    {
        // If working with half-duplex, disable RX interrupts
        if (mb_half_duplex && m_status == IDLE)
        {
            stopRx();
        }

        // Enable USART_DE pin
        m_uart_de_pin.set(true);

        m_status = TX;

        m_data = m_tx_queue.get();
        HAL_UART_Transmit_IT(mp_huart, (uint8_t *)m_data.buffer, m_data.length);
        b_success = true;
    }

    return b_success;
}

/**
 * @brief Re-enable UART RX Interrupts, Disable UART TX Interrupt and and
 * disable UART_DE Pin
 */
void CUartCom::endTx()
{
    if (mb_half_duplex)
    {
        startRx();
    }

    // Disable TX interrupt, disable pin
    __HAL_UART_DISABLE_IT(mp_huart, UART_IT_TXE);
    m_uart_de_pin.set(false);
    m_status = IDLE;
}

/**
 * @brief UART Reception Complete Interrupt Handler
 * @param p_huart Pointer to UART Handler
 */
void CUartCom::uartRxHandler(UART_HandleTypeDef *p_huart)
{
    if (!m_rx_queue.put(m_rx_byte))
    {
        send("[ERROR]: Buffer overflow -> m_rx_queue\n");
        m_rx_queue.reset();
    }

    HAL_UART_Receive_IT(p_huart, &m_rx_byte, 1);
}

/**
 * @brief UART Transmission Complete Interrupt Handler
 * @param p_huart Pointer to UART Handler
 */
void CUartCom::uartTxHandler(UART_HandleTypeDef *p_huart)
{
    // Check if queue is empty
    if (m_tx_queue.size() > 0)
    {
        transmit();
    }
    else
    {
        endTx();
    }
}

/**
 * @brief Check if RX Queue is empty
 * @return True if the queue is not empty
 */
bool CUartCom::isDataAvailable()
{
    if (m_rx_queue.size() > 0)
    {
        return true;
    }
    return false;
}

/**
 * @brief Get and delete the first element in the RX queue
 * @return byte of data
 */
uint8_t CUartCom::getData()
{
    uint8_t data = m_rx_queue.get();
    return data;
}

/**
 * @brief Get index for the active UART handler
 * @return index from 0 to MAX_UART_ENGINES
 */
int8_t CUartCom::getIndex(UART_HandleTypeDef *p_huart)
{
    int8_t index = -1;
    for (uint8_t i = 0; i < CUartCom::s_uart_instances; i++)
    {
        if (p_huart->Instance == CUartCom::sp_UART[i]->mp_huart->Instance)
        {
            index = i;
        }
    }
    return index;
}

/**
 * @brief UART Rx complete callback
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *p_huart)
{
    int8_t index = CUartCom::getIndex(p_huart);
    if (index >= 0)
    {
        CUartCom::sp_UART[index]->uartRxHandler(p_huart);
    }
    else
    {
        Error_Handler();
    }
}

/**
 * @brief UART Tx complete callback
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *p_huart)
{
    int8_t index = CUartCom::getIndex(p_huart);
    if (index >= 0)
    {
        CUartCom::sp_UART[index]->uartTxHandler(p_huart);
    }
    else
    {
        Error_Handler();
    }
}
