#include <gtest/gtest.h>

#include "json.h"

TEST(libjson_test_number, parse_integer)
{
    const char* buf  = "123 ";
    size_t  max = strlen(buf);
    uint32_t line = 0;
    uint8_t* b = (uint8_t*)buf;
    json::Number* n = dynamic_cast<json::Number*>(json::Number::parse(b, max, line));
    EXPECT_TRUE( n != NULL );
    EXPECT_EQ(123, (int)*n);
    EXPECT_TRUE(n->str() == "123");
}

TEST(libjson_test_number, parse_float)
{
    const char* buf  = "4.2 ";
    size_t  max = strlen(buf);
    uint32_t line = 0;
    uint8_t* b = (uint8_t*)buf;
    json::Number* n = dynamic_cast<json::Number*>(json::Number::parse(b, max, line));
    EXPECT_TRUE( n != NULL );
    EXPECT_EQ(4.2, (double)*n);
    EXPECT_TRUE(n->str() == "4.2");
}

TEST(libjson_test_number, parse_float_invalid)
{
    const char* buf  = "4. ";
    size_t  max = strlen(buf);
    uint32_t line = 0;
    uint8_t* b = (uint8_t*)buf;
    json::Number* n = dynamic_cast<json::Number*>(json::Number::parse(b, max, line));
    EXPECT_TRUE( n == NULL );
}

TEST(libjson_test_number, parse_float_negative)
{
    const char* buf  = "-124.99 ";
    size_t  max = strlen(buf);
    uint32_t line = 0;
    uint8_t* b = (uint8_t*)buf;
    json::Number* n = dynamic_cast<json::Number*>(json::Number::parse(b, max, line));
    EXPECT_TRUE( n != NULL );
    EXPECT_EQ(-124.99, (double)*n);
    EXPECT_TRUE(n->str() == "-124.99");
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}