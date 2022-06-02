/**
 * @file CDebugController.h
 */

/*
 * CDebugController.h
 *
 *  Created on: 18 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CDEBUGCONTROLLER_H_
#define CDEBUGCONTROLLER_H_

#include "../etl/string.h"
#include "CController.h"
#include "CGpioWrapper.h"
#include "main.h"

#define MAX_STRING_SIZE 60

class CDebugController : public CController
{
public:
    CDebugController(etl::string<MAX_STRING_SIZE> name, uint32_t run_period_ms);
    virtual ~CDebugController();

    virtual void run();
    virtual bool newCommand(etl::string<MAX_STRING_SIZE> command,
                            IComChannel *p_comchannel);
    virtual void reset();

private:
    CGpioWrapper m_breather_light;
};

#endif /* CDEBUGCONTROLLER_H_ */
