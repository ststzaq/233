#include <iostream>
#include <iomanip>
#include <vector>
#include <exception>

#include "referee.hpp"
#include "robo_define.hpp"

int main(void)
{
  rm::RefereeSystem sys;

  std::cout << "=== 登记机器人 ===" << std::endl;
  auto tryRegister = [&sys](std::unique_ptr<rm::Robot> robot)
  {
    int id = robot ? robot->info().id : -1;
    try
    {
      sys.registerRobot(std::move(robot));
      rm::Robot* r = sys.findRobot(id);

      if(r)
      {
        rm::printRobot(*r);
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << "登记失败" << e.what() << '\n';
    }
  };

  tryRegister(std::make_unique<rm::Infantry>(
        rm::RobotInfo{1, "王八", rm::Team::Red}));
  tryRegister(std::make_unique<rm::Hero>(
        rm::RobotInfo{2, "英雄", rm::Team::Red}));
  tryRegister(std::make_unique<rm::Sentry>(
        rm::RobotInfo{3, "过洞", rm::Team::Blue}));
  tryRegister(std::make_unique<rm::AerialRobot>(
        rm::RobotInfo{4, "飞机", rm::Team::Blue}));

  tryRegister(std::make_unique<rm::Infantry>(
        rm::RobotInfo{1, "重复编号", rm::Team::Blue}));
  
  std::cout << "=== 处理比赛事件 ===" << std::endl;

  for(std::size_t i = 0; i < events.size(); i++)
  {
    const rm::MatchEvent& event = events[i];
    std::cout << "事件" << (i + 1) << ":";

    try
    {
      int actual = sys.processEvent(event);

      rm::Robot* robot = sys.findRobot(event.robotId);

      if(event.type == rm::EventType::Damage)
      {
        std::cout << "机器人 " << event.robotId
                          << " 受到 " << event.value
                          << " 点伤害 -> 成功，实际扣除 " << actual
                          << " 点，生命值："
                          << robot->hp() << "/" << robot->maxHp();
      }
      else
      {
        std::cout << "机器人 " << event.robotId
                          << " 接收补给 -> 成功，实际恢复 " << actual
                          << " 点，生命值："
                          << robot->hp() << "/" << robot->maxHp();
      }

      if(!robot->isAlive())
      {
        std::cout << "已退场";
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << "失败：" << e.what() << '\n';
    }
  }

  std::cout << "\n=== 最终状态 ===\n";
  sys.printStatus();
  std::cout << "成功处理事件数：" << sys.successfulEvents() << '\n';
  return 0;
}
