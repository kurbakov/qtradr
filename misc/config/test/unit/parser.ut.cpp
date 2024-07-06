#include <config/toml_parser.hpp>

#include <gtest/gtest.h>

const std::string cfg_path{"data/example.cfg"};

TEST(TomlParserTest, FilePresented)
{
    config::TomlParser parser(cfg_path);
    ASSERT_TRUE(parser.parse_file());
}

TEST(TomlParserTest, FileMissing)
{
    const std::string fake_cfg_path{"data/missing.cfg"};

    config::TomlParser parser(fake_cfg_path);
    ASSERT_FALSE(parser.parse_file());
}

TEST(TomlParserTest, GetIntValue)
{
    config::TomlParser parser(cfg_path);
    parser.parse_file();

    int value = 0;
    EXPECT_TRUE(parser.get<int>("section", "key_int", value));
    EXPECT_EQ(30, value);
}

TEST(TomlParserTest, GetStrValue)
{
    config::TomlParser parser(cfg_path);
    parser.parse_file();

    std::string value{};
    EXPECT_TRUE(parser.get("section", "key_str", value));
    EXPECT_EQ("hello", value);
}

TEST(TomlParserTest, GetBoolValue)
{
    config::TomlParser parser(cfg_path);
    parser.parse_file();

    bool t;
    bool f;
    EXPECT_TRUE(parser.get("section", "key_bool_true", t));
    EXPECT_TRUE(parser.get("section", "key_bool_false", f));
    EXPECT_TRUE(t);
    EXPECT_FALSE(f);
}

TEST(TomlParserTest, MissingSection)
{
    config::TomlParser parser(cfg_path);
    parser.parse_file();

    int v;
    EXPECT_FALSE(parser.get("fake_section", "key_int", v));
}

TEST(TomlParserTest, MissingKey)
{
    config::TomlParser parser(cfg_path);
    parser.parse_file();

    int v;
    EXPECT_FALSE(parser.get("section", "fake_key", v));
}
