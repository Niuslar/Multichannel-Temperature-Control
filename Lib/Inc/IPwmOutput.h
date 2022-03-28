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

#ifndef IPWMOUTPUT_H_
#define IPWMOUTPUT_H_

class IPwmOutput
{
public:
    IPwmOutput(){};

    virtual void setDutyCycle(float duty_cycle_percent) = 0;
    virtual float getDutyCycle() const = 0;
};

#endif /* IPWMOUTPUT_H_ */
