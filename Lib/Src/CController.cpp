/**
 * @file CController.cpp
 *
 */

/*
 * CController.cpp
 *
 *  Created on: 16 Feb 2022
 *      Author: salavat.magazov
 */

#include "CController.h"

CController::CController(char *p_name, uint32_t run_period_ms)
    : m_run_period_ms(run_period_ms),
      m_previous_tick(0)
{
    strncpy(m_name, p_name, MAX_NAME_SIZE);
    m_name[MAX_NAME_SIZE - 1] = 0;
}

CController::~CController()
{
    // TODO Auto-generated destructor stub
}

bool CController::tick(uint32_t current_time)
{
    bool b_time_to_run = false;
    if ((current_tick - m_previous_tick) >= m_run_period_ms)
    {
        b_time_to_run = true;
    }
    return b_time_to_run;
}

bool CController::newCommand(char *p_command)
{
    return false;
}
