#pragma once

#define BYTE_VECTOR_REV_LABEL "master-c9-gc218aa1"
#define BYTE_VECTOR_VERSION BYTE_VECTOR_REV_LABEL

/////////////  byte_vector.hpp  /////////////////

#include <vector>
#include <cstdint>

// using byte_vector = std::vector<uint8_t>;
struct byte_vector : std::vector<uint8_t> {
    using strsize_t = uint16_t;
    // using std::vector<uint8_t>::std::vector<uint8_t>;
};

template<typename T>
inline auto operator<<(byte_vector&bv, T const& v) ->std::enable_if_t<std::is_integral_v<T>, byte_vector& >
{
    static_assert(std::is_integral_v<T>,"operator<<(byte_vector&,T) implemented for integral types only");
    //ToDo C++20 if constexpr (std::endian::native == std::endian::little)
#if BYTE_ORDER == LITTLE_ENDIAN
    std::reverse_iterator<uint8_t*> first{(uint8_t *) (&v + 1)}, last{(uint8_t *) (&v)};
#else
    uint8_t *first=(uint8_t*)&v, *last=(uint8_t*)(&v+1);
#endif
    bv.insert(end(bv), first, last);
    return bv;
}

inline auto operator<<(byte_vector&vec, byte_vector const& bv) ->byte_vector&
{
    vec.insert(vec.end(), bv.begin(), bv.end());
    return vec;
}

#include <string_view>
inline auto operator<<(byte_vector&vec, std::string_view const& sv) ->byte_vector&
{
    vec<<byte_vector::strsize_t(sv.size());
    vec.insert(vec.end(), sv.begin(), sv.end());  // Alternative way: vec.reserve(vec.size()+sv.size()); for(auto&c:sv) vec<<c;
    return vec;
}

inline auto operator<<(byte_vector&vec, char const* str) ->byte_vector&
{
    //ToDo optimization wanted
    // while(*str!='\0')
    //     vec<<*str++;
    // return vec;
    return vec<<std::string_view(str);
}
template<size_t N>
inline auto operator<<(byte_vector&vec, char const str[N]) ->byte_vector&
{
    return vec<<std::string_view(str,N-1);
}

#include <optional>
template<typename T>
inline auto operator<<(byte_vector&vec, std::optional<T> const& arg) ->byte_vector&
{
    if(arg)
        vec << *arg;
    return vec;
    //return arg ? vec << arg->value : vec;
}

#include <tuple>
template<typename... Ts>
inline auto operator<<(byte_vector&vec, std::tuple<Ts...> const& theTuple) ->byte_vector&
{
    std::apply(
        [&](Ts const &... tupleArgs) {
            ((vec << tupleArgs), ...);
        }, theTuple
    );
    return vec;
}

#if 0
#include <iostream>
#include "byte_range_ascii.hpp"

template <typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, byte_vector const& bv){
    return os << byte_range_ascii(bv);
}
#endif

/////////////  byte_range_ascii.hpp  /////////////////
/**
 * A wrapper class around range of bytes to be printed 
 * as hex table like xxd, hexdump, and wireshark does.
 * 
 * Example:
 *   0x000: 48 65 6C 6C 6F 0A 08 07 1B 57 6F 72 6C 64 0C 11  |Hello....World..|
 *   0x010: 2E 20 54 68 69 73 20 69 73 20 61 20 76 65 72 79  |. This is a very|
 *   0x020: 20 66 69 72 73 74 20 74 65 73 74 20 6F 66 20 62  | first test of b|
 *   0x030: 79 74 65 5F 72 61 6E 67 65 5F 61 73 63 69 69 2E  |yte_range_ascii.|
 *   0x040: 00                                               |.|
 */


#include <iostream>
#include <iomanip>

template<typename Iter>
struct byte_range_ascii {
    static_assert( sizeof( typename std::iterator_traits<Iter>::value_type )== 1);
    using type = byte_range_ascii<Iter>;
    Iter first, last=first;
    bool oneline = (std::distance(begin(),end()) <= 16);
    size_t indent = 0;
    bool prenewline = not oneline;
    byte_range_ascii()=default;
    byte_range_ascii(Iter f, Iter l): first{f}, last{l} {}
    byte_range_ascii(std::pair<Iter,Iter> fl): byte_range_ascii(fl.first,fl.second) {}
    byte_range_ascii(Iter buf, size_t len): byte_range_ascii(buf,buf+len){}
    template<typename C> byte_range_ascii(C const& c): byte_range_ascii(c.begin(), c.end()){}
    Iter begin()const{ return first; }
    Iter end()  const{ return last; }
};
template<typename Container> byte_range_ascii(Container const& c) -> byte_range_ascii<decltype(c.begin())>;

template <typename T, typename Iter>
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, byte_range_ascii<Iter>const& range)
{
    using namespace std;

    auto prev_os_format = os.flags();
    auto prev_os_fill = os.fill();

    auto const &[begin, end, oneline, indent, prenewline] = range;
    if(prenewline)
        os << "\n";
    os << std::setw(2) << std::setfill('0') << std::hex << std::uppercase;
    string ascii;
    auto iter = begin;
    const char *newline = ""; // append \n at the begining of every line except first one, not at the end.
    while (iter != end)
    {
        os << setw(0) << newline;
        if(indent > 0)
            os << setfill(' ') << setw(indent) << " ";
        unsigned short offset = iter - begin;
        os << "0x" << setfill('0') << setw(3) << std::right << offset << ": ";

        auto const line_end = iter + 16;
        while (iter != line_end && iter != end)
        {
            ascii += isprint(*iter) ? *iter : '.';
            os << setw(2) << int(*iter) << " ";
            ++iter;
        }
        if(not oneline)
            // align ascii representation in last line
            for (int i = 0; i < line_end - iter; ++i)
                os << "   ";

        os << " |" << ascii << "|";  //os << " |" << setfill(' ') << setw(16) << std::left << ascii << "|";
        ascii = "";
        newline = "\n";
    }

    os.flags(prev_os_format);
    os.fill(prev_os_fill);
    return os;
}

/////////////  byte_range.hpp  /////////////////

#include <string>
#include <type_traits>
#include <iostream>

struct byte_range {
    using iterator = const unsigned char*;
    using siterator = const signed char*;
    iterator first, last=first;
    bool error_flag_=false;
    byte_range(byte_range const& bvv):first(bvv.first),last(bvv.last),error_flag_(bvv.error_flag()){}
    byte_range(byte_range     && bvv):byte_range(bvv){}
    byte_range(iterator beg, size_t len): first(beg),last(beg+len){}
    byte_range(iterator beg, iterator end): first(beg),last(end){}
    byte_range(siterator beg, size_t len): first((iterator)beg),last((iterator)beg+len){}
    //byte_range(byte_vector const& bv):first(bv.data()),last(bv.data()+bv.size()){}
    //template<typename C> byte_range(C const& c): byte_range(c.begin(), c.end()){}
    template<typename C> byte_range(C const& c): first((iterator)c.data()), last((iterator)c.data()+c.size()) {}
    template<size_t N> byte_range(char const (&c)[N]): first((iterator)c), last((iterator)(c+N-1)) {}
    iterator begin()const{return first;}
    iterator end()const{return last;}
    size_t size()const{return std::distance(first,last);}
    size_t empty()const{return size()==0;}
    void clear(){ first=last; }
    operator std::pair<iterator,iterator>(){
        return {first,last};
    }
    /// Calling front on an empty container is undefined.
    uint8_t front() const{
        return *first;
    }
    /// Calling back on an empty container is undefined.
    uint8_t back()const{
        return *(last-1);
    }
    uint8_t const* data()const{ return first; }
    operator bool()const{ return good(); }
    bool good()const{ return not error_flag_; }
    bool bad()const{ return error_flag_; }
    bool error_flag()const{ return error_flag_; }
    bool& error_flag(){ return error_flag_; }
};
//template<typename Container> byte_range(Container const&) -> byte_range<decltype(c.begin())>; //deduction guide

// /// to be able for rvalues byte_range, returns a copy
// template<class T>
// inline byte_range operator>>(byte_range&&bvv, T&v){
//     return bvv >> v;
// }

template<typename T>
inline static auto operator>>(byte_range&bvv, T&val)
    ->std::enable_if_t< std::is_integral_v<T>,  byte_range& >
{
    using namespace std;
    if(bvv.size() < sizeof(T)){
        cerr<<"err in "<<__func__<<"(): input stream is too short (size:"<<bvv.size()
            <<", requested:"<< sizeof(T)<<")"<<endl;
        bvv.error_flag() = true;
        return bvv;
    }
 #if BYTE_ORDER == LITTLE_ENDIAN
    std::reverse_iterator<uint8_t*> out_it{(uint8_t*)(&val+1)}, out_end{(uint8_t*)(&val)};
 #else
    uint8_t* out_it{(uint8_t*)(&val)}, *out_end{(uint8_t*)(&val+1)};
 #endif
    while (out_it != out_end && bvv.first != bvv.last) {
        *out_it++ = *bvv.first++;
    }
    return bvv;
}

inline static byte_range& operator>>(byte_range&bvv, std::string&str){
    uint16_t /* byte_vector::strsize_t */ strsize;
    bvv>>strsize;
    if(bvv.bad() or strsize > bvv.size())
        return bvv;
    str = std::string{(char*)&*bvv.begin(),strsize};
    bvv.first+=strsize;
    return bvv;
}

inline static byte_range& operator>>(byte_range&bvv, uint8_t&ui8){
    ui8 = *bvv.first++;
    return bvv;
}

#include <tuple>
template<typename...Ts>
inline static byte_range& operator>>(byte_range&bvv, std::tuple<Ts...>&tu)
{
    apply(
        [&](auto&...elements) {
            ((bvv>>elements),...);
        }, tu
    );
    return bvv;
}
/// and rvalue
template<typename...Ts>
inline static byte_range& operator>>(byte_range&bvv, std::tuple<Ts...>&&tu)
{
    return bvv >> tu;
}


//////////////////////////
template<typename UnknownType>
struct Type_Not_Supported : std::false_type {};

/// A fallback function template to stop compiler try different combinations of types, reduce compiler output.
template<typename UnknownType>
inline static auto operator>>(byte_range&bvv, UnknownType&&) ->byte_range& {
    static_assert(Type_Not_Supported<UnknownType>{});
    return bvv;
}

/////////////  byte_range_hex.hpp  /////////////////

#include <iostream>
#include <iomanip>

/// Wrapper struct to write  with std::ostream
template<typename Iter>
struct byte_range_hex {
    Iter first;
    Iter last;
    static_assert( sizeof(*first) == 1 );
//    static_assert( sizeof( typename std::iterator_traits<Iter>::value_type )== 1);

    byte_range_hex()=default;
    byte_range_hex(Iter f, Iter l): first{f}, last{l} {}
    byte_range_hex(Iter buf, size_t len): byte_range_hex(buf,buf+len){}
    template<typename C> byte_range_hex(C const& c): first{c.begin()}, last{c.end()} {} //byte_range_hex(c.begin(), c.end()){}
    template<size_t N> byte_range_hex(const char str[N]): byte_range_hex(str,str+N){} //first{str}, last{first+N} {} /// String literal

    Iter begin()const{ return first; }
    Iter end()  const{ return last; }

    friend std::ostream& operator<<(std::ostream&os, byte_range_hex const& hb){
        using namespace std;
        auto prev_os_format = os.flags();
        auto prev_os_fill = os.fill();
        os<<setfill('0')<<hex<<uppercase;
        for(auto i=hb.first; i!=hb.last; ++i)
            os<<setw(2)<<int(*i);
        os.flags(prev_os_format);
        os.fill(prev_os_fill);
        return os;
    }
};
template<typename Container> byte_range_hex(Container const& c) -> byte_range_hex<decltype(c.begin())>;
