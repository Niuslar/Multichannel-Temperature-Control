/**
 * @file CDispatcher.h
 *
 */

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
#include "CJsonParser.h"
#include "CUartCom.h"
#include "IComChannel.h"

class CDispatcher
{
public:
    CDispatcher(CUartCom *p_uart_com);

    bool registerController(CController *p_controller);
    bool registerComChannel(IComChannel *p_comchannel);
    void run();  // TODO: figure out a reliable way to declare this as noreturn.
    bool newCommand(ICommand *p_command, IComChannel *p_comchannel);

private:
    void processComChannels();
    uint8_t findControllerNumber(etl::string<MAX_STRING_SIZE> name);

    CJsonParser m_json_parser;
    CUartCom *mp_uart_com;
    CController *mp_controllers[MAX_CONTROLLERS];
    etl::string<MAX_STRING_SIZE> m_controller_names[MAX_CONTROLLERS];
    uint8_t m_controller_count;
    uint8_t m_active_controller;

    IComChannel *mp_comchannels[MAX_COMCHANNELS];
    uint8_t m_comchannel_count;
};

#endif /* CDISPATCHER_H_ */
