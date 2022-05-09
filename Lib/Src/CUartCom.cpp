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
    HAL_UART_Receive_IT(mp_huart, &m_rx_char, 1);
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
 * @brief send string
 * @param msg Message to be sent via UART
 * @return success if message was successfully stored in TX Queue.
 */
bool CUartCom::send(etl::string<MAX_STRING_SIZE> msg)
{
    uint32_t msg_len = msg.length();
    bool b_success = send((uint8_t *)msg.c_str(), msg_len);
    return b_success;
}

/**
 * @brief Add data to the TX Queue and start transmission if possible
 * @param p_data_buf Reference to array of uint8_t data
 * @param len Size of data that needs to be stored
 * @return True if data was added to the queue, false in case of buffer overflow
 */
bool CUartCom::send(uint8_t *p_data_buf, uint32_t len)
{
    bool b_success = true;

    // Store data in tx_queue
    for (uint32_t i = 0; i < len; i++)
    {
        if (m_tx_queue.put(p_data_buf[i]) == false)
        {
            b_success = false;
        }
    }

    if (m_status == IDLE)
    {
        transmit();
    }
    return b_success;
}

/**
 * @brief Update TX Buffer with next element on queue
 */
void CUartCom::updateTxBuffer()
{
    m_tx_char = m_tx_queue.get();
    m_status = TX;
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

        updateTxBuffer();
        HAL_UART_Transmit_IT(mp_huart, &m_tx_char, 1);
        b_success = true;
    }

    return b_success;
}

/**
 * @brief Disable UART TX Interrupt, Enable RX Interrupts and and disable
 * UART_DE Pin
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
    if (m_rx_buffer.put((char)m_rx_char) == false)
    {
        send("Error: Max string size is RX_BUF_SIZE\n");
        m_rx_buffer.reset();
    }

    if (m_rx_char == '\n' || m_rx_char == '\r')
    {
        etl::string<MAX_STRING_SIZE> rx_string = getString();
        if (m_rx_queue.size() <= MAX_RX_QUEUE_SIZE && !rx_string.empty())
        {
            if (m_rx_queue.put(rx_string) == false)
            {
                send("Error: Buffer overflow -> RX Queue\n");
            }
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
 * @brief Get next string stored in FIFO Buffer
 * @return string
 */
etl::string<MAX_STRING_SIZE> CUartCom::getString()
{
    uint8_t counter = 0;
    char c_string[RX_BUF_SIZE];
    char data = m_rx_buffer.get();
    /**
     * @note while loop stops when the counter is (RX_BUF_SIZE - 1) because
     * an extra space is needed for the '\0' character.
     */

    while ((data != '\0') && (data != '\n') && (data != '\r') &&
           (counter < (RX_BUF_SIZE - 1)))
    {
        c_string[counter] = data;
        data = m_rx_buffer.get();
        counter++;
    }
    c_string[counter] = '\0';
    etl::string<MAX_STRING_SIZE> cpp_string = (char *)c_string;
    m_rx_buffer.reset();

    return cpp_string;
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
 * @return Data as string
 */
etl::string<MAX_STRING_SIZE> CUartCom::getData()
{
    if (m_rx_queue.size() > 0)
    {
        etl::string<MAX_STRING_SIZE> data = m_rx_queue.get();
        return data;
    }
    return "";
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
