/**
 * @file thermistor.h
 */

/*
 *  Created on: Feb 15, 2022
 *      Author: niuslar
 */

#ifndef THERMISTOR_H_
#define THERMISTOR_H_

#include "adc_data.h"

class CThermistor
{
public:
    CThermistor(CAdcData &adc_data, uint8_t adc_channel);

    // Public methods
    float getTemperature();

private:
    CAdcData &m_adc_data;
    const uint8_t m_adc_channel;
};

#endif /* THERMISTOR_H_ */
