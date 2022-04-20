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

#define MAX_STRING_SIZE 60
#define MAX_TOKENS      60
#define MAX_ARGUMENTS   10

class CParser
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
        KEY_VALUE_DIV,
        INVALID
    } token_type_t;

    typedef enum PARSING_STATUS
    {
        IDLE,
        BEGIN_KEY,
        END_KEY,
        BEGIN_VALUE,
        BEGIN_ARRAY,
        ADD_VALUE,
        VALUE_ADDED,
        END_VALUE,
        FINISHED,
        ERROR
    } parse_status_t;

    typedef struct token
    {
        token_type_t type = WHITESPACE;
        etl::string<MAX_STRING_SIZE> text;

    } token_t;

    typedef struct command
    {
        etl::string<MAX_STRING_SIZE> name;
        float argument[MAX_ARGUMENTS];
    } command_t;

    // Public methods
    CParser::parse_status_t parse(const etl::string<MAX_STRING_SIZE> &string);
    command_t getCommand();
    etl::vector<token_t, MAX_TOKENS> m_tokens;
    uint8_t m_argument_counter = 0;

private:
    void getTokens(const etl::string<MAX_STRING_SIZE> &string);
    void endToken(CParser::token_t &token);
    uint16_t m_token_counter = 0;
    parse_status_t m_status = IDLE;
    command_t m_command;
};

#endif /* CPARSER_H_ */
