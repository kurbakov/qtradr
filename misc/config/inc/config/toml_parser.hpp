#pragma once

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

#include <toml++/toml.hpp>

namespace config
{

class TomlParser
{
public:
    explicit TomlParser(const std::string &path) : _config_file(path), _config{} {}

    template <typename T> bool get(const std::string &section, const std::string &key, T &value) const
    {
        std::optional<T> res;
        try
        {
            if (section.empty() && !key.empty())
            {
                res = _config[key].value<T>();
            }
            else if (!section.empty() && !key.empty())
            {
                res = _config[section][key].value<T>();
            }
        }
        catch (...)
        {
        }

        if (res.has_value())
        {
            value = res.value();
            return true;
        }

        return false;
    }

    bool parse_file()
    {
        if (!std::filesystem::exists(_config_file))
        {
            std::cerr << "File not found" << std::endl;
            return false;
        }

        try
        {
            _config = toml::parse_file(_config_file);
        }
        catch (const toml::parse_error &err)
        {
            std::cerr << "Parsing failed:\n" << err << std::endl;
            return false;
        }

        return true;
    }

    std::string path() const { return _config_file; }

private:
    const std::string _config_file;
    toml::table _config;
};
} // namespace config
