/**
 * @file CTemperatureController.h
 *
 */

/*
 * CTemperatureController.h
 *
 *  Created on: 26 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef CTEMPERATURECONTROLLER_H_
#define CTEMPERATURECONTROLLER_H_

#include "CController.h"
#include "CPIDLoop.h"
#include "IHardwareMap.h"

#define CHANNEL_NUMBER 10

class CTemperatureController : public CController
{
public:
    CTemperatureController(IHardwareMap *p_hardwaremap,
                           etl::string<MAX_STRING_SIZE> name,
                           uint32_t run_period_ms);

    virtual void run();
    virtual bool newCommand(ICommand *p_command, IComChannel *p_comchannel);
    virtual void reset();

private:
    void sendStatus(IComChannel *p_comchannel);
    ICommand::command_error_code_t setTemperature(ICommand *p_command);
    ICommand::command_error_code_t overrideHeater(ICommand *p_command);

    IHardwareMap *mp_hw;

    float m_target_temperature[CHANNEL_NUMBER];
    float m_power_override[CHANNEL_NUMBER];
    CPIDLoop m_control_loop[CHANNEL_NUMBER];
};

#endif /* CTEMPERATURECONTROLLER_H_ */
