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
CController::CController(etl::string<MAX_STRING_SIZE> name, uint32_t run_period_ms)
    : m_name(name),
      m_run_period_ms(run_period_ms),
      mb_stopped(false),
      m_previous_time(0),
      m_run_calls_counter(0)
{
}

/**
 * @brief Get the name of the controller
 *
 * @return Name of the controller
 */
etl::string<MAX_STRING_SIZE> CController::getName() const
{
    return m_name;
}

/**
 * @brief This method determines if it's time for the main loop of the
 * controller to run.
 * @note This method can be overloaded to allow controller to do something
 * simple very often. However, parent method must be called to ensure the
 * correct time keeping of the schedule. Alternatively, overloaded method must
 * perform the time keeping.
 *
 * @param current_time Current time in milliseconds.
 * @return True if it's time to execute run() method.
 */
bool CController::tick(uint32_t current_time)
{
    bool b_time_to_run = false;
    if (!mb_stopped)
    {
        if ((current_time - m_previous_time) >= m_run_period_ms)
        {
            b_time_to_run = true;
            m_previous_time = current_time;
        }
    }
    return b_time_to_run;
}

/**
 * @brief Run() method prototype.
 * @note If stats collection is required, then overloaded method must call
 * parent method or implement stats collection.
 */
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
bool CController::newCommand(etl::string<MAX_STRING_SIZE> command, IComChannel *p_comchannel)
{
    return false;
}

/**
 * @brief Stop controller execution.
 * @note Calling this method will stop run() method being called. If controller
 * is already stopped, the method will have no effect.
 */
void CController::stop()
{
    mb_stopped = true;
}

/**
 * @brief Start controller execution.
 * @note Calling this method will restart the cycle of run() method calls. The
 * run() method call timer is reset and will likely cause call to run() method
 * in the next cycle.
 */
void CController::start()
{
    mb_stopped = false;
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
 */
void CController::resetRunCalls()
{
    m_run_calls_counter = 0;
}
#endif
