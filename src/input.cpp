#include "input.hpp"

Input::Input(std::string entry_file_path)
{
    entry.fpath = std::filesystem::current_path() / entry_file_path;
    included_files.insert(entry.fpath);
}

bool Input::read_input()
{
    if (!std::filesystem::exists(entry.fpath))
    {
        log("FATAL", "The input file \'" + entry.fpath + "' doesn't exist.\n");
        return false;
    }
    if (std::filesystem::is_directory(entry.fpath))
    {
        log("FATAL", "The input file \'" + entry.fpath + "' is a directory.\n");
        return false;
    }
    std::fstream _r(entry.fpath, std::ios::in);
    std::string _l;
    while (!_r.eof())
    {
        std::getline(_r, _l);
        entry._file_contents += _l + '\n';
    }
    _r.close();
    entry.fdetails.lstart = 1;
    entry.parent = nullptr;
    return true;
}

constexpr File *Input::get_result()
{
    return &entry;
}

bool Input::first_pass(size_t _start)
{
    size_t line = _start;
    // go character by character
    // we will not check for grammer other than $include (...)
    std::string::iterator iter = entry._file_contents.begin();
    entry.fdetails.lstart = _start;
    bool comment = false;
    bool multi_line = false;
    while (iter != entry._file_contents.end())
    {
        switch (*iter)
        {
        case '/':
        {
            if (comment || multi_line)
                break;
            if ((iter + 1) != entry._file_contents.end() && *(iter + 1) == '/')
                comment = true;
            break;
        }
        case '@':
        {
            if (comment)
                break;
            multi_line = !multi_line;
            break;
        }
        case '$':
        {
            if (comment || multi_line)
                break;
            std::string temp;
            auto i2 = iter + 1;
            while (std::isspace(*i2))
                i2++;
            while (*i2 != '\n')
            {
                temp += *i2;
                i2++;
            }
            i2 = temp.begin();
            if (!temp.starts_with("include"))
            {
                std::cerr << "In file '" << entry.fpath << "':" << line << ": Expected 'include' after '$' got '" << temp << "' instead.\n";
                return false;
            }
            i2 += 7;
            while (std::isspace(*i2))
                i2++;
            if (i2 == temp.end())
            {
                std::cerr << "In file '" << entry.fpath << "':" << line << ": Expected '(' after 'include' got EOF instead.\n";
                return false;
            }
            if (*i2 != '(')
            {
                std::cerr << "In file '" << entry.fpath << "':" << line << ": Expected '(' after 'include' got '" << *i2 << "' instead.\n";
                return false;
            }
            i2++;
            while (std::isspace(*i2))
                i2++;
            // now get the path
            std::string path;
            while (i2 != temp.end() && !std::isspace(*i2) && *i2 != ')')
            {
                path += *i2;
                i2++;
            }
            while (std::isspace(*i2))
                i2++;
            if (*i2 != ')')
            {
                std::cerr << "In file '" << entry.fpath << "':" << line << ": Expected ')' after the path.\n";
                return false;
            }
            if (included_files.find(path) != included_files.end())
                break;
            Input i(path);
            if (!i.read_input() || !i.first_pass(line))
            {
                std::cerr << "In file '" << path << "':" << line << ": Included from '" << entry.fpath << "'.\n";
                return false;
            }
            File _r = *i.get_result();
            _r.parent = &entry;
            entry.next_files.push_back(_r);
            included_files.merge(i.included_files);
            break;
        }
        case '\n':
            if (comment)
                comment = false;
            line++;
            break;
        }
        iter++;
    }
    return true;
}

void Input::back_trace_error_log()
{
}