#include "referee.hpp"

#include <stdexcept>
#include <utility>

namespace rm
{
  //RefereeSystem::RefereeSystem(){}

  void RefereeSystem::registerRobot(std::unique_ptr<Robot> robot)
  {
    if(!robot)
    {
      throw std::invalid_argument("机器人指针为空");
    }

    if(robot->info().name.empty())
    {
      throw std::invalid_argument("机器人指针不能为空");
    }

    if(robot->info().id <= 0)
    {
      throw std::invalid_argument("机器人id必须大于0");
    }

    if(findRobot(robot->info().id) != nullptr)
    {
      throw std::invalid_argument("机器人编号重复");
    }

    robots_.push_back(std::move(robot));
  }

  Robot* RefereeSystem::findRobot(int id)
  {
    for(auto& robot : robots_)
    {
      if(robot->info().id == id)
      {
        return robot.get();
      }
    }
    return nullptr;
  }

  int RefereeSystem::processEvent(const MatchEvent& event)
  {
    Robot* robot = findRobot(event.robotId);
    if(!robot)
    {
      throw std::invalid_argument("机器人不存在");
    }

    int actual = 0;
    switch (event.type)
    {
    case EventType::Damage:
      actual = robot->takeDamage(event.value);
      break;
    case EventType::Supply:
      actual = robot->supply();
      break;
    default:
      throw std::invalid_argument("未知事件类型");
    }

    successCount_++;
    return actual;
  }

  std::size_t RefereeSystem::successfulEvents() const noexcept
  {
    return successCount_;
  }

  void RefereeSystem::printStatus() const
  {
    for(const auto&robot : robots_)
    {
      printRobot(*robot);
    }
  }
}
