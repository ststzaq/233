#pragma once

#include "robot.hpp"

namespace rm {

class Infantry : public Robot {
public:
    explicit Infantry(RobotInfo info);
    ~Infantry() override = default;

    int supply() override;                  // recover(20)
    std::string typeName() const override;  // "步兵"
};

class Hero : public Robot {
public:
    explicit Hero(RobotInfo info);
    ~Hero() override = default;

    int supply() override;                  // recover(30)
    std::string typeName() const override;  // "英雄"
};

class Sentry : public Robot {
public:
    explicit Sentry(RobotInfo info);
    ~Sentry() override = default;

    int supply() override;                  // recover(10)
    std::string typeName() const override;  // "哨兵"
};

class AerialRobot : public Robot {
public:
    explicit AerialRobot(RobotInfo info);
    ~AerialRobot() override = default;

    int supply() override;                  // 抛 logic_error
    std::string typeName() const override;  // "空中机器人"
};

} // namespace rm