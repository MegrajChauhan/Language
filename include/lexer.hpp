#ifndef _LEXER_
#define _LEXER_

#include "defs.hpp"
#include <string>
#include <vector>

#define is_comment(cont) (*cont == '/' && (cont != (cont+1)) && *(cont + 1) == '/')
#define is_multiline_comment(cont) (*cont == '@')
#define is_num(n) (n >= '0' && n <= '9')
#define is_alpha(n) ((n >= 'a' && n <= 'z') || (n >= 'A' && n <= 'Z'))
#define is_alnum(n) (is_num(n) || is_alpha(n))

struct LexerContext
{
    size_t line;
    File *f;
    std::string::iterator curr;
    std::string::iterator end;

    LexerContext& operator++();
    LexerContext& operator++(int);
    bool operator!();
    LexerContext& operator=(LexerContext &&c);
    char operator*();
    std::string::iterator operator+(size_t num);

    friend bool operator!=(LexerContext &c, std::string::iterator i);
};

class Lexer
{
    std::vector<LexerContext> _older_state;
    LexerContext context;

public:
    Lexer(File *entry);

    Token next_token();

    void consume();
};

#endif