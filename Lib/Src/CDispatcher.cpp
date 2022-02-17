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
        Error_Handler();
    }
    if (m_controller_count >= MAX_CONTROLLERS)
    {
        Error_Handler();
    }
}

bool registerComChannel(IComChannel *p_com_channel) {}
