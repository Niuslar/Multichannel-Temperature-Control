/**
 * @file CParser.h
 *
 */
/*
 *  Created on: Apr 4, 2022
 *      Author: niuslar
 */

#ifndef CPARSER_H_
#define CPARSER_H_

#include "../etl/string.h"
#include "../etl/vector.h"
#include "CFIFOBuffer.h"
#include "IComChannel.h"
#include "ICommand.h"
#include "main.h"

#define MAX_TOKENS 60

#ifndef MAX_STRING_SIZE
#    define MAX_STRING_SIZE 100
#endif

class CParser : public ICommand
{
public:
    typedef enum TOKEN_TYPES
    {
        WHITESPACE,
        INTEGER,
        FLOAT,
        STRING_LITERAL,
        BEGIN_OPERATOR,
        END_OPERATOR,
        START_ARRAY_OP,
        END_ARRAY_OP,
        ARGUMENT_DELIMITER,
        KEY_VALUE_DIV
    } token_type_t;

    typedef enum PARSING_STATE
    {
        IDLE,
        BEGIN_KEY,
        END_KEY,
        BEGIN_VALUE,
        BEGIN_ARRAY,
        ADD_VALUE,
        VALUE_ADDED,
        END_VALUES,
        COMMAND_OK,
        ERROR_MAX_ARGS,
        ERROR_NESTED_JSON,
        ERROR
    } parser_state_t;

    typedef struct token
    {
        token_type_t type = WHITESPACE;
        etl::string<MAX_STRING_SIZE> text;

    } token_t;

    // Public methods
    CParser();
    CParser::parser_state_t parse(const etl::string<MAX_STRING_SIZE> &string);
    etl::string<MAX_STRING_SIZE> getName() const;
    unsigned int getArgumentCount() const;
    float operator[](unsigned int index);

private:
    // Private methods
    void addOperator(CParser::token_t &token,
                     CParser::token_type_t operator_type,
                     char character);
    void getTokens(const etl::string<MAX_STRING_SIZE> &string);
    void endToken(CParser::token_t &token);

    // Private variables
    uint8_t m_argument_counter = 0;
    etl::vector<token_t, MAX_TOKENS> m_tokens;
    uint16_t m_token_counter = 0;
    etl::string<MAX_STRING_SIZE> m_command_name;
    float m_arguments[MAX_ARGUMENT_COUNT];
    parser_state_t m_status = IDLE;
};

#endif /* CPARSER_H_ */
