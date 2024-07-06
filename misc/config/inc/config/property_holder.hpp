#pragma once

#include <vector>

namespace config
{

class IProperty;
class TomlParser;

class PropertyHolder
{
public:
    explicit PropertyHolder(TomlParser &parser);
    void add_property(IProperty *property);
    bool populate() const;

private:
    TomlParser &_parser;
    std::vector<IProperty *> _properties;
};
} // namespace config
