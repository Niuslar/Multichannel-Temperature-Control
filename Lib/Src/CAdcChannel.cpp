/**
 *@file CAdcChannel.cpp
 *@brief This files contains the methods for the CAdcChannel class
 *
 */

/*
 *  Created on: Feb 14, 2022
 *      Author: niuslar
 */

#include "CAdcChannel.h"

#define ADC_CONVERSION_COEFF ((float)ADC_VDDA / ADC_RES)

bool CAdcChannel::s_adc_initialised = false;
ADC_HandleTypeDef *CAdcChannel::sp_hadc = nullptr;
uint16_t CAdcChannel::s_adc_data_buf[ADC_CHANNELS];

/**
 * @brief Constructor
 * @param adc channel
 */
CAdcChannel::CAdcChannel(uint8_t adc_channel) : m_adc_channel(adc_channel)
{
    if (adc_channel >= ADC_CHANNELS)
    {
        Error_Handler();
    }
}

/**
 * @brief Calibrate and Start ADC
 * @note The ADC will read all the channels once and then stop.
 * To read the channels again, use the triggerADC() method.
 * @note This function needs to be called only once.
 */
void CAdcChannel::startADC(ADC_HandleTypeDef *p_hadc)
{
    if (!CAdcChannel::s_adc_initialised)
    {
        CAdcChannel::sp_hadc = p_hadc;

        // Calibrate ADC
        if (HAL_ADCEx_Calibration_Start(CAdcChannel::sp_hadc,
                                        ADC_SINGLE_ENDED) != HAL_OK)
        {
            Error_Handler();
        }

        // Start ADC with DMA
        if (HAL_ADC_Start_DMA(CAdcChannel::sp_hadc,
                              (uint32_t *)CAdcChannel::s_adc_data_buf,
                              ADC_CHANNELS) != HAL_OK)
        {
            Error_Handler();
        }

        CAdcChannel::s_adc_initialised = true;
    }
}

/**
 * @brief read data converted to volts for a specific adc_channel
 * @retval Volts
 */
float CAdcChannel::getVolts()
{
    float volts = ((float)CAdcChannel::s_adc_data_buf[m_adc_channel]) *
                  ADC_CONVERSION_COEFF;

    return volts;
}

/**
 * @brief Trigger a sequence read of all ADC Channels
 * @note This function will replace all the values in the adc_data_buf.
 */
void CAdcChannel::trigger()
{
    // Change ADCSTART bit in the ADC control register
    CAdcChannel::sp_hadc->Instance->CR |= ADC_CR_ADSTART;
}
