#pragma once

#include <iostream>
#include <string>

#include <mINI/ini.h>

namespace config
{
class ConfigHelper
{
public:
    explicit ConfigHelper(const std::string &path) : _file(path), _path((path)) {}

    std::string getValueAsString(const std::string &section, const std::string &key)
    {
        if (!_ini.has(section))
        {
            std::cerr << "Failed to load the section: " << section << std::endl;
            return {};
        }

        if (!_ini[section].has(key))
        {
            std::cerr << "Failed to load the key: " << key << std::endl;
            return {};
        }

        return _ini[section][key];
    }

    bool parse_file() { return _file.read(_ini); }

    std::string path() const { return _path; }

private:
    mINI::INIStructure _ini;
    mINI::INIFile _file;
    const std::string _path;
};
} // namespace config
