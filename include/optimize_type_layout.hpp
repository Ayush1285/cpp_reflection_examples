#ifndef OPTIMIZE_TYPE_LAYOUT_HPP
#define OPTIMIZE_TYPE_LAYOUT_HPP

#include <meta>
#include <vector>
#include <algorithm>


struct optimized_type_impl;

// Return member specs sorted by (alignment desc, size desc, name asc) for the given reflected type.
consteval std::vector<std::meta::info> sorted_members_of(std::meta::info type) {
    auto members = std::meta::nonstatic_data_members_of(type, ctx);

    std::sort(members.begin(), members.end(), [](std::meta::info a, std::meta::info b) consteval {

            auto ta = std::meta::type_of(a);
            auto tb = std::meta::type_of(b);

            std::size_t align_a = std::meta::alignment_of(ta);
            std::size_t align_b = std::meta::alignment_of(tb);

            if (align_a != align_b)
                return align_a > align_b;

            std::size_t size_a = std::meta::size_of(ta);
            std::size_t size_b = std::meta::size_of(tb);

            if (size_a != size_b)
                return size_a > size_b;

            return std::meta::identifier_of(a) < std::meta::identifier_of(b);
        });

    std::vector<std::meta::info> new_members = {};
    for (std::meta::info member : members) {
        auto mem_descr = std::meta::data_member_spec(std::meta::type_of(member), {.name = std::meta::identifier_of(member)});
        new_members.push_back(mem_descr);
    }
    return new_members;
}

template<typename T>
consteval void optimize_type_layout()
{
    // Usage: call in a consteval { } block, then alias optimized_type_impl to a name.
    // Example: consteval { optimize_type_layout<MyType>(); } using MyOptimized = optimized_type_impl;
    auto sorted = sorted_members_of(^^T);
    std::meta::define_aggregate(^^optimized_type_impl, sorted);
}


#endif  // OPTIMIZE_TYPE_LAYOUT_HPP

