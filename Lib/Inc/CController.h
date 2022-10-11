/**
 * @file CController.h
 *
 */

/*
 * CController.h
 *
 *  Created on: 16 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CCONTROLLER_H_
#define CCONTROLLER_H_

#include <stdio.h>
#include "../etl/string.h"
#include "IComChannel.h"
#include "ICommand.h"

// comment out if you don't want to collect runtime statistics
#define COLLECT_STATS

class CController
{
public:
    CController(etl::string<MAX_STRING_SIZE> name, uint32_t run_period_ms);

    etl::string<MAX_STRING_SIZE> getName() const;
    virtual bool tick(uint32_t current_time);
    virtual void run();
    virtual bool newCommand(ICommand *p_command, IComChannel *p_comchannel);
    virtual void reset() = 0;
    virtual void stop();
    virtual void start();
#ifdef COLLECT_STATS
    virtual uint32_t getRunCalls() const;
    virtual void resetRunCalls();
#endif
    virtual void sendResultMessage(ICommand::command_error_code_t error_code);

protected:
    // This variable is for generation and storage of temporary messages, e.g.
    // command replies.
    static etl::string<MAX_STRING_SIZE> s_scratch_pad;

    etl::string<MAX_STRING_SIZE> const m_name;
    uint32_t m_run_period_ms;
    bool mb_stopped;

private:
    uint32_t m_previous_time;
#ifdef COLLECT_STATS
    uint32_t m_run_calls_counter;
#endif
};

#endif /* CCONTROLLER_H_ */
