/**
 * @file CThermistor.cpp
 */

/*
 *  Created on: Feb 15, 2022
 *      Author: niuslar
 */

#include "CThermistor.h"
#include "math.h"

/**
 * @brief Constructor
 */
CThermistor::CThermistor() {}

/**
 * @brief replaces the default voltage range
 * @param min_voltage Minimum voltage limit
 * @param max_voltage Max voltage limit
 */
void CThermistor::setLimits(const float min_voltage, const float max_voltage)
{
    m_min_volt_limit = min_voltage;
    m_max_volt_limit = max_voltage;
}

void CThermistor::setLogger(CLog *p_uart_com)
{
    if (p_uart_com != nullptr)
    {
        mp_uart_com = p_uart_com;
    }
}

float CThermistor::getTemperature(float voltage)
{
    float temp_celsius;

    // Check the values are within range (2.67 and 0.57V)
    if (voltage > m_min_volt_limit && voltage < m_max_volt_limit)
    {
        /* Polynomial equation to convert voltage to Celsius
         * f(x) = 214 + -166x + 69.7x^2 + -13.4x^3, where x is the voltage */
        temp_celsius = 214 - 166 * voltage + 69.7 * pow(voltage, 2) -
                       13.4 * pow(voltage, 3);
    }
    else
    {
        // Log error and return unrealistic value
        if (mp_uart_com != nullptr)
        {
            mp_uart_com->log(CLog::LOG_WARNING, "Sensor fault");
        }
        temp_celsius = OUT_OF_RANGE;
    }

    return temp_celsius;
}
