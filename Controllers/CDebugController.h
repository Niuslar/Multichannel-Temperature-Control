/*
 * CDebugController.h
 *
 *  Created on: 18 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CDEBUGCONTROLLER_H_
#define CDEBUGCONTROLLER_H_

#include "CController.h"
#include "CGpioWrapper.h"
#include "main.h"

class CDebugController : public CController
{
public:
    CDebugController(std::string name, uint32_t run_period_ms);
    virtual ~CDebugController();

    virtual void run();
    virtual bool newCommand(std::string command, IComChannel *p_comchannel);
    virtual void reset();

private:
    CGpioWrapper m_breather_light;
};

#endif /* CDEBUGCONTROLLER_H_ */
