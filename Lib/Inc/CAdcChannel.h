/**
 * @file CAdcChannel.h
 */

/*
 *  Created on: Feb 14, 2022
 *      Author: niuslar
 */

#ifndef CADCCHANNEL_H_
#define CADCCHANNEL_H_

#include "adc.h"

/**
 *  @brief Number of active ADC Channels,
 *   change this according to hardware
 */
#define ADC_CHANNELS 14

/** ADC_RES should change depending on the resolution selected (4096 for
 * 12-bits) */
#define ADC_RES  4096
#define ADC_VDDA (3.3)

class CAdcChannel
{
public:
    CAdcChannel(uint8_t adc_channel);

    static void startADC(ADC_HandleTypeDef *p_hadc);
    static void trigger();

    // Public methods
    float getVolts();

    // Static members
    static bool s_adc_initialised;
    static ADC_HandleTypeDef *sp_hadc;
    static uint16_t s_adc_data_buf[ADC_CHANNELS];

private:
    uint8_t m_adc_channel;
};

#endif /* CADCCHANNEL_H_ */
