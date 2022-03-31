/**
 * @file CFifoBuffer.cpp
 */

/*
 *  Created on: Mar 28, 2022
 *      Author: niuslar
 */

#include <CCmdBuffer.h>

CCmdBuffer::CCmdBuffer() : m_head(0), m_tail(BUF_SIZE - 1)
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
    bool is_full = isFull();
    if (data == '\n' || data == '\r' || is_full)
    {
        end_of_string = true;
        m_rx_buffer[m_head] = data;
        // Mark end of string and reset head
        m_rx_buffer[m_head + is_full] = '\0';
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
    /**@note The message ends 1 before the end of the buffer is full
     * to add the '\0' character
     */
    if (m_head == (m_tail - 1))
    {
        return true;
    }
    return false;
}
