#include "ctx_definition.hpp"
#include "serialize_aggregate.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>

namespace {

enum class Mode : std::uint8_t {
    Idle = 0,
    Active = 1,
    Error = 2,
};

struct Header {
    std::uint32_t seq = 0;
    std::uint64_t stamp_ns = 0;
};

bool equals(const Header& a, const Header& b) {
    return a.seq == b.seq && a.stamp_ns == b.stamp_ns;
}

struct Vec3f {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

bool equals(const Vec3f& a, const Vec3f& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

struct Quaternionf {
    float w = 1.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

bool equals(const Quaternionf& a, const Quaternionf& b) {
    return a.w == b.w && a.x == b.x && a.y == b.y && a.z == b.z;
}

struct ImuMsg {
    Header header{};
    Mode mode = Mode::Idle;

    Quaternionf orientation{};
    float orientation_cov[9]{};

    Vec3f angular_velocity{};
    float angular_velocity_cov[9]{};

    Vec3f linear_acceleration{};
    float linear_acceleration_cov[9]{};
};

bool equals(const ImuMsg& a, const ImuMsg& b) {
    if (!equals(a.header, b.header)) return false;
    if (a.mode != b.mode) return false;
    if (!equals(a.orientation, b.orientation)) return false;
    if (!equals(a.angular_velocity, b.angular_velocity)) return false;
    if (!equals(a.linear_acceleration, b.linear_acceleration)) return false;
    for (std::size_t i = 0; i < 9; ++i) {
        if (a.orientation_cov[i] != b.orientation_cov[i]) return false;
        if (a.angular_velocity_cov[i] != b.angular_velocity_cov[i]) return false;
        if (a.linear_acceleration_cov[i] != b.linear_acceleration_cov[i]) return false;
    }
    return true;
}

struct WheelOdometry {
    Header header{};

    std::array<float, 2> wheel_position_rad{};
    std::array<float, 2> wheel_velocity_rad_s{};

    float dt_s = 0.0f;
    std::uint32_t integration_steps = 0;
};

bool equals(const WheelOdometry& a, const WheelOdometry& b) {
    return equals(a.header, b.header) &&
           a.wheel_position_rad == b.wheel_position_rad &&
           a.wheel_velocity_rad_s == b.wheel_velocity_rad_s &&
           a.dt_s == b.dt_s &&
           a.integration_steps == b.integration_steps;
}

}

int main() {
    {
        ImuMsg src{};
        src.header.seq = 42;
        src.header.stamp_ns = 1'735'000'123'456'789ull;
        src.mode = Mode::Active;

        src.orientation = {0.7071f, 0.0f, 0.7071f, 0.0f};
        src.angular_velocity = {0.01f, -0.02f, 0.005f};
        src.linear_acceleration = {0.0f, 0.0f, 9.81f};

        for (std::size_t i = 0; i < 9; ++i) {
            src.orientation_cov[i] = (i % 4 == 0) ? 1e-3f : 0.0f;
            src.angular_velocity_cov[i] = (i % 4 == 0) ? 2e-3f : 0.0f;
            src.linear_acceleration_cov[i] = (i % 4 == 0) ? 5e-3f : 0.0f;
        }

        constexpr std::size_t N = serialized_size<ImuMsg>();
        std::array<std::byte, N> buf{};

        const auto written = serialize_into(src, buf.data());
        std::cout << "  serialized_size: " << N << " bytes, written: " << written << " bytes" << std::endl;

        ImuMsg dst{};
        const auto read = deserialize_from(dst, buf.data());
        std::cout << "  read: " << read << " bytes" << std::endl;
        std::cout << "  roundtrip equal: " << std::boolalpha << equals(src, dst) << std::endl;
        std::cout << std::endl;
    }

    {
        WheelOdometry src{};
        src.header.seq = 314;
        src.header.stamp_ns = 1'735'000'999'000'000ull;

        src.wheel_position_rad = {12.5f, 12.45f};
        src.wheel_velocity_rad_s = {3.2f, 3.15f};
        src.dt_s = 0.01f;
        src.integration_steps = 100;

        constexpr std::size_t N = serialized_size<WheelOdometry>();
        std::array<std::byte, N> buf{};

        const auto written = serialize_into(src, buf.data());
        std::cout << "  serialized_size: " << N << " bytes, written: " << written << " bytes" << std::endl;

        WheelOdometry dst{};
        const auto read = deserialize_from(dst, buf.data());
        std::cout << "  read: " << read << " bytes" << std::endl;
        std::cout << "  roundtrip equal: " << std::boolalpha << equals(src, dst) << std::endl;
    }

    return 0;
}


