/**
 * @file CUartBuffer.cpp
 */

/*
 *  Created on: Mar 28, 2022
 *      Author: niuslar
 */

#include "CUartBuffer.h"

CUartBuffer::CUartBuffer() : m_head(0), m_tail(0)
{
    // TODO Auto-generated constructor stub
}

CUartBuffer::~CUartBuffer()
{
    // TODO Auto-generated destructor stub
}

/**
 * @brief Adds character to buffer
 */
void CUartBuffer::put(const char data)
{
    m_rx_buffer[m_head] = data;
    if (m_head == (BUF_SIZE - 1))
    {
        m_head = 0;
    }
    m_head++;
}

/**
 * @brief Gets data from buffer
 * @return char
 */
char CUartBuffer::get()
{
    char data = m_rx_buffer[m_tail];
    m_rx_buffer[m_tail] = 0;
    if (m_tail == (BUF_SIZE - 1))
    {
        m_tail = 0;
    }
    m_tail++;
    return data;
}

/**
 * @brief gets string stored in ringbuffer
 * @return string
 */
std::string CUartBuffer::getString()
{
    uint8_t counter = 0;
    char c_string[BUF_SIZE];
    char data;
    while ((data = get()) != '\0' && counter < BUF_SIZE)
    {
        c_string[counter] = data;
        counter++;
    }
    c_string[counter] = '\0';
    std::string cpp_string = (char *)c_string;

    return cpp_string;
}
