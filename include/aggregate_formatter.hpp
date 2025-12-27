#ifndef AGGREGATE_FORMATTER_HPP
#define AGGREGATE_FORMATTER_HPP

#include <meta>
#include <sstream>
#include <string>

template<typename T>
consteval bool is_aggregate() {
    return std::meta::is_aggregate_type(^^T);
}


// Format an aggregate object by iterating its non-static data members via reflection.
// (members must be streamable).
template<typename T>
std::string format_aggregate(const T& obj) {
    constexpr auto members = std::meta::reflect_constant_array(std::meta::nonstatic_data_members_of(^^T, ctx));

    std::ostringstream out;
    out << std::meta::identifier_of(^^T) << "{ ";

    bool first = true;

    template for(constexpr std::meta::info m : [: members :])
    {
        if (!first) {
            out << ", ";
        }
        first = false;

        auto name = std::meta::identifier_of(m);
        out << name << "=" << obj.[: m :];
    }

    out << " }";
    return out.str();
}


// Build a reusable formatter lambda for a type.
// Usage: static constexpr auto fmt = build_aggregate_formatter<Pose>(); std::cout << fmt(pose);
template<typename T>
consteval auto build_aggregate_formatter() {
    constexpr auto members = std::meta::reflect_constant_array(std::meta::nonstatic_data_members_of(^^T, ctx));

    return [=](const T& obj) {
        std::ostringstream out;
        out << std::meta::identifier_of(^^T) << "{ ";

        bool first = true;

        template for(constexpr std::meta::info m : [: members :]) {
            if(!first) out << ", ";
            first = false;

            auto name = std::meta::identifier_of(m);
            using M = [: std::meta::type_of(m) :];

            out << name << " = ";

            if constexpr (is_aggregate<M>()) {
                static constexpr auto nested = build_aggregate_formatter<M>();
                out << nested(obj.[: m :]);
            } else {
                out << obj.[: m :];
            }
        }

        out << " }";
        return out.str();
    };
}


#endif  // AGGREGATE_FORMATTER_HPP

