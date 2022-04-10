/**
 * @file CFIFOBuffer.h
 */

/*
 *  Created on: Mar 28, 2022
 *      Author: niuslar
 */

#ifndef CFIFOBUFFER_H_
#define CFIFOBUFFER_H_

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
     * @return True if buffer is empty
     */
    // TODO: deprecated in next release cycle.
    bool empty()
    {
#warning "do not use this method. To be deprecated."
        if (m_count == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    uint16_t size() const
    {
        return m_count;
    }
};

#endif /* CFIFOBUFFER_H_ */
