/**
 * @file CThermistor.h
 */

/*
 *  Created on: Feb 15, 2022
 *      Author: niuslar
 */

#ifndef CTHERMISTOR_H_
#define CTHERMISTOR_H_

#include "CAdcData.h"

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

#endif /* CTHERMISTOR_H_ */
