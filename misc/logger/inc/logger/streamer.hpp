#pragma once

#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

namespace logger::stream
{

class File
{
public:
    File() : _file{nullptr}
    {
        _file = fopen("some_name.txt", "w+");
        if (_file == nullptr)
        {
            // handle error
        }
    }

    ~File()
    {
        if (_file != nullptr)
        {
            fclose(_file);
        }
    }

    bool write_line(const std::string_view msg)
    {
        fputs(msg.data(), _file);
        fflush(_file);

        return true;
    }

    bool write_batch(const std::vector<std::string_view> &msgs)
    {
        for (auto &msg : msgs)
        {
            fputs(msg.data(), _file);
        }
        fflush(_file);

        return true;
    }

private:
    FILE *_file;
};

class Cout
{
public:
    bool write_line(const std::string_view msg)
    {
        std::cout << msg << std::endl;
        std::cout.flush();

        return true;
    }

    bool write_batch(const std::vector<std::string_view> &msgs)
    {
        for (auto &msg : msgs)
        {
            std::cout << msg << std::endl;
        }
        std::cout.flush();

        return true;
    }
};
} // namespace logger::stream
