#ifndef STRUCTURE_OF_ARRAYS_HPP
#define STRUCTURE_OF_ARRAYS_HPP

#include <meta>
#include <vector>

template <typename T, std::size_t N>
struct struct_of_arrays_impl;

template<typename T, size_t N>
using struct_of_arrays = struct_of_arrays_impl<T, N>;

consteval std::vector<std::meta::info> make_member_specs(std::meta::info type, std::meta::info N) {
    std::vector<std::meta::info> old_members = std::meta::nonstatic_data_members_of(type, ctx);
    std::vector<std::meta::info> new_members = {};

    for (std::meta::info member : old_members) {
        auto type_array = std::meta::substitute(^^std::array, {std::meta::type_of(member), N });
        auto mem_descr = std::meta::data_member_spec(type_array, {.name = std::meta::identifier_of(member)});
        new_members.push_back(mem_descr);
    }

    return new_members;
}


template<typename T, std::size_t N>
consteval void make_struture_of_array()
{
    // Usage: call in a consteval { } block, then alias struct_of_arrays<T, N>.
    // Example: consteval { make_struture_of_array<Point, 1024>(); } using PointsSoA = struct_of_arrays<Point, 1024>;
    auto t = ^^T;
    auto n = std::meta::reflect_constant(N);

    auto members = make_member_specs(t, n);

    std::meta::define_aggregate(std::meta::substitute(^^struct_of_arrays_impl, {t, n}), members);
}

#endif  // STRUCTURE_OF_ARRAYS_HPP

