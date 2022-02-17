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

/**
 * @brief Create controller base class.
 *
 * @param p_name Name of the controller
 * @param run_period_ms How often should run() method be called.
 */
CController::CController(std::string name, uint32_t run_period_ms)
    : m_name(name),
      m_run_period_ms(run_period_ms),
      m_previous_time(0),
      m_run_calls_counter(0)
{
}

CController::~CController()
{
    // TODO Auto-generated destructor stub
}

/**
 * @brief Get the name of the controller
 *
 * @return Name of the controller
 */
std::string CController::getName() const
{
    return m_name;
}

/**
 * @brief This method determines if it's time for the main loop of the
 * controller to run.
 *
 * @param current_time Current time in milliseconds.
 * @return True if it's time to execute run() method.
 */
bool CController::tick(uint32_t current_time)
{
    bool b_time_to_run = false;
    if ((current_time - m_previous_time) >= m_run_period_ms)
    {
        b_time_to_run = true;
    }
    return b_time_to_run;
}

void CController::run()
{
#ifdef COLLECT_STATS
    m_run_calls_counter++;
#endif
    return;
}

/**
 * @brief Give a new command to controller.
 *
 * @param p_command Pointer to NULL terminated command string.
 * @return True if command was recognised.
 */
bool CController::newCommand(std::string command)
{
    return false;
}

#ifdef COLLECT_STATS
/**
 * @brief Get number of calls to run() method.
 *
 * @return Number of calls to run() method since last reset.
 */
uint32_t CController::getRunCalls() const
{
    return m_run_calls_counter;
}

/**
 * @brief Reset number of calls to run() method.
 *
 */
void CController::resetRunCalls()
{
    m_run_calls_counter = 0;
}
#endif