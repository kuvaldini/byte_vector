#include "byte_range_ascii.hpp"
#include <sstream>
#include "gtest/gtest.h"

using namespace std;


TEST(byte_range_ascii,first)
{
   constexpr char buf[] = "Hello\n\b\a\eWorld\f\x11. This is a very first test of byte_range_ascii.";
   constexpr char expected_ascii[] = "\n0x000: 48 65 6C 6C 6F 0A 08 07 1B 57 6F 72 6C 64 0C 11  |Hello....World..|\n"
                                       "0x010: 2E 20 54 68 69 73 20 69 73 20 61 20 76 65 72 79  |. This is a very|\n"
                                       "0x020: 20 66 69 72 73 74 20 74 65 73 74 20 6F 66 20 62  | first test of b|\n"
                                       "0x030: 79 74 65 5F 72 61 6E 67 65 5F 61 73 63 69 69 2E  |yte_range_ascii.|\n"
                                       "0x040: 00                                               |.|";
   std::stringstream ss;
   byte_range_ascii bra {buf,sizeof(buf)};
   ss<< bra;
   EXPECT_EQ(ss.str() , expected_ascii);
}
