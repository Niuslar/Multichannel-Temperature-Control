/**
 * @file CFIFOBuffer.h
 */

/*
 *  Created on: Mar 28, 2022
 *      Author: niuslar
 */

#ifndef CFIFOBUFFER_H_
#define CFIFOBUFFER_H_

#include "main.h"

template <typename T, uint16_t BUF_SIZE>
class CFIFOBuffer
{
private:
    T m_rx_buffer[BUF_SIZE];
    uint32_t m_head = 0;
    uint32_t m_tail = 0;

public:
    /**
     * @brief Adds data to buffer
     */
    void put(T data)
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
     * @return T data
     */
    T get()
    {
        T data = m_rx_buffer[m_tail];
        if (m_tail == (BUF_SIZE - 1))
        {
            m_tail = 0;
        }
        m_tail++;
        return data;
    }

    /**
     * @return True if buffer is empty
     */
    bool empty()
    {
        if (m_head == m_tail)
        {
            return true;
        }
        return false;
    }

    uint16_t size()
    {
        uint16_t size;
        if (empty())
        {
            size = 0;
        }
        else if (m_head > m_tail)
        {
            size = m_head - m_tail;
        }
        else
        {
            size = (BUF_SIZE - m_tail) + m_head;
        }

        return size;
    }
};

#endif /* CFIFOBUFFER_H_ */
