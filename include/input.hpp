#ifndef _INPUT_
#define _INPUT_

// Read the source file and make one pass to resolve includes
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_set>
#include "log.hpp"
#include "defs.hpp"

class Input
{
    File entry;
    std::unordered_set<std::string> included_files; // make sure some files are not included multiple times
public:
    Input(std::string entry_file_path);

    bool read_input();

    bool first_pass(size_t _start);

    constexpr File* get_result();

    void back_trace_error_log();
};

#endif