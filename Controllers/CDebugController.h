/*
 * CDebugController.h
 *
 *  Created on: 18 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CDEBUGCONTROLLER_H_
#define CDEBUGCONTROLLER_H_

#include "CController.h"

class CDebugController : protected CController
{
public:
    CDebugController(std::string name, uint32_t run_period_ms);
    virtual ~CDebugController();

    virtual void run();
    virtual bool newCommand(std::string command, IComChannel *p_comchannel);
    virtual void reset() = 0;
};

#endif /* CDEBUGCONTROLLER_H_ */
