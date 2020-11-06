#pragma once

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


#include "local_type_traits.hpp"
template<typename T> //c++20 requires T.value
inline auto operator<<(byte_vector&vec, T const& arg) ->std::enable_if_t<has_field_value<T>, byte_vector&>
{
    return vec << arg.value;
}

#if 0
#include <iostream>
#include "byte_range_ascii.hpp"

template <typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, byte_vector const& bv){
    return os << byte_range_ascii(bv);
}
#endif
