/**
 * @file IHardwareMap.h
 *
 */

/*
 * IHardwareMap.h
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef IHARDWAREMAP_H_
#define IHARDWAREMAP_H_

#include "stdint.h"

#define HARD_PWM_OUTPUTS 10

class IHardwareMap
{
public:
    virtual void init() = 0;

    /**
     * @note Any type of access that is required by the controllers to the
     * underlying peripherals must be done via this interface class.
     * If access is required, then the procedure is to create a virtual method
     * here, then add corresponding implementations in all child classes.
     */

    /* ADC access */
    virtual float getInputVoltage() const = 0;
    virtual float getTotalCurrent() const = 0;
    virtual float getControlCurrent() const = 0;
    virtual float getAmbientTemp() const = 0;
    virtual float getChanneTemp(uint8_t channel) const = 0;

    /* PWM control */
    virtual float setPwmOutput(float power, uint8_t channel) = 0;
};

#endif /* IHARDWAREMAP_H_ */
