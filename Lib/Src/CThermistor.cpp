/**
 * @file CThermistor.cpp
 */

/*
 *  Created on: Feb 15, 2022
 *      Author: niuslar
 */

#include "CThermistor.h"

constexpr float CThermistor::s_default_coeff[] = {214, -166, 69.7, -13.4};
constexpr uint8_t CThermistor::s_default_order = 3;

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
CThermistor::CThermistor(float *p_calibration_coeff, uint8_t calibration_order)
    : m_calibration_order(calibration_order)
{
    float const *p_coeff;

    // Check if we need to use default values
    if ((p_calibration_coeff == nullptr) || (calibration_order == 0))
    {
        p_coeff = s_default_coeff;
        m_calibration_order = s_default_order;
    }

    else
    {
        // Check pointer
        if (p_calibration_coeff == nullptr)
        {
            Error_Handler();
        }

        p_coeff = p_calibration_coeff;

        // Check order
        if (calibration_order > MAX_ORDER)
        {
            Error_Handler();
        }
    }

    for (int i = 0; i <= m_calibration_order; i++)
    {
        m_calibration_coeff[i] = p_coeff[i];
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

float CThermistor::getTemperature(float voltage) const
{
    float temp_celsius = m_calibration_coeff[0];

    // Check the values are within range
    if (voltage > m_min_volt_limit && voltage < m_max_volt_limit)
    {
        float original_voltage = voltage;
        // Convert voltage to temperature
        for (uint8_t i = 1; i <= m_calibration_order; i++)
        {
            temp_celsius += voltage * m_calibration_coeff[i];
            voltage *= original_voltage;
        }
    }

    else
    {
        // Return unrealistic value
        temp_celsius = OUT_OF_RANGE;
    }

    return temp_celsius;
}

void CThermistor::setCalibration(float *p_calibration_coeff,
                                 uint8_t calibration_order)
{
    // Check input
    if ((p_calibration_coeff == nullptr) || (calibration_order > MAX_ORDER) ||
        (calibration_order == 0))
    {
        return;
    }

    // TODO: see if we need to check this loop won't try to access elements past
    // the array
    // e.g. p_calibration_coeff points to array of size 3 and calibration_order
    // is 3.
    for (uint8_ = 0; i <= calibration_order; i++)
    {
        m_calibration_coeff[i] = p_calibration_coeff[i];
    }
}
