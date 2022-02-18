/*
 * CDispatcher.h
 *
 *  Created on: 16 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CDISPATCHER_H_
#define CDISPATCHER_H_

#include <stdio.h>
#include <string>

#define MAX_CONTROLLERS 20
#define MAX_COMCHANNELS 5

#include "CController.h"
#include "CLog.h"
#include "IComChannel.h"

class CDispatcher
{
public:
    CDispatcher(CLog *p_logger);
    virtual ~CDispatcher();

    bool registerController(CController *p_controller);
    bool registerComChannel(IComChannel *p_comchannel);
    void run();  // TODO: figure out a reliable way to declare this as noreturn.
    bool newCommand(std::string command, IComChannel *p_comchannel);

private:
    void processComChannels();
    uint8_t findControllerNumber(std::string name);

    CLog *mp_logger;
    CController *mp_controllers[MAX_CONTROLLERS];
    std::string m_controller_names[MAX_CONTROLLERS];
    uint8_t m_controller_count;
    uint8_t m_active_controller;

    IComChannel *mp_comchannels[MAX_COMCHANNELS];
    uint8_t m_comchannel_count;
};

#endif /* CDISPATCHER_H_ */
