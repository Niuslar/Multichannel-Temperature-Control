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

class CExpFilter
{
public:
    CExpFilter(float filter_order);

    void setOrder(float filter_order);
    void reset();
    float getLast() const
    {
        return m_last;
    };
    float getAverage() const
    {
        return m_average;
    };
    float getAverage(float instant_value);

    float getRate() const
    {
        return m_rate;
    };
#ifdef TRACK_EXTREMA
    float getMin() const
    {
        return m_min;
    };
    float getMax() const
    {
        return m_max;
    };
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
};

#endif /* CEXPFILTER_H_ */
