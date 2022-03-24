/**
 * @file CAdcData.h
 */

/*
 *  Created on: Feb 14, 2022
 *      Author: niuslar
 */

#ifndef CADCDATA_H_
#define CADCDATA_H_

#include "CLog.h"
#include "adc.h"

/**
 *  @brief Number of active ADC Channels,
 *   change this according to hardware
 *  @Note: This does not set the number of channels,
 *  	   and it should match the number set in
 *  	   hadc.Init.NbrOfConversions
 */
#define ADC_CHANNELS 14

/** ADC_RES should change depending on the resolution selected (4096 for
 * 12-bits) */
#define ADC_RES  4096
#define ADC_VDDA (3.3)

class CAdcData
{
public:
    CAdcData(ADC_HandleTypeDef *p_hadc);

    // Public methods
    void init();
    // use [] operator to read voltage in a particular ADC Channel
    float operator[](uint8_t adc_channel);
    void trigger();

private:
    ADC_HandleTypeDef *const mp_hadc;
    const uint8_t m_adc_channels = ADC_CHANNELS;
    uint16_t m_adc_data_buf[ADC_CHANNELS];
};

#endif /* CADCDATA_H_ */
