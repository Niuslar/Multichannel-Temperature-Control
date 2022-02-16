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

#define MAX_NAME_SIZE 20

class CController
{
public:
    CController(char *p_name, uint32_t run_period_ms);
    virtual ~CController();

    virtual void tick(uint32_t current_time);
    virtual bool run() = 0;
    virtual bool newCommand(char *p_command);

protected:
    char m_name[MAX_NAME_SIZE];
    uint32_t m_run_period_ms;

private:
    uint32_t m_previous_tick;
};

#endif /* CCONTROLLER_H_ */
