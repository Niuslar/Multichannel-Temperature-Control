/**
 *@file adc_data.cpp
 *@brief This files contains the methods for the CAdc class
 *
 */

/*
 *  Created on: Feb 14, 2022
 *      Author: niuslar
 */

#include "adc_data.h"

#define ADC_START_BIT   2
#define ALARM_TOGGLE_MS 300

CUartCom uart_adc(&huart2);

CLog log_adc(&uart_adc, "ADC_data");

/**
 * @brief Calibrate and Start ADC with DAM
 * @note The ADC will read all the channels once and then stop.
 * To read the channels again, use the triggerADC() method.
 * @param p_hadc Pointer to ADC handler
 * param adc_channels Number of active channels for the ADC
 * 		(This depends on the hardware configuration)
 */
CAdc::CAdc(ADC_HandleTypeDef *p_hadc, uint8_t adc_channels)
    : mp_hadc(p_hadc),
      m_adc_channels(adc_channels)
{
    // Check mp_hadc is not null
    if (!mp_hadc)
    {
        // Send error message
        log_adc.log(CLog::LOG_ERROR, "Invalid pointer to ADC Handler");
        // Hang application with a toggling ALARM Pin
        while (1)
        {
            HAL_GPIO_TogglePin(ALARM_GPIO_Port, ALARM_Pin);
            HAL_Delay(ALARM_TOGGLE_MS);
        }
    }

    // Allocate memory to store the incoming data
    mp_adc_data_buf = new uint16_t[adc_channels];

    // Calibrate ADC
    if (HAL_ADCEx_Calibration_Start(p_hadc, ADC_SINGLE_ENDED) != HAL_OK)
    {
        // Free memory and log error
        delete[] mp_adc_data_buf;
        log_adc.log(CLog::LOG_ERROR, "ADC Calibration failed");
    }

    // Start ADC with DMA
    if (HAL_ADC_Start_DMA(p_hadc, (uint32_t *)mp_adc_data_buf, adc_channels) !=
        HAL_OK)
    {
        // Free memory and log error
        delete[] mp_adc_data_buf;
        log_adc.log(CLog::LOG_ERROR, "ADC Start DMA failed");
    }
}

CAdc::~CAdc()
{
    // Free memory when the object is destroyed
    delete[] mp_adc_data_buf;
}

/**
 * @brief read data from a specific adc_channel
 * @param adc_channel Number of channel starting from 0
 */
uint16_t CAdc::readChannel(uint8_t adc_channel)
{
    // Check channel is within defined range
    if (adc_channel >= m_adc_channels)
    {
        std::string warning =
            std::to_string(adc_channel) + " is not a valid ADC Channel";
        log_adc.log(CLog::LOG_WARNING, warning);
    }

    return mp_adc_data_buf[adc_channel];
}

void CAdc::triggerAdc()
{
    // Change ADCSTART bit in the ADC control register
    mp_hadc->Instance->CR |= (1 << ADC_START_BIT);
}
