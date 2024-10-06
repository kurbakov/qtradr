#pragma once

#include <ctime>
#include <iostream>

#include "logger/defs.hpp"

namespace stream {
class IO {
public:
    static void process(const logger::Log &log) {
        if (log.meta) {
            std::clog << "META | "
                      << log.uuid << " "
                      << logger::to_string(log.meta->level) << " "
                      << log.meta->location.file_name() << ":" << log.meta->location.line() << " | "
                      << log.meta->pattern << std::endl;
        } else {
            char time_buffer[64];
            std::strftime(time_buffer, sizeof time_buffer, "%Y%m%d-%H:%M:%S", std::localtime(&log.data->tv.tv_sec));
            std::clog
                << "DATA | "
                << log.uuid << " "
                << time_buffer << "." << log.data->tv.tv_usec << " "
                << "[" << log.data->tid << "]  | "
                << log.data->args << std::endl;
        }
    }
};
}// namespace stream