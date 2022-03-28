/**
 * @file CFifoBuffer.cpp
 */

/*
 *  Created on: Mar 28, 2022
 *      Author: niuslar
 */

#include <CCmdBuffer.h>

CCmdBuffer::CCmdBuffer() : m_head(0), m_tail(BUF_SIZE)
{
    // TODO Auto-generated constructor stub
}

CCmdBuffer::~CCmdBuffer()
{
    // TODO Auto-generated destructor stub
}

/**
 * @brief Adds character to buffer
 * @return True if the character is \n or \r to mark end of string
 */
bool CCmdBuffer::put(const char data)
{
    bool end_of_string = false;
    if (data == '\n' || data == '\r' || isFull())
    {
        end_of_string = true;
        // Mark end of string and reset head
        m_rx_buffer[m_head] = '\0';
        m_head = 0;
    }
    else
    {
        m_rx_buffer[m_head] = data;
        m_head++;
    }
    return end_of_string;
}

/**
 * @brief Gets the string stored in the buffer
 * @return string
 */
std::string CCmdBuffer::get()
{
    std::string string = (char *)m_rx_buffer;
    return string;
}

/**
 * @brief Check if buffer reached its max size
 * @return True if the buffer is full
 */
bool CCmdBuffer::isFull()
{
    if (m_head == m_tail)
    {
        return true;
    }
    return false;
}
