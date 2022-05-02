/**
 * @file CMockHardwareMap.h
 *
 */

/*
 * CMockHardwareMap.h
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef CMOCKHARDWAREMAP_H_
#define CMOCKHARDWAREMAP_H_

#include "IHardwareMap.h"

class CMockHardwareMap : public IHardwareMap
{
public:
    CMockHardwareMap();

    virtual void init();

    virtual float getInputVoltage() const;
    virtual float getTotalCurrent() const;
    virtual float getControlCurrent() const;
    virtual float getAmbientTemp() const;
    virtual float getChanneTemp(uint8_t channel) const;

    virtual float setHardPwmOutput(float power, uint8_t channel);
    virtual float getHardPwmOutput(uint8_t channel);

#ifdef SOFT_PWM_OUTPUTS
    virtual float setSoftPwmOutput(float power, uint8_t channel) = 0;
    virtual float getSoftPwmOutput(uint8_t channel) = 0;
#endif

    virtual void setBreathingLight(float duty_cycle);

    virtual void enableControlPower(bool b_enable);

};

#endif /* CMOCKHARDWAREMAP_H_ */
