/**
 * @file CGpioWrapper.h
 */
/*
 *  Created on: 14 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CGPIOWRAPPER_H_
#define CGPIOWRAPPER_H_

#include "gpio.h"

class CGpioWrapper
{
public:
    CGpioWrapper();
    CGpioWrapper(GPIO_TypeDef *p_port, uint16_t pin, bool init_state = false);

    bool init(GPIO_TypeDef *p_port, uint16_t pin);
    bool get();
    void set(bool value);
    void toggle();

private:
    GPIO_TypeDef *mp_port;
    uint16_t m_pin;
    bool mb_state;
};

#endif /* CGPIOWRAPPER_H_ */
