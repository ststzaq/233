#include "robot.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace rm
{
  Robot::Robot(RobotInfo info, int maxHp): info_(std::move(info)), hp_(maxHp), maxHp_(maxHp)
  {
    if(maxHp_ <= 0)
    {
      throw std::invalid_argument("生命值必须大于0");
    }
  }

  const RobotInfo& Robot::info() const noexcept {return info_;}
  int Robot::hp() const noexcept {return hp_;}
  int Robot::maxHp() const noexcept {return maxHp_;}
  bool Robot::isAlive() const noexcept {return hp_ > 0;}

  void Robot::requireAlive() const
  {
    if(!isAlive())
    {
      throw std::logic_error("机器人已退场");
    }
  }

  int Robot::takeDamage(int damage)
  {
    requireAlive();
    if(damage <= 0)
    {
      throw std::invalid_argument("伤害必须大于0");
    }
    int actual = std::min(hp_, damage);
    hp_ -= actual;
    return actual;
  }

  int Robot::recover(int amount)
  {
    requireAlive();
    int actual = std::min(maxHp_ - hp_, amount);
    hp_ += actual;
    return actual;
  }

  void printRobot(const Robot& robot)
  {
    std::cout << robot.info().id            << ' '
              << teamToString(robot.info().team) << ' '
              << robot.typeName()           << ' '
              << robot.info().name          << ' '
              << robot.hp() << '/' << robot.maxHp() << ' '
              << (robot.isAlive() ? "正常" : "已退场")
              << '\n';
  }
}