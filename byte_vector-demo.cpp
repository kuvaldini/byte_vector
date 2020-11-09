#include "byte_vector.hh"

using namespace std;
using namespace std::literals;

struct X{
   int a=1111111;
   short b =2;
   string c = "asd";
   
   friend ostream& operator<<(ostream&os, X const& x){
      return os<<x.a<<" "<<x.b<<" "<<x.c;
   }
   friend byte_vector& operator<<(byte_vector&bv, X const& x){
      return bv<<tuple(x.a,x.b,x.c); 
      //OR return bv<<x.a<<x.b<<x.c;
   }
   friend byte_range& operator>>(byte_range&br, X&x){
      return br>>tie(x.a,x.b,x.c);
      //OR return br>>x.a>>x.b>>x.c;
   }
};

int main()
{
   {
   byte_vector bv;
   bv<<1312314151617;
   cout<<byte_range_ascii{bv}<<endl;             /// 0x000: 00 00 01 31 8C 04 D2 C1  |...1....|
   cout<<byte_range_ascii{bv<<short(32)}<<endl;  /// 0x000: 00 00 01 31 8C 04 D2 C1 00 20  |...1..... |
   bv<< "Hello"s << " world"sv << '!' ;
   cout<<byte_range_ascii{bv}<<endl;             /// 0x000: 00 00 01 31 8C 04 D2 C1 00 20 00 05 48 65 6C 6C  |...1..... ..Hell|
   }                                             /// 0x010: 6F 00 06 20 77 6F 72 6C 64 21                    |o.. world!|
   cerr<<"----------------"<<endl;               /// ----------------
   {
   /// Serialize custom struct
   byte_vector bv; //bv={};
   bv<<X{2344567,-1,"hello"};
   cout<<byte_range_ascii(bv)<<endl;             /// 0x000: 00 23 C6 77 FF FF 00 05 68 65 6C 6C 6F  |.#.w....hello|
   byte_range br(bv);
   /// Deserialize custom struct
   X x;
   br>>x;
   (br.size()? cout<<byte_range_ascii(br) : cout<<"<empty>")<<endl;  /// <empty>
   cout<<x<<endl;                                /// 2344567 -1 hello
   }
}
