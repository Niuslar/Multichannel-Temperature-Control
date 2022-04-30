/**
 * @file CHeater.h
 *
 */

/*
 * CHeater.h
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef CHEATER_H_
#define CHEATER_H_

#include <CHardPwmOutput.h>
#include <CPIDLoop.h>

class CHeater : private CHardPwmOutput, private CPIDLoop
{
public:
    CHeater();
    virtual ~CHeater();

    float setTarget(float target);
    void run();

private:
    float m_target;
    float m_override_power;
};

#endif /* CHEATER_H_ */
