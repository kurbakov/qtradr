#include <string>

#include "config/property.hpp"
#include "config/property_holder.hpp"
#include "config/toml_parser.hpp"

#include <gtest/gtest.h>

class PropertyHolderFixture
{
    const std::string cfg_path{"data/example.cfg"};

public:
    const std::string section{"section"};
    const std::string key_int{"key_int"};
    const std::string key_str{"key_str"};

    config::TomlParser parser;

    PropertyHolderFixture() : parser(cfg_path) {}
};

TEST(PropertyHolderTest, Foo)
{
    PropertyHolderFixture fixture;

    config::PropertyHolder holder(fixture.parser);

    config::Property<int, config::TomlParser> int_prop(fixture.parser, holder, fixture.section, fixture.key_int);
    config::Property<std::string, config::TomlParser> str_prop(fixture.parser, holder, fixture.section,
                                                               fixture.key_str);

    EXPECT_TRUE(holder.populate());

    EXPECT_EQ(30, int_prop());
    EXPECT_EQ("hello", str_prop());
}
