#include "byte_range_view.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace std::literals;


TEST(byte_range_view, first)
{
   constexpr string_view input = "asdfghjkl1\0\n1234567890"sv;
   byte_range_view bvv {input};
   EXPECT_EQ(bvv.size(), input.size());
   
   char c;
   bvv>>c;
   EXPECT_EQ(c,input[0]);
   bvv>>c;
   EXPECT_EQ(c,input[1]);
   
   int i;
   bvv>>i;
   EXPECT_EQ(i, 1684432744);
   bvv>>i;
   EXPECT_EQ(i, 1785424945);
   
   string s;
   bvv>>s;
   EXPECT_EQ(s, input.substr(12));
   
   EXPECT_TRUE(bvv.empty());
}

TEST(byte_range_view, second)
{
   constexpr char input[] = "asdfghjkl123456789";
   byte_range_view bvv {input};
   EXPECT_EQ(bvv.size(), sizeof(input)-1);
   
   char c;
   bvv>>c;
   EXPECT_EQ(c,input[0]);
   bvv>>c;
   EXPECT_EQ(c,input[1]);
   
   int i;
   bvv>>i;
   EXPECT_EQ(i, 1684432744);
   bvv>>i;
   EXPECT_EQ(i, 1785424945);
   
   EXPECT_EQ(bvv.size(), sizeof(input)-1-2-8);
   EXPECT_FALSE(bvv.bad());
   EXPECT_TRUE(bvv.good());
   
   bvv.clear();
   bvv>>i;
   EXPECT_EQ(bvv.size(), 0);
   EXPECT_TRUE(bvv.bad());
   EXPECT_FALSE(bvv.good());
}

TEST(byte_range_view, integral_t)
{
   constexpr string_view input = "as\x12\x34\xab\xcd\x11\x22\x33\x44";
   byte_range_view bvv {input};
   EXPECT_EQ(bvv.size(), input.size());
   
   char c;
   bvv>>c;
   EXPECT_EQ(c,input[0]);
   bvv>>c;
   EXPECT_EQ(c,input[1]);
   
   uint16_t s1,s2;
   bvv>>s1>>s2;
   EXPECT_EQ(s1, 0x1234);
   EXPECT_EQ(s2, 0xabcd);
   
   int32_t i1;
   bvv>>i1;
   EXPECT_EQ(i1, 0x11223344);

   EXPECT_TRUE(bvv.empty());
   
   string str;
   bvv>>str;
   EXPECT_TRUE(bvv.bad());
   EXPECT_TRUE(str.empty());
   // EXPECT_EQ(str, ""); //input.substr(2+4+4));
}

TEST(byte_range_view, not_enough_bytes)
{
   byte_range_view bvv {"input"};
   int32_t i;
   bvv>>i;
   char c;
   bvv>>c;
   EXPECT_TRUE(bvv.empty());
   i=32;
   bvv>>i;
   EXPECT_EQ(i,32);
   EXPECT_TRUE(bvv.bad());
}
