#include "ctx_definition.hpp"
#include "aggregate_formatter.hpp"

#include <iostream>

struct Point {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct Quaternion {
    double w = 1.0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct Pose {
    Point position{};
    Quaternion orientation{};
};

struct RobotState {
    int id = 0;
    Pose pose{};
    double velocity = 0.0;
    bool active = false;
};

int main() {
    Point point{1.5, 2.5, 3.5};
    std::cout << "Printing Point struct:" << std::endl;
    std::cout << "  " << format_aggregate(point) << std::endl;
    std::cout << std::endl;

    Quaternion quat{0.707, 0.0, 0.707, 0.0};
    std::cout << "Printing Quaternion struct:" << std::endl;
    std::cout << "  " << format_aggregate(quat) << std::endl;
    std::cout << std::endl;

    static constexpr auto pose_formatter = build_aggregate_formatter<Pose>();
    Pose pose{{1.0, 2.0, 3.0}, {1.0, 0.0, 0.0, 0.0}};
    std::cout << "Printing Pose struct:" << std::endl;
    std::cout << "  " << pose_formatter(pose) << std::endl;
    std::cout << std::endl;

    static constexpr auto robot_formatter = build_aggregate_formatter<RobotState>();
    RobotState robot{42, {{5.0, 6.0, 7.0}, {0.5, 0.5, 0.5, 0.5}}, 1.5, true};
    std::cout << "Printing RobotState struct:" << std::endl;
    std::cout << "  " << robot_formatter(robot) << std::endl;

    return 0;
}

