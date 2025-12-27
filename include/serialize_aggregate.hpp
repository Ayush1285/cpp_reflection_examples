#ifndef SERIALIZE_AGGREGATE_HPP
#define SERIALIZE_AGGREGATE_HPP

#include "ctx_definition.hpp"

#include <array>
#include <cstddef>
#include <cstring>
#include <concepts>
#include <meta>
#include <type_traits>


template<typename T>
struct is_c_array : std::false_type {};

template<typename T, std::size_t N>
struct is_c_array<T[N]> : std::true_type {};

template<typename T>
inline constexpr bool is_c_array_v = is_c_array<T>::value;

template<typename T>
struct is_std_array : std::false_type {};

template<typename U, std::size_t N>
struct is_std_array<std::array<U,N>> : std::true_type {};

template<typename T>
inline constexpr bool is_std_array_v = is_std_array<T>::value;


template<typename T>
consteval bool is_serializable() {
    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>) {
        return true;
    } else if constexpr (is_std_array_v<T>) {
        return is_serializable<typename T::value_type>();
    } else if constexpr (is_c_array_v<T>) {
        return is_serializable<std::remove_extent_t<T>>();
    } else if constexpr (std::meta::is_aggregate_type(^^T)) {
        constexpr auto members = std::meta::reflect_constant_array(std::meta::nonstatic_data_members_of(^^T, ctx));

        template for (constexpr std::meta::info m : [: members :]) {
            if constexpr (!is_serializable<typename [: std::meta::type_of(m) :]>())
                return false;
        }
        return true;
    } else {
        return false;
    }
}


// Concept for types supported by this serialization.
// Supported: arithmetic, enums, std::array of supported types, C-arrays of supported types, and aggregates composed of supported members.
template<typename T>
concept Serializable = is_serializable<T>();


template<typename T>
void write_bytes(std::byte* dst, T value) {
    static_assert(std::is_trivially_copyable_v<T>);
    std::memcpy(dst, &value, sizeof(T));
}

template<typename T>
void read_bytes(const std::byte* in, T& obj) {
    static_assert(std::is_trivially_copyable_v<T>);
    std::memcpy(&obj, in, sizeof(T));
}


template<Serializable T>
consteval std::size_t serialized_size() {
    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>)
        return sizeof(T);
    else if constexpr (is_std_array<T>::value)
        return T{}.size() * serialized_size<typename T::value_type>();
    else if constexpr (is_c_array_v<T>)
        return std::extent_v<T> * serialized_size<std::remove_extent_t<T>>();
    else {
        std::size_t sum = 0;
        constexpr auto members = std::meta::reflect_constant_array(std::meta::nonstatic_data_members_of(^^T, ctx));
        template for(constexpr std::meta::info m : [: members :]) {
            sum += serialized_size<typename [: std::meta::type_of(m) :]>();
        }
        return sum;
    }
}


// Serialize into a caller-provided buffer (must have at least serialized_size<T>() bytes).
// Usage: std::array<std::byte, serialized_size<T>()> buf{}; serialize_into(obj, buf.data());
template<Serializable T>
std::size_t serialize_into(const T& obj, std::byte* out) {
    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>) {
        write_bytes(out, obj);
        return sizeof(T);
    } else if constexpr (is_std_array<T>::value) {
        using Elem = typename T::value_type;
        std::size_t offset = 0;

        for (const auto& e : obj)
            offset += serialize_into<Elem>(e, out + offset);

        return offset;
    } else if constexpr (is_c_array_v<T>) {
        using Elem = std::remove_extent_t<T>;
        constexpr std::size_t N = std::extent_v<T>;

        std::size_t offset = 0;

        for(std::size_t i = 0; i < N; ++i)
            offset += serialize_into<Elem>(obj[i], out + offset);

        return offset;
    } else {
        constexpr auto members = std::meta::reflect_constant_array(std::meta::nonstatic_data_members_of(^^T, ctx));

        std::size_t offset = 0;

        template for(constexpr std::meta::info m : [: members :]) {
            offset += serialize_into(obj.[: m :], out + offset);
        }

        return offset;
    }
}

// Deserialize from a buffer written by serialize_into (must have at least serialized_size<T>() bytes).
// Usage: T obj{}; deserialize_from(obj, buf.data());
template<Serializable T>
std::size_t deserialize_from(T& obj, const std::byte* in)
{
    if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>) {
        read_bytes(in, obj);
        return sizeof(T);
    } else if constexpr (is_std_array<T>::value) {
        using Elem = typename T::value_type;
        std::size_t offset = 0;

        for (auto& e : obj)
            offset += deserialize_from<Elem>(e, in + offset);

        return offset;
    } else if constexpr (is_c_array_v<T>) {
        using Elem = std::remove_extent_t<T>;
        constexpr std::size_t N = std::extent_v<T>;

        std::size_t offset = 0;

        for(std::size_t i = 0; i < N; ++i)
            offset += deserialize_from<Elem>(obj[i], in + offset);

        return offset;
    } else {
        constexpr auto members = std::meta::reflect_constant_array(std::meta::nonstatic_data_members_of(^^T, ctx));

        std::size_t offset = 0;

        template for(constexpr std::meta::info m : [: members :]) {
            offset += deserialize_from(obj.[: m :], in + offset);
        }

        return offset;
    }
}


#endif  // SERIALIZE_AGGREGATE_HPP
