#include "ctx_definition.hpp"
#include "structure_of_arrays.hpp"

#include <array>
#include <cstddef>
#include <iostream>

constexpr size_t ARRAY_SIZE = 10;

struct Point {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct Particle {
    double position_x;
    double position_y;
    double velocity_x;
    double velocity_y;
    float mass;
    int id;
};

consteval {
    make_struture_of_array<Point, ARRAY_SIZE>();
}

using Points = struct_of_arrays<Point, ARRAY_SIZE>;

int main() {
    Points points;
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        points.x[i] = static_cast<float>(i);
        points.y[i] = static_cast<float>(i * 2);
        points.z[i] = static_cast<float>(i * 3);
    }

    std::cout << "Accessing SoA data:" << std::endl;
    std::cout << "  All X values: ";
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        std::cout << points.x[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "  All Y values: ";
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        std::cout << points.y[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "  All Z values: ";
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        std::cout << points.z[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

