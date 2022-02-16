/**
 * @file CThermistor.h
 */

/*
 *  Created on: Feb 15, 2022
 *      Author: niuslar
 */

#ifndef CTHERMISTOR_H_
#define CTHERMISTOR_H_

#include "CAdcData.h"

// Default Voltage Range
// Can be changed using the setLimits method
#define MIN_VOLT_RANGE (0.57)
#define MAX_VOLT_RANGE (2.67)

class CThermistor
{
public:
    CThermistor();

    // Public methods
    float getTemperature(float voltage);
    void setLimits(float min_voltage, float max_voltage);

private:
    float m_min_volt_limit = MIN_VOLT_RANGE;
    float m_max_volt_limit = MAX_VOLT_RANGE;
};

#endif /* CTHERMISTOR_H_ */
