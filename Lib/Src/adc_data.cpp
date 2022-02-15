/**
 *@file adc_data.cpp
 *@brief This files contains the methods for the CAdcData class
 *
 */

/*
 *  Created on: Feb 14, 2022
 *      Author: niuslar
 */

#include "adc_data.h"

#define ADC_START_BIT 2

CUartCom uart_adc(&huart2);

CLog log_adc(&uart_adc, "ADC_data");

/**
 * @brief Constructor
 * @param p_hadc Pointer to ADC handler
 */
CAdcData::CAdcData(ADC_HandleTypeDef *p_hadc) : mp_hadc(p_hadc)
{
    // Check mp_hadc is not null
    if (!mp_hadc)
    {
        // Send error message
        log_adc.log(CLog::LOG_ERROR, "Invalid pointer to ADC Handler");
    }
}

/**
 * @brief Calibrate and Start ADC
 * @note The ADC will read all the channels once and then stop.
 * To read the channels again, use the triggerADC() method.
 */
void CAdcData::init()
{
    // Calibrate ADC
    if (HAL_ADCEx_Calibration_Start(mp_hadc, ADC_SINGLE_ENDED) != HAL_OK)
    {
        log_adc.log(CLog::LOG_ERROR, "ADC Calibration failed");
    }

    // Start ADC with DMA
    if (HAL_ADC_Start_DMA(mp_hadc,
                          (uint32_t *)m_adc_data_buf,
                          m_adc_channels) != HAL_OK)
    {
        log_adc.log(CLog::LOG_ERROR, "ADC Start DMA failed");
    }
}

/**
 * @brief read data from a specific adc_channel
 * @param adc_channel Number of channel starting from 0
 */
uint16_t CAdcData::operator[](uint8_t adc_channel)
{
    // Check channel is within defined range
    if (adc_channel >= m_adc_channels)
    {
        std::string warning =
            std::to_string(adc_channel) + " is not a valid ADC Channel";
        log_adc.log(CLog::LOG_WARNING, warning);

        // Return 0 if it's an invalid channel
        return 0;
    }

    return m_adc_data_buf[adc_channel];
}

float CAdcData::getVolts(uint8_t adc_channel)
{
    // The [] operator checks the channel and returns 0 in case of error
    uint16_t raw_data = this->operator[](adc_channel);
    float volts = ((float)raw_data) / (ADC_RES)*ADC_VDDA;

    return volts;
}

/**
 * @brief Trigger a sequence read of all ADC Channels
 * @note This function will replace all the values in the adc_data_buf.
 */
void CAdcData::trigger()
{
    // Change ADCSTART bit in the ADC control register
    mp_hadc->Instance->CR |= (1 << ADC_START_BIT);
}
