#pragma once

#include <tuple>
#include <cxxabi.h>
#include <string_view>
#include <optional>

template <typename>          constexpr bool is_tuple           = false;
template<typename ... types> constexpr bool is_tuple<std::tuple<types...>>       = true;
template<typename ... types> constexpr bool is_tuple<std::tuple<types...>&>      = true;
template<typename ... types> constexpr bool is_tuple<std::tuple<types...>const>  = true;
template<typename ... types> constexpr bool is_tuple<std::tuple<types...>const&> = true;
template<typename ... types> constexpr bool is_tuple<std::tuple<types...>&&>     = true;
template<typename T> constexpr bool is_tuple_f(T){ return is_tuple<T>; }

template<typename>   constexpr bool is_optional_impl = false;
template<typename T> constexpr bool is_optional_impl<std::optional<T>> = true;
template<>           constexpr bool is_optional_impl<std::nullopt_t> = true;
template<typename T> constexpr bool is_optional = is_optional_impl<std::remove_cv_t<std::remove_reference_t<T>>>;
static_assert(not is_optional<int>);
static_assert(is_optional<std::optional<int>>);
static_assert(is_optional<std::optional<int>&>);
static_assert(is_optional<std::optional<int>const&>);

template<typename T, template<typename> class TT> struct is_instantiation_of            : std::false_type {};
template<typename T, template<typename> class TT> struct is_instantiation_of<TT<T>, TT> : std::true_type {};


template<typename,typename=void> constexpr bool has_field_value = false;
template<typename T>             constexpr bool has_field_value<T,decltype(T::value,void())> = true;

template<typename,typename=void> constexpr bool has_name = false;
template<typename T>             constexpr bool has_name<T,decltype(T::name,void())> = true;


template <typename T, typename Tuple>  struct has_type;
template <typename T, typename... Us>  struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {};
//template <typename T, typename... Us> constexpr auto has_type_v = has_type<T, std::tuple<Us...>>::value;  //DOES NOT WORK

//std::string demangle(type_info const& ti){
//    char buf[128];
//    size_t size = 128;
//    int status;
//    abi::__cxa_demangle(ti.name(),buf,&size,&status);
//    return buf;
//}

inline static const char* demangle(const char *s) {
    return abi::__cxa_demangle(s, nullptr, nullptr, nullptr);
}

template<typename T>
inline static const char* type_name() {
    return demangle(typeid(T).name());
}
template<typename T>
inline static const char* type_name(T&&v) {
    return demangle(typeid(std::forward<T>(v)).name());
}

inline static std::string_view short_type_name(const char* name) {
    std::string_view tname{name};
    size_t tn = tname.find_last_of(':');
    if(tn != std::string_view::npos){
        if(tn+1 < tname.size())
            return tname.substr(tn+1);
        else
            return "";
    }
    return tname;
}
template<typename T>
inline static std::string_view short_type_name() {
    return short_type_name(type_name<T>());
}
template<typename T>
inline static std::string_view short_type_name(T&&v) {
    return short_type_name(type_name(std::forward<T>(v)));
}

template<typename What, typename ... Args>
struct is_present {
    static constexpr bool value {(std::is_same_v<What, Args> || ...)};
};
template<typename What,typename...Args> constexpr bool is_present_v = is_present<What,Args...>::value;
