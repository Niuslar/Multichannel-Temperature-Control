/**
 * @file CExpFilter.cpp
 */

/*
 * CExpFilter.cpp
 *
 *  Created on: 19 Feb 2022
 *      Author: salavat.magazov
 */

#include "CExpFilter.h"
#include <stdlib.h>
#include <limits>
#include <math.h>

/**
 * @brief Create a filter with specified order.
 *
 * @param filter_order
 */
CExpFilter::CExpFilter(float filter_order)
{
    setOrder(filter_order);
    reset();
}

void CExpFilter::setOrder(float filter_order)
{
    filter_order = abs(filter_order);
    m_coefficient = 1 - pow(10, -filter_order);
}

void CExpFilter::reset()
{
    m_last = 0;
    m_average = 0;
    m_rate = 0;
#ifdef TRACK_EXTREMA
    /* Set max to smallest number and min to largest number, so the first
     * comparison triggers automatically.
     */
    m_max = -std::numeric_limits<float>::max();
    m_min = std::numeric_limits<float>::max();
#endif
}

float CExpFilter::getAverage(float instant_value)
{
    /* if all three values are zero, chances are this is the first sample, so
     * skip calculations. */
    if ((m_last == 0) && (m_average == 0) && (m_rate == 0))
    {
        m_average = instant_value;
    }
    else
    {
        float old_average = m_average;
        m_average *= m_coefficient;
        m_average += instant_value * (1 - m_coefficient);
        m_rate = m_average - old_average;
#ifdef TRACK_EXTREMA
        if (m_max < instant_value)
        {
            m_max = instant_value;
        }
        else
        {
            m_max *= m_coefficient;
            m_max += m_average * (1 - m_coefficient);
        }
        if (instant_value < m_min)
        {
            m_min = instant_value;
        }
        else
        {
            m_min *= m_coefficient;
            m_min += m_average * (1 - m_coefficient);
        }
#endif
    }
    m_last = instant_value;
    return m_average;
}
