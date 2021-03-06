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
#include "CStringParser.h"
#include "IComChannel.h"
#include "main.h"

#define MAX_TOKENS 60

class CJsonParser : public CStringParser
{
public:
    CJsonParser();
    virtual bool parse(const etl::string<MAX_STRING_SIZE> &string);

    typedef enum TOKEN_TYPE_T
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

    typedef struct SToken
    {
        token_type_t type = WHITESPACE;
        etl::string<MAX_STRING_SIZE> text;
    } token_t;

private:
    void stringToTokens(const etl::string<MAX_STRING_SIZE> &string);
    void addSpecialChar(CJsonParser::token_type_t operator_type,
                        char character);
    void finishCurrentToken();
    void resetCurrentToken();
    void processDigit(char current_char);
    void processSpacer(char current_char);
    void processDecimalPoint(char current_char);
    void processDoubleQuote(char current_char);
    void processDefault(char current_char);

    /* Parser helper functions */
    bool parseObject();
    bool parsePair();
    bool parseValue();
    bool parseArray();

    uint8_t m_token_index = 0;
    etl::vector<token_t, MAX_TOKENS> m_tokens;

    token_t m_current_token;

    uint16_t m_token_counter = 0;
};

#endif /* CJSONPARSER_H_ */
