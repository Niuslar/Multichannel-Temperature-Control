/**
 * @file CGpioWrapper.h
 *
 *  Created on: 14 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CGPIOWRAPPER_H_
#define CGPIOWRAPPER_H_

#include "gpio.h"

class CGpioWrapper
{
public:
    CGpioWrapper(GPIO_TypeDef *p_port, uint16_t pin);
    virtual ~CGpioWrapper();

    bool get();
    void set(bool value);
    void toggle();

private:
    GPIO_TypeDef *const mp_port;
    uint16_t const m_pin;
};

#endif /* CGPIOWRAPPER_H_ */
