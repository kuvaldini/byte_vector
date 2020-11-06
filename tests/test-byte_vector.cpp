#include "byte_vector.hpp"
#include "byte_range_view.hpp"
#include "byte_range_hex.hpp"
#include "byte_range_ascii.hpp"
#include "gtest/gtest.h"
#include <iostream>

using namespace std;
using namespace std::literals;

TEST(byte_vector,integral)
{
   byte_vector bv;
    int8_t  c=0x67, c1;
   uint8_t  d=0x76, d1;
    int16_t s=0x1234, s1;
   uint16_t t=0xabcd, t1;
    int32_t i=0x12345678, i1;
   uint32_t u=0xabcd4321, u1;
    int64_t l=0x0987654321abcdef, l1;
   uint64_t k=0xabcdef1234567890, k1;
   bv << c << d << s << t << i << u << l << k;
   EXPECT_EQ(bv.size(), sizeof(c)+sizeof(d)+sizeof(s)+sizeof(t)+sizeof(i)+sizeof(u)+sizeof(l)+sizeof(k));
   
   byte_vector bv2 = bv;
   EXPECT_EQ(bv,bv2);
   
   byte_range_view bvv{bv};
   bvv >> c1 >> d1 >> s1 >> t1 >> i1 >> u1 >> l1 >> k1;
   EXPECT_EQ(bvv.size(), 0);
   EXPECT_TRUE(bvv.empty());
   EXPECT_EQ(c,c1);
   EXPECT_EQ(d,d1);
   EXPECT_EQ(s,s1);
   EXPECT_EQ(t,t1);
   EXPECT_EQ(i,i1);
   EXPECT_EQ(u,u1);
   EXPECT_EQ(l,l1);
   EXPECT_EQ(k,k1);
}

TEST(byte_vector,string)
{
   byte_vector bv;
   const char* cstr = "Hello byte_vector";
   bv<<int(strlen(cstr))<<cstr;
   
   byte_range_view bvv{bv};
   int slen;
   string s;
   bvv >> slen >> s;
   bv.resize(bvv.size());
   ASSERT_TRUE(bvv.good());
   EXPECT_EQ(slen,strlen(cstr));
   EXPECT_EQ(s,cstr);
   EXPECT_EQ(bv.size(), 0);
   EXPECT_TRUE(bv.empty());
   
   bv<<string_view{"No place for peace"};
   byte_range_view{bv} >> s;
   EXPECT_EQ(s,"No place for peace");
   
   byte_vector bv2;
   bv2<<"There is ";
   bv2<<bv;
   string s1;
   byte_range_view{bv2} >> s >> s1;
   EXPECT_TRUE(bvv.good());
   EXPECT_EQ(s+s1,"There is No place for peace");
   EXPECT_TRUE(bvv.empty());
}

TEST(byte_vector,tup)
{
   string str = "myname";
   auto t = tuple{123,str,456};//,vector{4,5,6}};
   byte_vector bv;
   bv << t;
   decltype(t) to;
   byte_range_view bvv{bv};
   bvv >> to;
   EXPECT_EQ(t,to);
// cerr<<byte_range_ascii(bvv)<<endl;
}

TEST(byte_vector,opt)
{
   byte_vector bv;
   bv << 32144 << optional<int>{};
   
   EXPECT_EQ(bv.size(),sizeof(int));

   // optional<int> oi;
   //byte_range_view{bv} >> oi;
   
}

TEST(byte_vector,ascii)
{
   byte_vector bv;
   bv << "Hello byte_vector";
   constexpr auto expected = "\n"
         "0x000: 00 11 48 65 6C 6C 6F 20 62 79 74 65 5F 76 65 63  |..Hello byte_vec|\n"
         "0x010: 74 6F 72                                         |tor|";
   stringstream ss;
   ss<<byte_range_ascii{bv};
   EXPECT_EQ(ss.str(),expected);
   
   bv.clear();
   bv << "Short";
   constexpr auto expected_short = "0x000: 00 05 53 68 6F 72 74  |..Short|";  // No new line before.
   ss = {};
   ss<<byte_range_ascii{bv};
   EXPECT_EQ(ss.str(),expected_short);
}

TEST(byte_vector,hex)
{
   byte_vector bv;
   bv << "Hello byte_vector";
   constexpr auto expected = "001148656C6C6F20627974655F766563746F72";
   stringstream ss;
   ss<<byte_range_hex{bv};
   EXPECT_EQ(ss.str(),expected);
   
   bv.clear();
   bv << "Short";
   constexpr auto expected_short = "000553686F7274";  // No new line before.
   ss = {};
   ss<<byte_range_hex{bv};
   EXPECT_EQ(ss.str(),expected_short);
}
