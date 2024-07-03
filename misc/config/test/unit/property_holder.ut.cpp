#include <string>

#include "config/helper.hpp"
#include "config/property.hpp"
#include "config/property_holder.hpp"

#include <gtest/gtest.h>

class PropertyHolderFixture
{
    const std::string ini_path{"data/example.ini"};

public:
    const std::string section{"section"};
    const std::string key_int{"key_int"};
    const std::string key_str{"key_str"};

    config::ConfigHelper cfgHelper;

    PropertyHolderFixture() : cfgHelper(ini_path) {}
};

TEST(PropertyHolderTest, Foo)
{
    PropertyHolderFixture fixture;
    config::PropertyHolder holder(fixture.cfgHelper);

    config::Property<int> int_prop(holder, fixture.section, fixture.key_int);
    config::Property<std::string> str_prop(holder, fixture.section, fixture.key_str);

    EXPECT_EQ(2, holder.size());

    EXPECT_TRUE(holder.populate());

    EXPECT_EQ(30, int_prop());
    EXPECT_EQ("hello", str_prop());
}
