#include "robo_define.hpp"

#include <stdexcept>
#include <utility>

namespace rm{
//---------------------步兵--------------------------
Infantry::Infantry(RobotInfo info) : Robot(std::move(info), 100){}

int Infantry::supply()
{
  return recover(20);
}

std::string Infantry::typeName() const
{
  return "步兵";
}

// ---------- 英雄 ----------
Hero::Hero(RobotInfo info) : Robot(std::move(info), 150) {}

int Hero::supply() {
    return recover(30);
}

std::string Hero::typeName() const {
    return "英雄";
}

// ---------- 哨兵 ----------
Sentry::Sentry(RobotInfo info) : Robot(std::move(info), 120) {}

int Sentry::supply() {
    return recover(10);
}

std::string Sentry::typeName() const {
    return "哨兵";
}

// ---------- 飞机 ----------
AerialRobot::AerialRobot(RobotInfo info) : Robot(std::move(info), 80) {}

int AerialRobot::supply() {
    // 先查存活：退场的空中机器人报"机器人已退场"
    requireAlive();
    // 存活的空中机器人：地面补给一律失败
    throw std::logic_error("空中机器人不支持地面补给");
}

std::string AerialRobot::typeName() const {
    return "空中机器人";
}


}