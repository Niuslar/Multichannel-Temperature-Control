/**
 * @file CRealHardwareMap.cpp
 *
 */

/*
 * CRealHardwareMap.cpp
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#include "CRealHardwareMap.h"

/**
 * @note All of these parameters are subject to PCB design layout. Refer to
 * specific PCB this code will run on for correct information on what
 * peripherals are being used.
 */
#define INPUT_VOLTAGE_CHANNEL   10
#define TOTAL_CURRENT_CHANNEL   9
#define CONTROL_CURRENT_CHANNEL 8
#define AMBIENT_TEMP_CHANNEL    7

#define INPUT_VOLTAGE_SCALE   10
#define TOTAL_CURRENT_SCALE   3
#define CONTROL_CURRENT_SCALE 3
#define AD22100_SCALE         1
#define AD22100_OFFSET        1

CRealHardwareMap::CRealHardwareMap() : m_adc(&hadc1)
{
    // TODO Auto-generated constructor stub
}

void CRealHardwareMap::init()
{
    /* all hardware initialisation of peripherals goes here. */
    m_adc.init();
}

float CRealHardwareMap::getInputVoltage() const
{
    return m_adc[INPUT_VOLTAGE_CHANNEL] * INPUT_VOLTAGE_SCALE;
}

float CRealHardwareMap::getTotalCurrent() const
{
    return m_adc[TOTAL_CURRENT_CHANNEL] * TOTAL_CURRENT_SCALE;
}

float CRealHardwareMap::getControlCurrent() const
{
    return m_adc[CONTROL_CURRENT_CHANNEL] * CONTROL_CURRENT_SCALE;
}
float CRealHardwareMap::getAmbientTemp() const
{
    float temperature = m_adc[AMBIENT_TEMP_CHANNEL];
    temperature *= AD22100_SCALE;
    temperature += AD22100_OFFSET;
    return temperature;
}

float CRealHardwareMap::getChanneTemp(uint8_t channel) const
{
    float temperature = m_adc[channel];
    // TODO: calibration for thermistor goes here.
    return temperature;
}
