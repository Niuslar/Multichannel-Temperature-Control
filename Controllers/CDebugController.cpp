/**
 * @file CDebugController.cpp
 */

/*
 * CDebugController.cpp
 *
 *  Created on: 18 Feb 2022
 *      Author: salavat.magazov
 */

#include "CDebugController.h"
#include "../etl/to_string.h"

CDebugController::CDebugController(IHardwareMap *p_hardware_map,
                                   etl::string<MAX_STRING_SIZE> name,
                                   uint32_t run_period_ms)
    : CController(name, run_period_ms),
      mp_hw(p_hardware_map),
      m_breather_light(BREATHING_GPIO_Port,
                       BREATHING_Pin)  // TODO: convert this to HW map call
{
    // TODO Auto-generated constructor stub
}

CDebugController::~CDebugController()
{
    // TODO Auto-generated destructor stub
}

void CDebugController::run()
{
    m_breather_light.toggle();
}

bool CDebugController::newCommand(ICommand *p_command,
                                  IComChannel *p_comchannel)
{
    bool b_command_recognised = false;
    ICommand::command_error_code_t error_code;
    /**
     * Set mains power output.
     *
     * mains(channel, power);
     */
    if (p_command->getName()->compare("mains") == 0)
    {
        error_code = setMainsPower(p_command);
        b_command_recognised = true;
    }
    else if (p_command->getName()->compare("?mains") == 0)
    {
        float mains_power;
        error_code = getMainsPower(p_command, &mains_power);
        if (error_code == ICommand::COMMAND_OK)
        {
                    	CController::s_scratch_pad = "Mains channel ";
                        etl::to_string((*p_command)[0],
                                       CController::s_scratch_pad,
                                       etl::format_spec(),
                                       true);
                        CController::s_scratch_pad += ": ";
                        etl::to_string(mains_power,
                                       CController::s_scratch_pad,
                                       etl::format_spec().precision(2),
                                       true);
                        p_comchannel->send(CController::s_scratch_pad);
        }
        b_command_recognised = true;
    }
    if (b_command_recognised)
    {
        sendResultMessage(error_code, p_comchannel);
    }
    return (b_command_recognised);
}

void CDebugController::reset() {}

ICommand::command_error_code_t CDebugController::setMainsPower(
    ICommand *p_command)
{
    uint8_t mains_channel;
    float mains_power;
    // extract arguments from command.
    if (p_command->getArgumentCount() == 1)
    {
        mains_channel = 0;
        mains_power = (*p_command)[0];
    }
    else if (p_command->getArgumentCount() == 2)
    {
        mains_channel = (uint8_t)(*p_command)[0];
        if (mains_channel != (*p_command)[0])
        {
            return (ICommand::ERROR_TYPE_MISMATCH);
        }
        mains_power = (*p_command)[1];
    }
    // range check arguments.
    if ((mains_channel > 2) || (mains_power < 0) || (mains_power > 100))
    {
        return (ICommand::ERROR_OUT_OF_BOUNDS);
    }
    // set channels to requested power
    if (mains_channel)
    {
        mp_hw->setMainsPwm(mains_channel, mains_power);
    }
    else
    {
        mp_hw->setMainsPwm(1, mains_power);
        mp_hw->setMainsPwm(2, mains_power);
    }
    return (ICommand::COMMAND_OK);
}

ICommand::command_error_code_t CDebugController::getMainsPower(
    ICommand *p_command,
    float *power)
{
    *power = 0;
    if (p_command->getArgumentCount() != 1)
    {
        return (ICommand::ERROR_ARG_COUNT);
    }
    else if (((*p_command)[0] != 1) && ((*p_command)[0] != 2))
    {
        return (ICommand::ERROR_OUT_OF_BOUNDS);
    }
    else
    {
        uint8_t channel = (*p_command)[0];
        *power = m_mains_power[channel];
    }
    return (ICommand::COMMAND_OK);
}
