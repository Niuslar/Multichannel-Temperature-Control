/*
 * CFifoBuffer.cpp
 *
 *  Created on: Mar 28, 2022
 *      Author: niuslar
 */

#include <CFifoBuffer.h>

CFifoBuffer::CFifoBuffer() : m_head(0), m_tail(BUF_SIZE)
{
    // TODO Auto-generated constructor stub
}

CFifoBuffer::~CFifoBuffer()
{
    // TODO Auto-generated destructor stub
}

/*
 * @brief Adds character to buffer
 * @return True if the character is \n or \r to mark end of string
 */
bool CFifoBuffer::put(const char data)
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

std::string CFifoBuffer::get()
{
    std::string string = (char *)m_rx_buffer;
    return string;
}

bool CFifoBuffer::isFull()
{
    if (m_head == m_tail)
    {
        return true;
    }
    return false;
}
