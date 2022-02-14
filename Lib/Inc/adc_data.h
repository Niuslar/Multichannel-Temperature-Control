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

/**
 *  @brief Number of active ADC Channels,
 *   change this according to hardware
 */
#define ADC_CHANNELS 14

class CAdcData
{
public:
    CAdcData(ADC_HandleTypeDef *p_hadc);

    // Public methods
    void init();
    // use [] operator to read a particular ADC Channel
    uint16_t operator[](uint8_t adc_channel);
    void triggerRead();

private:
    ADC_HandleTypeDef *const mp_hadc;
    const uint8_t m_adc_channels = ADC_CHANNELS;
    uint16_t m_adc_data_buf[ADC_CHANNELS];
};

#endif /* ADC_DATA_H_ */
