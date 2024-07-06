#include <config/property.hpp>
#include <config/property_holder.hpp>
#include <config/toml_parser.hpp>

#include <gtest/gtest.h>

class PropertyFixture
{
    const std::string cfg_path{"data/example.cfg"};

public:
    const std::string section{"section"};
    const std::string key{"key_str"};
    config::TomlParser parser;
    config::PropertyHolder holder;

    PropertyFixture() : parser(cfg_path), holder(parser) {}
};

TEST(PropertyTest, BasicFields)
{
    PropertyFixture fixture;
    config::Property<int, config::TomlParser> int_prop(fixture.parser, fixture.holder, fixture.section, fixture.key);

    EXPECT_EQ(fixture.section, int_prop.section());
    EXPECT_EQ(fixture.key, int_prop.key());
}

TEST(PropertyTest, NoDefault)
{
    PropertyFixture fixture;
    config::Property<int, config::TomlParser> int_prop(fixture.parser, fixture.holder, fixture.section, fixture.key);
    EXPECT_FALSE(int_prop.has_default());
}

TEST(PropertyTest, HasDefault)
{
    PropertyFixture fixture;
    constexpr int default_value{10};
    config::Property<int, config::TomlParser> int_prop(fixture.parser, fixture.holder, fixture.section, fixture.key,
                                                       default_value);

    EXPECT_TRUE(int_prop.has_default());
    EXPECT_EQ(default_value, int_prop.default_value());

    EXPECT_TRUE(int_prop.set_default());
    EXPECT_EQ(default_value, int_prop());
}
