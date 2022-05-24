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

#define MAX_COMMAND_SIZE 100
#if (MAX_COMMAND_SIZE > MAX_STRING_SIZE)
#    error "Command size too big to fit into strings."
#endif

#define MAX_ARGUMENT_COUNT 10
#define DEFAULT_ARGUMENT   0

class ICommand
{
public:
    ICommand() {}

    /**
     * @brief Possible outcomes of processing new command.
     */
    typedef enum COMMAND_ERROR_CODE_T
    {
        COMMAND_OK = 0,       /**< COMMAND_OK */
        ERROR_ARG_COUNT,      // wrong number of arguments
        ERROR_OUT_OF_BOUNDS,  // arguments out of bounds
        ERROR_TYPE_MISMATCH   // argument is wrong type, e.g. float when int
                              // expected
    } command_error_code_t;

    /**
     * @brief Get name of the last parsed command.
     *
     * @return Name of the last parsed command.
     */
    virtual const etl::string<MAX_STRING_SIZE> *getName() const = 0;

    /**
     * @brief Get number of arguments.
     *
     * @return Number of parsed arguments.
     */
    virtual unsigned int getArgumentCount() const = 0;

    /**
     * @brief Get string argument.
     *
     * @return String argument if available, empty string if not.
     */
    virtual const etl::string<MAX_STRING_SIZE> *getStringArgument() const = 0;

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
