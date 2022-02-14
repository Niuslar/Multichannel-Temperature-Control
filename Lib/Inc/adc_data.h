/**
 * @file adc_data.h
 */

/*
 *  Created on: Feb 14, 2022
 *      Author: niuslar
 */

#ifndef ADC_DATA_H_
#define ADC_DATA_H_

#include "adc.h"
#include "log.h"

class CAdc
{
public:
    CAdc(ADC_HandleTypeDef *p_hadc, uint8_t adc_channels);
    ~CAdc();

    // Public methods
    uint16_t readChannel(uint8_t adc_channel);
    void triggerAdc();

private:
    ADC_HandleTypeDef *mp_hadc;
    uint8_t m_adc_channels;
    uint16_t *mp_adc_data_buf;
};

#endif /* ADC_DATA_H_ */
