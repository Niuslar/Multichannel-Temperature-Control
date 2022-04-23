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
    uint16_t m_head = 0;
    uint16_t m_tail = 0;
    uint16_t m_count = 0;

public:
    /**
     * @brief Add data to buffer
     */
    bool put(T data)
    {
        bool b_success = false;
        /**
         * @note Disable interrupts while calling put() to avoid simultaneous
         * access to the buffer and its variables.
         */
        __disable_irq();
        if (m_count < BUF_SIZE)
        {
            m_rx_buffer[m_head] = data;
            if (++m_head >= BUF_SIZE)
            {
                m_head = 0;
            }
            m_count++;
            b_success = true;
        }
        __enable_irq();
        return b_success;
    }

    /**
     * @brief Get data from buffer
     * @return T data
     */
    T get()
    {
        T data = m_rx_buffer[m_tail];
        if (m_count > 0)
        {
            if (++m_tail >= BUF_SIZE)
            {
                m_tail = 0;
            }
            m_count--;
        }
        return data;
    }

    /**
     * @brief Empty buffer
     */
    void reset()
    {
        m_head = 0;
        m_tail = 0;
        m_count = 0;
    }

    uint16_t size() const
    {
        return m_count;
    }
};

#endif /* CFIFOBUFFER_H_ */
