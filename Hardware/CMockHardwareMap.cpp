/**
 * @file CMockHardwareMap.cpp
 *
 */

/*
 * CMockHardwareMap.cpp
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#include "CMockHardwareMap.h"

CMockHardwareMap::CMockHardwareMap()
{
    // TODO Auto-generated constructor stub
}

void CMockHardwareMap::init()
{
    /* all mock initialisation of mock hardware layer goes here. */
}

float getInputVoltage() const
{
    return 24.0;
}

float getTotalCurrent() const
{
    // TODO: model current consumption based control current + some for digital
    // logic.
    return getControlCurrent() + 0.1;
}

float getControlCurrent() const
{
    // TODO: model current consumption based on number of heaters powered on.
}

float getAmbientTemp() const
{
    // TODO: consider modelling this.
    return 21.0;
}

float getChanneTemp(uint8_t channel) const
{
    return getAmbientTemp();
}

float setHardPwmOutput(float power, uint8_t channel)
{
    // TODO: This sets power parameters for the model.
    return 0;
}

float getHardPwmOutput(uint8_t channel)
{
    // TODO: this just returns what's been recorded for heater power
    return 0;
}

#ifdef SOFT_PWM_OUTPUTS
float setSoftPwmOutput(float power, uint8_t channel) = 0;
float getSoftPwmOutput(uint8_t channel) = 0;
#endif

void setBreathingLight(float duty_cycle);

void enableControlPower(bool b_enable)
{
    // todo: this should probably participate in the model of control current
    // flow and temperature control.
}

