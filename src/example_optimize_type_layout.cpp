#include "ctx_definition.hpp"
#include "optimize_type_layout.hpp"

#include <iostream>

struct BadLayout {
    bool flag1;
    double value1;
    bool flag2;
    int id;
    bool flag3;
    double value2;
    char tag;
};

struct JointState {
    bool enabled;
    bool has_limits;
    float min_limit;
    float max_limit;
    double position;
    double velocity;
    double effort;
    int id;
};

consteval {
    optimize_type_layout<BadLayout>();
}

using OptimizedBadLayout = optimized_type_impl;

int main() {
    std::cout << "BadLayout original:" << std::endl;
    std::cout << "  sizeof: " << sizeof(BadLayout) << " bytes" << std::endl;
    std::cout << "  alignof: " << alignof(BadLayout) << std::endl;
    std::cout << std::endl;

    std::cout << "OptimizedBadLayout using optimize_type_layout:" << std::endl;
    std::cout << "  sizeof: " << sizeof(OptimizedBadLayout) << " bytes" << std::endl;
    std::cout << "  alignof: " << alignof(OptimizedBadLayout) << std::endl;
    std::cout << std::endl;

    std::cout << "Space saved: " << (sizeof(BadLayout) - sizeof(OptimizedBadLayout)) << " bytes" << std::endl;

    return 0;
}

