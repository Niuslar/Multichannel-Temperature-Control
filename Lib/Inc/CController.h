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
#include <string.h>
#include <string>

#define MAX_NAME_SIZE 20
// comment out if you don't want to collect runtime statistics
#define COLLECT_STATS

class CController
{
public:
    CController(std::string name, uint32_t run_period_ms);
    virtual ~CController();

    std::string getName() const;
    virtual bool tick(uint32_t current_time);
    virtual void run();
    virtual bool newCommand(std::string command);
#ifdef COLLECT_STATS
    virtual uint32_t getRunCalls() const;
    virtual void resetRunCalls();
#endif

protected:
    std::string const m_name;
    uint32_t m_run_period_ms;

private:
    uint32_t m_previous_time;
#ifdef COLLECT_STATS
    uint32_t m_run_calls_counter;
#endif
};

#endif /* CCONTROLLER_H_ */
