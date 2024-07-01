#pragma once

#include <string>
#include <optional>

#include "property_holder.hpp"

namespace config
{

class IProperty
{
public:
    virtual ~IProperty() {}

    virtual std::string section() = 0;
    virtual std::string key() = 0;
    virtual bool has_default() = 0;
    virtual bool from_str(const std::string &) = 0;
    virtual bool set_default() = 0;
};

template <typename T> class Property final : public IProperty
{
public:
    Property(PropertyHolder &holder, const std::string_view section, const std::string_view key)
        : _section(section), _key(key)
    {
        holder.add_property(this);
    }

    Property(PropertyHolder &holder, const std::string_view section, const std::string_view key, const T &default_value)
        : _section(section), _key(key), _default(default_value)
    {
        holder.add_property(this);
    }

    bool has_default() override { return _default.has_value(); }

    const T &default_value() { return _default.value(); }

    const T &operator()() { return _value; }

    std::string section() override { return _section; }

    std::string key() override { return _key; }

    bool set_default() override
    {
        if (_default.has_value())
            _value = _default.value();

        return _default.has_value();
    }

    bool from_str(const std::string &val) override;

private:
    const std::string _section;
    const std::string _key;
    T _value;
    std::optional<T> _default;
};

template <> inline bool Property<int>::from_str(const std::string &val)
{
    const int res = std::stoi(val);
    _value = res;

    return true;
}

template <> inline bool Property<std::string>::from_str(const std::string &val)
{
    _value = val;

    return true;
}
} // namespace qconfig


