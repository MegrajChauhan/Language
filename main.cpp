#include "input.hpp"

int main()
{
    Input i("examples/temp.l");
    i.read_input();
    i.first_pass(1);
    File *f = i.get_result();
}