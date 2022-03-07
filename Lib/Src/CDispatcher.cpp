/**
 * @file CDispatcher.cpp
 *
 */

/*
 * CDispatcher.cpp
 *
 *  Created on: 16 Feb 2022
 *      Author: salavat.magazov
 */

#include "CDispatcher.h"
#include "main.h" /* makes HAL function calls available. */

// TODO: make logger optional to allow memory saving if logger is not required.
/**
 * @brief Create instance of the dispatcher class.
 *
 * @param p_logger Pointer to an external logger class.
 */
CDispatcher::CDispatcher(CLog *p_logger)
    : mp_logger(p_logger),
      m_controller_count(0),
      m_active_controller(0),
      m_comchannel_count(0)
{
    if (mp_logger == nullptr)
    {
        Error_Handler();
    }
}

/**
 * @brief Register CController derived class instance with dispatcher.
 *
 * @param p_controller Pointer to the controller instance.
 * @return True is registration was successful.
 */
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
        m_controller_names[m_controller_count] = p_controller->getName();
        m_controller_count++;
        success = true;
    }
    return success;
}

/**
 * @brief Register IComChannel derived class instance with dispatcher.
 *
 * @param p_comchannel Pointer to the com channel instance.
 * @return True if registration successful.
 */
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

/**
 * @brief Method to process incoming commands.
 *
 * @param command Command that arrived via coms channel
 * @param p_comchannel Coms channel which delivered command.
 * @return True if command has been recognised.
 */
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
        std::string::size_type open_bracket = command.find("(");
        std::string::size_type close_bracket = command.find(")");
        if ((open_bracket == command.npos) || (close_bracket == command.npos))
        {
            p_comchannel->send("Command: " + command + " is malformatted.");
        }
        else
        {
            controller_name = command.substr(open_bracket + 1,
                                             close_bracket - open_bracket - 1);
            uint8_t controller_number = findControllerNumber(controller_name);
            mp_controllers[controller_number]->stop();
        }
    }
    else if (command.find("start") < command.npos)
    {
        b_command_recognised = true;
        std::string::size_type open_bracket = command.find("(");
        std::string::size_type close_bracket = command.find(")");
        if ((open_bracket == command.npos) || (close_bracket == command.npos))
        {
            p_comchannel->send("Command: " + command + " is malformatted.");
        }
        else
        {
            controller_name = command.substr(open_bracket + 1,
                                             close_bracket - open_bracket - 1);
            uint8_t controller_number = findControllerNumber(controller_name);
            mp_controllers[controller_number]->start();
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

/**
 * @brief Method to find number of the controller based on the name.
 *
 * @param name Name of the controller.
 * @return
 */
uint8_t CDispatcher::findControllerNumber(std::string name)
{
    uint8_t controller = 0;
    while ((m_controller_names[controller].compare(name) != 0) &&
           (controller < m_controller_count))
    {
        controller++;
    }
    return controller;
}
