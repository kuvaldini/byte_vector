[![pipeline status](https://gitlab.com/kyb/byte_vector/badges/master/pipeline.svg)](https://gitlab.com/kyb/byte_vector/pipelines?scope=branches) 
[![releases](https://img.shields.io/badge/byte_vector-releases-green.svg?style=flat)](https://gitlab.com/kyb/byte_vector/-/releases) 
[![Gitpod.ready-to-code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod)](https://gitpod.io/#https://gitlab.com/kyb/byte_vector) 

byte_vector for C++
===================
**byte_vector** is a family of C++ types and functions to provide next functionality:
- serialization to sequence of bytes
- deserialization from sequence of bytes
- print array of bytes in hex to any std::ostream like cout, cerr or stringstream

`byte_vector` is a struct extended from `std::vector<uint8_t>`.  
It provides ability to serialize and deserialize C++ basic types and classes 
by using family of template operators `byte_vector<<` and `byte_range>>`. 


### Example of operation
Lets consider example program `byte_vector-demo.cpp`
```cpp
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
```

This is an output from `./byte_vector-demo`
```
0x000: 00 00 01 31 8C 04 D2 C1  |...1....|
0x000: 00 00 01 31 8C 04 D2 C1 00 20  |...1..... |

0x000: 00 00 01 31 8C 04 D2 C1 00 20 00 05 48 65 6C 6C  |...1..... ..Hell|
0x010: 6F 00 06 20 77 6F 72 6C 64 21                    |o.. world!|
----------------
0x000: 00 23 C6 77 FF FF 00 05 68 65 6C 6C 6F  |.#.w....hello|
<empty>
2344567 -1 hello
```

Build and run it with:
```
cmake -Bbuild  &&  cmake --build build  &&  ./build/byte_vector-demo 
```


Integrate to user project
-----
### 1. The most straightforward way is to include this project's directory.
I suggest to use [git subrepo](https://github.com/ingydotnet/git-subrepo)
instead of git subtree and git submodule.
```
git subrepo clone git@gitlab.com:kyb/byte_vector.git byte_vector
```
Then add byte_vector project directory to includes.

### 2. Using the only header file – may be the most convenient way
Download amalgama header from the project's [releases page](https://gitlab.com/kyb/byte_vector/-/releases)
```
cd your/project/include_dir
curl -fsSL https://gitlab.com/kyb/byte_vector/raw/artifacts/master/$(curl https://gitlab.com/kyb/byte_vector/raw/artifacts/master/byte_vector.hh -fsSL) -o byte_vector.hh
```

### 3. Using CMake `target_link_libraries` with interface library
*Assuming the repo has been already cloned as sugested in (1)*  
Since `byte_vector` this is header-only library there is no need to link with it.
But CMake has feature INTERFACE LIBRARIES. byte_vector provides two interface 
libraries: `byte_vector` and `byte_vector_amalgama`. Linking against them addes 
include directory to the target.
1. `byte_vector`
    ```
    target_link_libraries(your_executable PRIVATE byte_vector)
    ```
    ```
    #include "byte_vector.hpp"
    #include "byte_range.hpp"
    #include "byte_range_ascii.hpp"
    #include "byte_range_hex.hpp"
    ```
2. `byte_vector_amalgama`
    ```
    target_link_libraries(your_executable PRIVATE byte_vector_amalgama)
    ```
    ```
    #include "byte_vector.hh"
    ```
