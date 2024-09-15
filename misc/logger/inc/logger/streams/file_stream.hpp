#pragma once

#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>
#include <filesystem>
#include <format>

namespace logger::stream
{

class FileStream
{
    std::string get_file_name() {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer,sizeof(buffer),"LOG_%Y-%m-%d/%H:%M:%S",timeinfo);
        std::string str(buffer);

        return str;
    }
public:
    FileStream() : m_file{nullptr}
    {
        std::filesystem::path wd("./");

        auto file = get_file_name();
        m_file = fopen(file.c_str(), "w+");
        if (m_file == nullptr)
        {
            // handle error
        }
    }

    ~FileStream()
    {
        if (m_file != nullptr)
        {
            fclose(m_file);
        }
    }

    bool write_line(const std::string_view msg)
    {
        fputs(msg.data(), m_file);
        fflush(m_file);

        return true;
    }

    bool write_batch(const std::vector<std::string_view> &msgs)
    {
        for (auto &msg : msgs)
        {
            fputs(msg.data(), m_file);
        }
        fflush(m_file);

        return true;
    }

private:
    FILE *m_file;
};
} // namespace logger::stream
