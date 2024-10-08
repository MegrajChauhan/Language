#include "defs.hpp"

bool Token::operator!()
{
    return !(type == ERR);
}