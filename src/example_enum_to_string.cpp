#include "enum_to_string.hpp"

#include <iostream>

enum class Color {
    Red,
    Green,
    Blue,
    Yellow
};

enum Planners {
    RRT,
    ASTAR,
    HYBRID_ASTAR,
    PRM
};

enum class RobotState {
    Idle,
    Moving,
    Grasping,
    Error,
    EmergencyStop
};

int main() {
    std::cout << "Color::Red -> " << enum_to_string(Color::Red) << std::endl;
    std::cout << "Color::Blue -> " << enum_to_string(Color::Blue) << std::endl;
    std::cout << std::endl;

    std::cout << "Planners::RRT -> " << enum_to_string(Planners::RRT) << std::endl;
    std::cout << "Planners::HYBRID_ASTAR -> " << enum_to_string(Planners::HYBRID_ASTAR) << std::endl;
    std::cout << std::endl;

    std::cout << "RobotState::Idle -> " << enum_to_string(RobotState::Idle) << std::endl;
    std::cout << "RobotState::Moving -> " << enum_to_string(RobotState::Moving) << std::endl;
    std::cout << "RobotState::EmergencyStop -> " << enum_to_string(RobotState::EmergencyStop) << std::endl;

    return 0;
}

