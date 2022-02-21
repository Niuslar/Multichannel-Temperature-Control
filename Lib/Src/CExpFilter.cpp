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
#include <math.h>
#include <stdlib.h>
#include <limits>

/**
 * @brief Create a filter with specified order.
 *
 * @param filter_order Power of the filter.
 * @note The filter coefficient is calculated as
 * 1 - 10^(-filter_order). Example
 * filter_order = 1, filter coefficient = 0.9
 * filter_order = 2, filter coefficient = 0.99
 * filter_order = 5, filter coefficient = 0.99999
 * Essentially the higher the filter order the "slower" the filter becomes. The
 * response of the filter also depends on frequency with which new data is being
 * fed to it. Ideally, you want reasonably high frequency of fresh samples, but
 * that brings higher overhead of calculating new average values.
 */
CExpFilter::CExpFilter(float filter_order)
{
    setOrder(filter_order);
    reset();
}

/**
 * @brief Set the order of the filter after construction.
 * @note The change will not affect past samples, but will take immediate effect
 * from next sample forward.
 *
 * @param filter_order New filter order. See ctor notes for detailed
 * explanation.
 */
void CExpFilter::setOrder(float filter_order)
{
    filter_order = abs(filter_order);
    m_coefficient = 1 - pow(10, -filter_order);
}

/**
 * @brief Reset the filter to freshly constructed state.
 * @note The reset is not going to affect filter order.
 */
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
#ifdef TRACK_RMS
    m_rms = 0;
#endif
}

/**
 * @brief Calculate new average and return it.
 *
 * @param instant_value New instant value.
 * @return Newly calculated running average.
 */
float CExpFilter::getAverage(float instant_value)
{
    /* if all three values are zero, chances are this is the first sample, so
     * skip calculations and initialise filter with first sample. */
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
#ifdef TRACK_RMS
        m_rms *= m_coefficient;
        m_rms += pow(m_average - instant_value, 2) * (1 - m_coefficient);
#endif
    }
    m_last = instant_value;
    return m_average;
}
