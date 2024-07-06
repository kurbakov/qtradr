#include "config/property_holder.hpp"
#include "config/property.hpp"
#include "config/toml_parser.hpp"

namespace config
{

PropertyHolder::PropertyHolder(TomlParser &parser) : _parser(parser) {}

void PropertyHolder::add_property(IProperty *property) { _properties.push_back(property); }

bool PropertyHolder::populate() const
{
    if (!_parser.parse_file())
    {
        std::cerr << "Failed to parse the config file: " << _parser.path() << std::endl;
        return false;
    }

    bool res = true;
    for (auto *prop : _properties)
    {
        if (!prop->set_value())
        {
            res = false;
            std::cerr << "Fail to load config" << std::endl;
        }
    }

    return res;
}
} // namespace config
