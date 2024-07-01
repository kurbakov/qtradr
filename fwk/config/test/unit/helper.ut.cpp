#include <config/helper.hpp>

#include <gtest/gtest.h>

const std::string ini_path{"data/example.ini"};

TEST(ConfigHelperTest, FilePresented)
{
    config::ConfigHelper cfgHelper(ini_path);
    ASSERT_TRUE(cfgHelper.parse_file());
}

TEST(ConfigHelperTest, FileMissing)
{
    const std::string fake_ini_path{"data/missing.ini"};

    config::ConfigHelper cfgHelper(fake_ini_path);
    ASSERT_FALSE(cfgHelper.parse_file());
}

TEST(ConfigHelperTest, GetIntValue)
{
    config::ConfigHelper cfgHelper(ini_path);
    cfgHelper.parse_file();

    EXPECT_EQ("30", cfgHelper.getValueAsString("section", "key_int"));
}

TEST(ConfigHelperTest, GetStrValue)
{
    config::ConfigHelper cfgHelper(ini_path);
    cfgHelper.parse_file();

    EXPECT_EQ("hello", cfgHelper.getValueAsString("section", "key_str"));
}

TEST(ConfigHelperTest, GetBoolValue)
{
    config::ConfigHelper cfgHelper(ini_path);
    cfgHelper.parse_file();

    EXPECT_EQ("true", cfgHelper.getValueAsString("section", "key_bool_true"));
    EXPECT_EQ("false", cfgHelper.getValueAsString("section", "key_bool_false"));
}

TEST(ConfigHelperTest, MissingSection)
{
    config::ConfigHelper cfgHelper(ini_path);
    cfgHelper.parse_file();

    EXPECT_EQ("", cfgHelper.getValueAsString("fake_section", "key_int"));
}

TEST(ConfigHelperTest, MissingKey)
{
    config::ConfigHelper cfgHelper(ini_path);
    cfgHelper.parse_file();

    EXPECT_EQ("", cfgHelper.getValueAsString("section", "fake_key"));
}
