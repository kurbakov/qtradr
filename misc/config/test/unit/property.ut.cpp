#include <config/property.hpp>
#include <config/helper.hpp>
#include <config/property_holder.hpp>

#include <gtest/gtest.h>

class PropertyFixture
{
    const std::string ini_path{"data/example.ini"};

public:
    const std::string section{"section"};
    const std::string key{"key_str"};
    config::ConfigHelper cfgHelper;
    config::PropertyHolder holder;

    PropertyFixture() : cfgHelper(ini_path), holder(cfgHelper) {}
};

TEST(PropertyTest, BasicFields)
{
    PropertyFixture fixture;
    config::Property<int> int_prop(fixture.holder, fixture.section, fixture.key);

    EXPECT_EQ(fixture.section, int_prop.section());
    EXPECT_EQ(fixture.key, int_prop.key());
}

TEST(PropertyTest, NoDefault)
{
    PropertyFixture fixture;
    config::Property<int> int_prop(fixture.holder, fixture.section, fixture.key);
    EXPECT_FALSE(int_prop.has_default());

    EXPECT_EQ(1, fixture.holder.size());
}

TEST(PropertyTest, HasDefault)
{
    PropertyFixture fixture;
    constexpr int default_value{10};
    config::Property<int> int_prop(fixture.holder, fixture.section, fixture.key, default_value);

    EXPECT_TRUE(int_prop.has_default());
    EXPECT_EQ(default_value, int_prop.default_value());

    EXPECT_EQ(1, fixture.holder.size());

    EXPECT_TRUE(int_prop.set_default());
    EXPECT_EQ(default_value, int_prop());
}

TEST(PropertyTest, FromStrInt)
{
    PropertyFixture fixture;
    config::Property<int> int_prop(fixture.holder, fixture.section, fixture.key);
    const std::string int_str("123");
    int_prop.from_str(int_str);

    EXPECT_EQ(123, int_prop());
}
