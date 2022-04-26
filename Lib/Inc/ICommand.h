/**
 * @file ICommand.h
 *
 */

/*
 * ICommand.h
 *
 *  Created on: Apr 26, 2022
 *      Author: salavat.magazov
 */

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include "../etl/string.h"

#define MAX_COMMAND_SIZE   100
#define MAX_ARGUMENT_COUNT 10

class ICommand
{
public:
    ICommand();

    typedef enum STATUS_T
    {
        COMMAND_OK = 0,
        ERROR_INVALID_NAME,
        ERROR_MAX_SIZE,
        ERROR_MAX_ARGUMENT_COUNT,
        ERROR_MAX_ARGUMENT_SIZE,
        ERROR_INVALID_ARGUMENT,
        ERROR_MALFORMATTED
    } status_t;

    /**
     * @brief Get status of command.
     * @note Command is considered valid if status is returned as COMMAND_OK.
     *
     * @return Last parsing status.
     */
    virtual status_t getStatus() const = 0;

    /**
     * @brief Get name of the last parsed command.
     *
     * @return Name of the last parsed command.
     */
    virtual etl::string<MAX_STRING_SIZE> *getName() const = 0;

    /**
     * @brief Get number of arguments.
     *
     * @return Number of parsed arguments.
     */
    virtual unsigned int getArgumentCount() const = 0;

    /**
     * @brief Access parsed arguments by index of their position in the command
     * string.
     *
     * @param index Position of the argument to be accessed. Zero-based index.
     * @return Value of the argument at the requested index.
     */
    virtual float operator[](unsigned int index) = 0;
};

#endif /* CCOMMAND_H_ */
