#if ! __cplusplus > 201703L
#error "C++20 required"
#endif

#include "byte_vector_view.hpp"
#include "std_to_ostream.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace std::literals;

// template<size_t N>
// constexpr bool operator==(std::array<std::string_view,N> const& asv, std::array<std::string_view,N> const& asv2){
//    auto a = begin(asv);
//    auto i = begin(asv2);
//    while(a!=end(asv) and i!=end(asv2) and *a==*i){
//       ++a, ++i;
//    }
//    return (a==end(asv) and i==end(asv2));
// }

template<class C1, class C2> //requires C1::size()->size_t, C2::size()->size_t, is_same_v<C1::value_type,C2::value_type>
constexpr auto operator==(C1 const& vs1, C2 const& vs2)
->bool //std::enable_if_t< std::is_same_v<C1::value_type, C2::value_type> ,bool>
{
   //static_assert(std::is_same_v<C1::value_type, C2::value_type>);
   if(vs1.size() != vs1.size())
      return false;
   auto a = begin(vs1);
   auto i = begin(vs2);
   while(a!=end(vs1) and i!=end(vs2) and *a==*i){
      ++a, ++i;
   }
   return (a==end(vs1) and i==end(vs2));
}

TEST(byte_vector_view, first)
{
   constexpr string_view input = "asdfghjkl1\0\n1234567890"sv;
   byte_vector_view bvv {input};
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

TEST(byte_vector_view, second)
{
   constexpr char input[] = "asdfghjkl123456789";
   byte_vector_view bvv {input};
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

TEST(byte_vector_view, integral_t)
{
   constexpr string_view input = "as\x12\x34\xab\xcd\x11\x22\x33\x44";
   byte_vector_view bvv {input};
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

TEST(byte_vector_view, not_enough_bytes)
{
   byte_vector_view bvv {"input"};
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
