#pragma once

#include <optional>
#include <string>

#include "property_holder.hpp"

namespace config
{

class IProperty
{
public:
    virtual ~IProperty() {}

    virtual std::string_view section() = 0;
    virtual std::string_view key() = 0;
    virtual bool has_default() = 0;
    virtual bool set_value() = 0;
    virtual bool set_default() = 0;
};

template <typename T, class Parser> class Property final : public IProperty
{
public:
    using data_type = T;
    Property(const Parser &parser, PropertyHolder &holder, const std::string_view section, const std::string_view key)
        : _parser(parser), _section(section), _key(key), _value{}, _default{}
    {
        holder.add_property(this);
    }

    Property(const Parser &parser, PropertyHolder &holder, const std::string_view section, const std::string_view key,
             const T &default_value)
        : _parser(parser), _section(section), _key(key), _value{}, _default(default_value)
    {
        holder.add_property(this);
    }

    bool has_default() override { return _default.has_value(); }

    const std::optional<T> &default_value() { return _default; }

    bool set_default() override
    {
        if (_default.has_value())
            _value = _default.value();

        return _default.has_value();
    }

    const T &operator()() { return _value; }

    std::string_view section() override { return _section; }

    std::string_view key() override { return _key; }

    bool set_value() override
    {
        if (_parser.get(_section, _key, _value))
        {
            return true;
        }

        if (_default.has_value())
        {
            _value = _default.value();
            return true;
        }

        return false;
    }

private:
    const Parser &_parser;
    const std::string _section;
    const std::string _key;

    T _value;
    std::optional<T> _default;
};

} // namespace config
