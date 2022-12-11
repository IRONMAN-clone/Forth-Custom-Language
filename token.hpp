#ifndef PITT_TOKEN_H
#define PITT_TOKEN_H

#include <iostream>
#include <map>

// Some data definitions
#define byte signed char

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

    // TODO: Work in progress
    P_IF,
    P_ELIF,
    P_END,
    P_THEN,
    P_WHILE,
    P_ELSE,

    P_NE,
    P_G,
    P_E,
    P_LEQ,
    P_GEQ,
    P_L,
    P_DUP,
    P_SWAP,
    P_DROP,
    P_OVER,
    P_ROT,
    P_NROT,
    P_NIP,
    P_PER,
    P_ADDR8,
    P_DEREF8,
    P_PUSH8,
    P_ADDR16,
    P_DEREF16,
    P_PUSH16,
    P_ADDR32,
    P_DEREF32,
    P_PUSH32,

    P_FUNC_PRINT,
    P_FUNC_PUTS,
    P_FUNC_PUTC,
    P_FUNC_WRITE,
    P_FUNC_EXIT,
    P_FUNC_PAUSE,
    P_DEF,
    P_BEGIN,
    P_WORD, // Used as Variable names too and for macros too

    P_COMMENT,
    P_EOT
}PittType;

std::map<std::string,PittType> usable_map;

void init_map()
{
    usable_map["pause"] = P_FUNC_PAUSE;
    usable_map["exit"] = P_FUNC_EXIT;
    usable_map["write"] = P_FUNC_WRITE;
    usable_map["putc"] = P_FUNC_PUTC;
    usable_map["addr(i8)"] = P_ADDR8;
    usable_map["@i8"] = P_DEREF8;
    usable_map["!i8"] = P_PUSH8;
    usable_map["addr(i16)"] = P_ADDR16;
    usable_map["@i16"] = P_DEREF16;
    usable_map["!i16"] = P_PUSH16;
    usable_map["addr(i32)"] = P_ADDR32;
    usable_map["@i32"] = P_DEREF32;
    usable_map["!i32"] = P_PUSH32;
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
    usable_map["#"] = P_COMMENT;
    usable_map["if"] = P_IF;
    usable_map["elif"] = P_ELIF;
    usable_map["end"] = P_END;
    usable_map["then"] = P_THEN;
    usable_map["while"] = P_WHILE;
    usable_map["else"] = P_ELSE;
    usable_map["dup"] = P_DUP; 
    usable_map["swap"] = P_SWAP; 
    usable_map["drop"] = P_DROP; 
    usable_map["over"] = P_OVER;
    usable_map["rot"] = P_ROT;
    usable_map["-rot"] = P_NROT;
    usable_map["nip"] = P_NIP;
    usable_map["%"] = P_PER;
    usable_map["def"] = P_DEF;
    usable_map["begin"] = P_BEGIN;
}


typedef struct Token {
    int r{},c{};
    std::string token{};
    PittType type{};

    // Better End Injection
    PittType associatedType = {};
    // String Error Injection
    bool unescaped{};
}Pitt_token;

template <typename A,typename B,typename C>
struct Tuple {
    A first;
    B second; 
    C third;
};


#endif