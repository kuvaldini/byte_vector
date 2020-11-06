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

#pragma once

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
