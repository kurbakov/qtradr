#include "config/property_holder.hpp"
#include "config/property.hpp"

namespace config
{
[[nodiscard]] bool PropertyHolder::populate() const
{
    if (!_cfgHelper.parse_file())
    {
        std::cerr << "Failed to parse the config file: " << _cfgHelper.path() << std::endl;
        return false;
    }

    bool res = true;
    for (auto *prop : _properties)
    {
        const auto section = prop->section();
        const auto key = prop->key();
        auto value = _cfgHelper.getValueAsString(section, key);

        if (value.empty())
        {
            if (prop->has_default())
            {
                prop->set_default();
            }
            else
            {
                res = false;
                std::cerr << "Fail to load config" << std::endl;
            }
        }
        else
        {
            if (!prop->from_str(value))
            {
                res = false;
                std::cerr << "Fail to set from string config" << std::endl;
            }
        }
    }

    return res;
}
} // namespace config
