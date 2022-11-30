#ifndef PITT_TOKEN_H
#define PITT_TOKEN_H

#include <iostream>
#include <map>

// Porth Tokens
typedef enum {
    P_INT = 0,
    P_STR,
    P_CHR,
    P_FLT, // TODO: Will be implemented with pointers syntax: @f32 

    P_PLUS,
    P_MIN,
    P_MUL,
    P_DIV,
    P_BOR,
    P_BAND,
    P_BSHR,
    P_BSHL,
    P_BNOT,
    P_BXOR,

    P_NE,
    P_G,
    P_E,
    P_LEQ,
    P_GEQ,
    P_L,

    P_FUNC_PRINT,
    P_FUNC_PUTS,
    P_VAR_NAME,

    P_COMMENT,
    P_UNDEF
}PittType;

std::map<std::string,PittType> usable_map;

void init_map()
{
    usable_map["!="] = P_NE;
    usable_map[">"] = P_G;
    usable_map["<"] = P_L;
    usable_map["="] = P_E;
    usable_map["<="] = P_LEQ;
    usable_map[">="] = P_GEQ;
    usable_map["bor"] = P_BOR;
    usable_map["band"] = P_BAND;
    usable_map["shr"] = P_BSHR;
    usable_map["shl"] = P_BSHL;
    usable_map["not"] = P_BNOT;
    usable_map["xor"] = P_BXOR;
    usable_map["+"] = P_PLUS;
    usable_map["-"] = P_MIN;
    usable_map["*"] = P_MUL;
    usable_map["/"] = P_DIV;
    usable_map["print"] = P_FUNC_PRINT;
    usable_map["puts"] = P_FUNC_PUTS;
    usable_map[";;"] = P_COMMENT;
}

typedef struct Token {
    int r{},c{};
    std::string token{};
    PittType type{};
}Pitt_token;


#endif