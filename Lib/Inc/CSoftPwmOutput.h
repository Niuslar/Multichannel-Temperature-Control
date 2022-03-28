/**
 * @file CSoftPwmOutput.h
 *
 */

/*
 * CSoftPwmOutput.h
 *
 *  Created on: 20 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CSOFTPWMOUTPUT_H_
#define CSOFTPWMOUTPUT_H_

#include "CGpioWrapper.h"
#include "IPwmOutput.h"
#include "main.h"

class CSoftPwmOutput : public IPwmOutput, private CGpioWrapper
{
public:
    CSoftPwmOutput(GPIO_TypeDef *p_port, uint16_t pin);

    virtual void setDutyCycle(float duty_cycle_percent);
    virtual float getDutyCycle() const;

    static void init(uint32_t period, TIM_HandleTypeDef *p_timer = nullptr);
    static void tick();
    static TIM_HandleTypeDef *const getTimer();

private:
    static CSoftPwmOutput *getLastInstance();

    static TIM_HandleTypeDef *sp_timer;
    static uint32_t s_period;
    static uint32_t s_counter;
    static CSoftPwmOutput *sp_first_instance;

    CSoftPwmOutput *mp_next_instance;
    uint32_t m_duty_cycle;
};

extern "C"
{
    void softPwmTimerTick(TIM_HandleTypeDef *htim);
}

#endif /* CSOFTPWMOUTPUT_H_ */
