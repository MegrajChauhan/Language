#ifndef _DEFS_
#define _DEFS_

#include <string>
#include <vector>
#include <unordered_map>

struct Details
{
    size_t lstart, lend;
};

struct File
{
    std::string _file_contents;
    Details fdetails;
    std::string fpath;
    std::vector<File> next_files;
    File *parent;
};

enum TokenType
{
    ERR,
    VAR,
    CONST,
    ID,
    U, // use any length 'X'
    I, // signed version
    COLON,
    EQUALS,
    COPEN,
    CCLOSE,
    SEMI_COLON,
    NUM_INT,
    NUM_FLOAT,
    BOPEN,
    BCLOSE,
    QUOTE,
    INCLUDE, // useless really for lexer but this refers to '$'
};

struct Token
{
    TokenType type;
    std::string value;
    size_t linest, lined, colst, coled;
    File *f; // the file that it belonged to

    bool operator!();
};

static std::unordered_map<std::string, TokenType>
    key_map =
        {
            {"var", VAR},
            {"const", CONST},
            {"u", U},
            {"i", I},
};

#endif