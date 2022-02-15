/**
 * @file CThermistor.cpp
 */

/*
 *  Created on: Feb 15, 2022
 *      Author: niuslar
 */

#include "CThermistor.h"
#include "math.h"

#define MIN_VOLT_RANGE     (0.57)
#define MAX_VOLT_RANGE     (2.67)
#define OUT_OF_RANGE_VALUE (999.9)

CUartCom uart_temp(&huart2);
CLog log_temp(&uart_temp, "Temp_sensor");

/**
 * @brief Constructor
 * @param adc_data ADC Data object
 * @param adc_channel starting from 0
 */
CThermistor::CThermistor(CAdcData &adc_data, uint8_t adc_channel)
    : m_adc_data(adc_data),
      m_adc_channel(adc_channel)
{
    // Check channel
    if (m_adc_channel >= ADC_CHANNELS)
    {
        // Log warning
        log_temp.log(CLog::LOG_WARNING, "Invalid ADC Channel");
    }
}

float CThermistor::getTemperature()
{
    float volts = m_adc_data[m_adc_channel];

    float temp_celsius;

    // Check the values are within range (2.67 and 0.57V)
    if (volts > MIN_VOLT_RANGE && volts < MAX_VOLT_RANGE)
    {
        /* Polynomial equation to convert voltage to Celsius
         * f(x) = 214 + -166x + 69.7x^2 + -13.4x^3, where x is the voltage */
        temp_celsius =
            214 - 166 * volts + 69.7 * pow(volts, 2) - 13.4 * pow(volts, 3);
    }
    else
    {
        // Log error and return unrealistic value
        log_temp.log(CLog::LOG_WARNING, "Temperature outside operation range");
        temp_celsius = OUT_OF_RANGE_VALUE;
    }

    return temp_celsius;
}
