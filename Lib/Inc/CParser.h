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

#define MAX_COMMANDS    40
#define MAX_STRING_SIZE 60
#define MAX_TOKENS      60

class CParser
{
public:
    typedef enum TOKEN_TYPES
    {
        WHITESPACE,
        IDENTIFIER,
        INTEGER,
        FLOAT,
        STRING_LITERAL,
        OPERATOR,
        INVALID
    } token_type_t;

    typedef struct token
    {
        token_type_t type = WHITESPACE;
        etl::string<MAX_STRING_SIZE> text;

    } token_t;

    // Public methods
    void parse(const etl::string<MAX_STRING_SIZE> &string);

    etl::vector<token_t, MAX_TOKENS> m_tokens;

private:
    void getTokens(const etl::string<MAX_STRING_SIZE> &string);
    void endToken(CParser::token_t &token);
};

#endif /* CPARSER_H_ */
