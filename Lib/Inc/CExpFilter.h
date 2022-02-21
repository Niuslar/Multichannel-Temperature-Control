/**
 * @file CExpFilter.h
 */

/*
 * CExpFilter.h
 *
 *  Created on: 19 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CEXPFILTER_H_
#define CEXPFILTER_H_

// comment out if there is no need to track running extrema of the sequence.
#define TRACK_EXTREMA
// comment out if there is no need to track noise power. This is more demanding
// on clock cycles due to a lot of multiplications.
#define TRACK_RMS

class CExpFilter
{
public:
    CExpFilter(float filter_order);

    void setOrder(float filter_order);
    void reset();
    /**
     * @brief Get last instant value.
     *
     * @return Last instant value.
     */
    float getLast() const
    {
        return m_last;
    };

    /**
     * @brief Get current running average.
     *
     * @return Current running average.
     */
    float getAverage() const
    {
        return m_average;
    };

    float getAverage(float instant_value);

    /**
     * @brief Get instant rate of change of filtered value.
     * @note This filter is not very good at providing good average rate of
     * change estimate. If you want good rate of change, then window average
     * filter is much better.
     *
     * @return Difference between current and previous average value.
     */
    float getRate() const
    {
        return m_rate;
    };
#ifdef TRACK_EXTREMA
    /**
     * @brief Get running minimum of the instant value.
     *
     * @return Current value of the running minimum.
     */
    float getMin() const
    {
        return m_min;
    };

    /**
     * @brief Get running maximum of the instant value.
     *
     * @return Current value of the running maximum.
     */
    float getMax() const
    {
        return m_max;
    };
#endif
#ifdef TRACK_RMS
    /**
     * @brief Get running root mean square.
     * @note This is not a true RMS, but a running approximation. It is close
     * enough for everyday use.
     *
     * @return Current RMS value.
     */
    float getRms() const
    {
        return m_rms;
    }
#endif

private:
    float m_coefficient;
    float m_last;
    float m_average;
    float m_rate;
#ifdef TRACK_EXTREMA
    float m_max;
    float m_min;
#endif
#ifdef TRACK_RMS
    float m_rms;
#endif
};

#endif /* CEXPFILTER_H_ */
