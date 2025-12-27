#ifndef ENUM_TO_STRING_HPP
#define ENUM_TO_STRING_HPP

#include <meta>


// Convert an enum value to its enumerator name at compile time.
template <typename E> requires (std::meta::is_enumerable_type(^^E))
consteval std::string_view enum_to_string(E e) {
    constexpr std::meta::info enums_refl = std::meta::reflect_constant_array(std::meta::enumerators_of(^^E));

    template for(constexpr std::meta::info enum_refl : [: enums_refl :]) {
        if(e == [: enum_refl :]) {
            return std::meta::identifier_of(enum_refl);
        }
    }

    return "";
}

#endif  // ENUM_TO_STRING_HPP
