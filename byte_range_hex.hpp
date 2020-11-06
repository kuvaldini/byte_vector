#pragma once

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
