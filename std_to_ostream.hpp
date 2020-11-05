#pragma once

#include <type_traits>
#include <optional>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <initializer_list>
#include <string_view>
#include <vector>
#include "local_type_traits.hpp"


template<typename... Ts>
inline std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> const& theTuple)
{
    std::apply(
        [&os](Ts const &... tupleArgs) {
            os << '[';
            std::size_t n{0};
            ((os << /*short_type_name(tupleArgs) <<":"<<*/ tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
            os << ']';
        }, theTuple
    );
    return os;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, std::optional<T> const& opt)
{
    if(std::nullopt == opt)
        os << "nullopt";
    else
        os << *opt;
    return os;
}

inline auto operator<<(std::ostream&os, std::vector<std::string_view> const& lsv)->std::ostream&{
    const char* space = "";
    for(auto&sv : lsv){
        os<<space<<sv;
        space = " ";
    }
    return os;
}

template<size_t N>
inline auto operator<<(std::ostream&os, std::array<std::string_view,N> const& lsv)->std::ostream&{
    const char* space = "";
    for(auto&sv : lsv){
        os<<space<<sv;
        space = " ";
    }
    return os;
}

// Append space after if not empty
template<class T, class A>
struct append{
    friend std::ostream& operator<<(std::ostream&os, append<T,A> const& ap){
        std::stringstream ss;
        auto prev_tellp = ss.tellp();
        ss<< ap.value_ref;
        if(ss.tellp()>prev_tellp)  // if result not empty
            ss<< ap.app;
        return os<< ss.str();
    }
//    append(T const& valref, A&&a) : value_ref(valref), app(a) {}
    append(T const& valref, A const&a) : value_ref(valref), app(a) {}
    T const& value_ref;
    A const& app;
};

// Append space after if not empty
template<typename T>
struct with_space{
    friend std::ostream& operator<<(std::ostream&os, with_space<T> const& ws){
        std::stringstream ss;
        auto prev_tellp = ss.tellp();
        ss<<ws.value_ref;
        if(ss.tellp()>prev_tellp)
            ss<<" ";
        return os<<ss.str();
    }
    with_space(T const& valref):value_ref(valref){}
    T const& value_ref;
};
