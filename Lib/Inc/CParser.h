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

#define MAX_TOKENS    60
#define SPECIAL_CHARS 6

#ifndef MAX_STRING_SIZE
#    define MAX_STRING_SIZE 100
#endif

class CParser : public ICommand
{
public:
    typedef enum TOKEN_TYPES_T
    {
        WHITESPACE,
        INTEGER,
        FLOAT,
        CURLY_OPEN,
        CURLY_CLOSE,
        COLON,
        STRING,
        NUMBER,
        ARRAY_OPEN,
        ARRAY_CLOSE,
        COMMA,
        INVALID
    } token_type_t;

    typedef enum PARSING_STATE_T
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
    void addOperator(CParser::token_t &token,
                     CParser::token_type_t operator_type,
                     char character);
    void getTokens(const etl::string<MAX_STRING_SIZE> &string);
    void endToken(CParser::token_t &token);
    bool isSpecialChar(char character);

    // These characters are used to format JSON
    char m_special_characters[SPECIAL_CHARS] = {'{', '}', ',', '[', ']', ':'};

    uint8_t m_argument_counter = 0;
    etl::string<MAX_STRING_SIZE> m_command_name;
    float m_arguments[MAX_ARGUMENT_COUNT];

    etl::vector<token_t, MAX_TOKENS> m_tokens;
    uint16_t m_token_counter = 0;
    parser_state_t m_status = IDLE;
};

#endif /* CPARSER_H_ */
