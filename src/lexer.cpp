#include "lexer.hpp"

Lexer::Lexer(File *entry)
{
    context.f = entry;
    context.line = 1;
    context.curr = entry->_file_contents.begin();
    context.end = entry->_file_contents.end();
}

LexerContext &LexerContext::operator++()
{
    curr++;
    return *this;
}

bool LexerContext::operator!()
{
    return !(curr == end);
}

LexerContext &LexerContext::operator=(LexerContext &&c)
{
    line = c.line;
    f = c.f;
    curr = c.curr;
    end = c.end;
    return *this;
}

char LexerContext::operator*()
{
    return *curr;
}

std::string::iterator LexerContext::operator+(size_t num)
{
    return (curr + num);
}

bool operator!=(LexerContext &c, std::string::iterator i)
{
    return (c.end != i);
}

void Lexer::consume()
{
    if (!context)
    {
        context++;
    }
    return;
}

Token Lexer::next_token()
{
    Token t;
    while (!context)
    {
        if (std::isspace(*context))
        {
            while (std::isspace(*context))
            {
                consume();
            }
        }
    }
}