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
#include "IHardwareMap.h"
#include "main.h"

class CDebugController : public CController
{
public:
    CDebugController(IHardwareMap *p_hardware_map,
                     etl::string<MAX_STRING_SIZE> name,
                     uint32_t run_period_ms);
    virtual ~CDebugController();

    virtual void run();
    virtual bool newCommand(ICommand *p_command, IComChannel *p_comchannel);
    virtual void reset();

private:
    ICommand::command_error_code_t setMainsPower(ICommand *p_command);

    ICommand::command_error_code_t getMainsPower(ICommand *p_command,
                                                 float *power);

    IHardwareMap *mp_hw;
    float m_mains_power[2];
};

#endif /* CDEBUGCONTROLLER_H_ */
