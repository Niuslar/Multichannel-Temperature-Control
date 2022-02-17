/**
 * @file CThermistor.h
 */

/*
 *  Created on: Feb 15, 2022
 *      Author: niuslar
 */

#ifndef CTHERMISTOR_H_
#define CTHERMISTOR_H_

#include "CLog.h"

// Default Voltage Range
// Can be changed using the setLimits method
#define MIN_VOLT_RANGE (0.57)
#define MAX_VOLT_RANGE (2.67)
#define MAX_ORDER      5

class CThermistor
{
public:
    // Constructor
    CThermistor(const float *p_calibration_coeff = s_calibration_coeff,
                const uint8_t calibration_order = s_calibration_order);

    // Constructor with logger
    CThermistor(CLog *p_logger,
                const float *p_calibration_coeff = s_calibration_coeff,
                const uint8_t calibration_order = s_calibration_order);

    // Public methods
    float getTemperature(float voltage) const;
    void setLimits(float min_voltage, float max_voltage);

    // Public variables
    static constexpr float OUT_OF_RANGE = 999.9;
    static const float s_calibration_coeff[];
    static const uint8_t s_calibration_order;

private:
    float m_min_volt_limit = MIN_VOLT_RANGE;
    float m_max_volt_limit = MAX_VOLT_RANGE;
    const float *mp_calibration_coeff;
    const uint8_t m_calibration_order;
    CLog *mp_logger = nullptr;
};

#endif /* CTHERMISTOR_H_ */
