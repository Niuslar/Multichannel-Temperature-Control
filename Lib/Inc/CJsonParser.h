/**
 * @file CJsonParser.h
 *
 */
/*
 *  Created on: Apr 4, 2022
 *      Author: niuslar
 */

#ifndef CJSONPARSER_H_
#define CJSONPARSER_H_

#include "../etl/string.h"
#include "../etl/vector.h"
#include "CFIFOBuffer.h"
#include "IComChannel.h"
#include "ICommand.h"
#include "main.h"

#define MAX_TOKENS 60

class CJsonParser : public ICommand
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
        ARRAY_OPEN,
        ARRAY_CLOSE,
        COMMA,
        INVALID
    } token_type_t;

    typedef struct token
    {
        token_type_t type = WHITESPACE;
        etl::string<MAX_STRING_SIZE> text;

    } token_t;

    // Public methods
    void getTokens(const etl::string<MAX_STRING_SIZE> &string);
    etl::vector<token_t, MAX_TOKENS> m_tokens;

    CJsonParser();
    void parse(const etl::string<MAX_STRING_SIZE> &string);
    etl::string<MAX_STRING_SIZE> getName() const;
    unsigned int getArgumentCount() const;
    float operator[](unsigned int index);

private:
    void addSpecialChar(CJsonParser::token_t &token,
                        CJsonParser::token_type_t operator_type,
                        char character);
    void endToken(CJsonParser::token_t &token);

    uint8_t m_argument_counter = 0;
    etl::string<MAX_STRING_SIZE> m_command_name;
    float m_arguments[MAX_ARGUMENT_COUNT];

    uint16_t m_token_counter = 0;
};

#endif /* CJSONPARSER_H_ */
