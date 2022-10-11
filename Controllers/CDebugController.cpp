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
    return CController::newCommand(p_command,
                                   p_comchannel);  // todo: temporary plug
}

void CDebugController::reset() {}
