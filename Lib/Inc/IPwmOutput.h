/**
 * @file IPwmOutput.h
 *
 */

/*
 * IPwmOutput.h
 *
 *  Created on: 20 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef INC_IPWMOUTPUT_H_
#define INC_IPWMOUTPUT_H_

class IPwmOutput
{
public:
    IPwmOutput();

    virtual void setDutyCycle(float duty_cycle_percent) = 0;
    virtual float getDutyCycle() const = 0;
};

#endif /* INC_IPWMOUTPUT_H_ */
