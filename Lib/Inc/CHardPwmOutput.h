/**
 * @file CHardPwmOutput.h
 *
 */

/*
 * CHardPwmOutput.h
 *
 *  Created on: 20 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CHARDPWMOUTPUT_H_
#define CHARDPWMOUTPUT_H_

#include "IPwmOutput.h"
#include "main.h"

class CHardPwmOutput : public IPwmOutput
{
public:
    CHardPwmOutput();

    void init(TIM_HandleTypeDef *p_timer, uint8_t channel);
    TIM_HandleTypeDef *getTimer() const
    {
        return mp_timer;
    };
    uint8_t getChannel() const
    {
        return m_channel;
    };

    virtual void setDutyCycle(float duty_cycle_percent);
    virtual float getDutyCycle() const;

private:
    TIM_HandleTypeDef *mp_timer;
    uint8_t m_channel;
    bool mb_initialised;
    bool mb_stopped;
    float m_output;
};

#endif /* CHARDPWMOUTPUT_H_ */
