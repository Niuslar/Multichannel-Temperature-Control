/*
 * CDispatcher.cpp
 *
 *  Created on: 16 Feb 2022
 *      Author: salavat.magazov
 */

#include "CDispatcher.h"
#include "stm32l0xx_hal.h" /* makes HAL function calls available. */

CDispatcher::CDispatcher(CLog *p_logger)
    : mp_logger(p_logger),
      m_controller_count(0),
      m_active_controller(0),
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

/**
 * @brief The CDispatcher runner method. This method will never exit. This is
 * where super loop is executed.
 *
 */
void CDispatcher::run()
{
    uint32_t current_time;
    uint8_t idle_loop_counter = 0;  // TODO: this should be part of
                                    // COLLECT_STATS
    while (1)
    {
        /* scan through all registered controllers and query if they need to be
         * called. If they are due, then call the run() method. Then move to
         * next controller.*/

        // TODO: consider moving this bit of code into a private method.
        current_time = HAL_GetTick();
        if (mp_controllers[m_active_controller]->tick(current_time))
        {
            mp_controllers[m_active_controller]->run();
            idle_loop_counter = 0;
        }
        if (++m_active_controller >= m_controller_count)
        {
            m_active_controller = 0;
            idle_loop_counter++;
        }
        /* If full loop was done and no calls to run() methods were made, then
         * query if any new commands are available on the coms channels.*/

        // TODO: consider having a callback within the CDispatcher class that
        // gets registered with the IComChannel class and is called when new
        // command arrives. This might be a better option for a more real-time
        // response to commands.
        processComChannels();
    }
}

bool CDispatcher::newCommand(std::string command, IComChannel *p_comchannel)
{
    bool b_command_recognised = false;
    // TODO: when the command API is firmed up
    // Example. Start/stop controller.
    // start(Controller name)/stop(Controller name)
    std::string controller_name;
    if (command.find("stop") < command.npos)
    {
        b_command_recognised = true;
        command.size_type open_bracket = command.find("(");
        command.size_type close_bracket = command.find(")");
        if ((open_bracket == command.npos) || (close_bracket == command.npos))
        {
            p_comchannel->send("Command: " + command + " is malformatted.");
        }
        else
        {
            controller_name =
                command.substr(open_bracket, close_bracket - open_bracket)
        }
    }

    return b_command_recognised;
}

/**
 * @brief Check every registered coms channel for existing queued up commands.
 * If command is present pass it on to each controller in turn until one
 * recognises command and takes appropriate action. If no controller recognises
 * the command, respond with corresponding error message.
 * @note Since this logic will keep processing the commands as long as there are
 * some in the incoming buffer it is possible to DDoS the system with a
 * sufficiently fast coms channels. Effort is specifically not taken to protect
 * against such malicious attack, therefore the system architect must bear in
 * mind the adverse effect of flooding the system with large volume of traffic.
 */
void CDispatcher::processComChannels()
{
    for (int channel = 0; channel < m_comchannel_count; channel++)
    {
        while (mp_comchannels[channel]->isCommandAvailable())
        {
            bool b_command_recognised;
            std::string command = mp_comchannels[channel]->getCommand();
            /* first check if this command is for CDispatcher. */
            b_command_recognised = newCommand(command, mp_comchannels[channel]);
            uint8_t controller = 0;
            while (!b_command_recognised && (controller < m_controller_count))
            {
                b_command_recognised = mp_controllers[controller]->newCommand(
                    command,
                    mp_comchannels[channel]);
                controller++;
            }
            if (!b_command_recognised)
            {
                std::string message;
                message = "Command: " + command + " has not been recognised.";
                mp_comchannels[channel]->send(message);
            }
        }
    }
}
