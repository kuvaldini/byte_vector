#include "byte_range_hex.hpp"
#include <sstream>
#include "gtest/gtest.h"

using namespace std;


TEST(byte_range_hex,first)
{
   constexpr char buf[] = "Hello\n\b\a\eWorld\f\x11. This is a very first test of byte_range_ascii.";
   constexpr char expected_hex[] = "48656C6C6F0A08071B576F726C640C112E2054686973206973206120766572792066697273742074657374206F6620627974655F72616E67655F61736369692E00";
   std::stringstream ss;
   byte_range_hex hex {buf,sizeof(buf)};
   ss<< hex;
   EXPECT_EQ(ss.str() , expected_hex);
}
