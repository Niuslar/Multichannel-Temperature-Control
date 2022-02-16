/**
 * @file CThermistor.cpp
 */

/*
 *  Created on: Feb 15, 2022
 *      Author: niuslar
 */

#include "CThermistor.h"

constexpr float CThermistor::s_calibration_coeff[] = {214, -166, 69.7, -13.4};
constexpr uint8_t CThermistor::s_calibration_order = 3;

/**
 * @brief Constructor
 * @param p_calibration_coeff Pointer to array with polynomial coefficients
 * @param calibration_order Order of the polynomial equation
 *
 * @note The order of the polynomial is used in a strict mathematical sense,
 i.e. third order polynomial will have 4 coefficients: a0, a1, a2, a3. The
 minimum number of coefficients required is 2, which would make for a first
 order polynomial otherwise knows as linear relation.
 */
CThermistor::CThermistor(const float *p_calibration_coeff,
                         const uint8_t calibration_order)
    : mp_calibration_coeff(p_calibration_coeff),
      m_calibration_order(calibration_order)
{
    // Check pointer
    if (mp_calibration_coeff == nullptr)
    {
        if (mp_uart_com != nullptr)
        {
            mp_uart_com->log(CLog::LOG_ERROR, "Null pointer");
        }

        Error_Handler();
    }

    // Check order
    if (m_calibration_order > MAX_ORDER || m_calibration_order < 1)
    {
        if (mp_uart_com != nullptr)
        {
            mp_uart_com->log(CLog::LOG_ERROR, "Invalid calibration order");
        }

        Error_Handler();
    }
}

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

float CThermistor::getTemperature(float voltage) const
{
    float temp_celsius = mp_calibration_coeff[0];

    // Check the values are within range
    if (voltage > m_min_volt_limit && voltage < m_max_volt_limit)
    {
        float original_voltage = voltage;
        // Convert voltage to temperature
        for (uint8_t i = 1; i <= m_calibration_order; i++)
        {
            temp_celsius += voltage * mp_calibration_coeff[i];
            voltage *= original_voltage;
        }
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
