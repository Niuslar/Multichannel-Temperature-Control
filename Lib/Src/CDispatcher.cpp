/*
 * CDispatcher.cpp
 *
 *  Created on: 16 Feb 2022
 *      Author: salavat.magazov
 */

#include "CDispatcher.h"

CDispatcher::CDispatcher(CLog *p_logger)
    : mp_logger(p_logger),
      m_controller_count(0),
      m_comchannel_count(0)
{
    // TODO Auto-generated constructor stub
}

CDispatcher::~CDispatcher()
{
    // TODO Auto-generated destructor stub
}

bool CDispatcher::registerController(CController *p_controller)
{
    bool success = false;
    if (p_controller == nullptr)
    {
        mp_logger->log(CLog::LOG_ERROR, "Invalid controller pointer.");
        Error_Handler();
    }
    else if (m_controller_count >= MAX_CONTROLLERS)
    {
        std::string message;
        message = "Controller " + p_controller->getName() +
                  " could not be registered with dispatcher.";
        mp_logger->log(CLog::LOG_WARNING, message);
        Error_Handler();
    }
    else
    {
        mp_controllers[m_controller_count] = p_controller;
        m_controller_count++;
        success = true;
    }
    return success;
}

bool CDispatcher::registerComChannel(IComChannel *p_comchannel)
{
    bool success = false;
    if (p_comchannel == nullptr)
    {
        mp_logger->log(CLog::LOG_ERROR, "Invalid coms channel pointer.");
        Error_Handler();
    }
    else if (m_comchannel_count >= MAX_COMCHANNELS)
    {
        std::string message;
        message = "Coms channel " + p_comchannel->getName() +
                  " could not be registered with dispatcher.";
        mp_logger->log(CLog::LOG_WARNING, message);
        Error_Handler();
    }
    else
    {
        mp_comchannels[m_comchannel_count] = p_comchannel;
        m_comchannel_count++;
        success = true;
    }
    return success;
}
