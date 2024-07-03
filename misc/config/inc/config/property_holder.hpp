#pragma once

#include <vector>

#include "config/helper.hpp"

namespace config
{

class IProperty;

class PropertyHolder
{
public:
    explicit PropertyHolder(ConfigHelper &cfgHelper) : _cfgHelper(cfgHelper) {}

    void add_property(IProperty *property) { _properties.push_back(property); }

    [[nodiscard]] size_t size() const { return _properties.size(); }

    [[nodiscard]] bool populate() const;

private:
    ConfigHelper &_cfgHelper;
    std::vector<IProperty *> _properties;
};
} // namespace config
