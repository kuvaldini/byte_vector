#pragma once

#include <iostream>
#include <iomanip>

/// Wrapper struct to write  with std::ostream
template<typename Iter>
struct bytes_hex {
    Iter first;
    Iter last;
    static_assert( sizeof(*first) == 1 );
//    static_assert( sizeof( typename std::iterator_traits<Iter>::value_type )== 1);

    bytes_hex()=default;
    bytes_hex(Iter f, Iter l): first{f}, last{l} {}
    bytes_hex(Iter buf, size_t len): bytes_hex(buf,buf+len){}
    template<typename C> bytes_hex(C const& c): first{c.begin()}, last{c.end()} {} //bytes_hex(c.begin(), c.end()){}

    Iter begin()const{ return first; }
    Iter end()  const{ return last; }

    friend std::ostream& operator<<(std::ostream&os, bytes_hex const& hb){
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
template<typename Container> bytes_hex(Container const& c) -> bytes_hex<decltype(c.begin())>;
