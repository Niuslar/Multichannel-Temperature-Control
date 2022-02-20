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
    IPwmOutput();

    virtual uint8_t set(uint8_t output) = 0;
    virtual uint8_t get() const = 0;
};

#endif /* INC_IPWMOUTPUT_H_ */
