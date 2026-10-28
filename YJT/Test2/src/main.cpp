#include "Refree_System.h"
#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    RefereeSystem refereeSystem;

    int robotCount;
    if (!(std::cin >> robotCount))
        return 0;

    std::cout << "=== 登记机器人 ===" << std::endl;
    for (int i = 0; i < robotCount; ++i)
    {
        int id;
        std::string robotName;
        std::string teamText;
        std::string typeText;
        if (!(std::cin >> id >> robotName >> teamText >> typeText))
            break;

        try
        {
            Team team;
            if (teamText == "红方")
                team = Team::Red;
            else if (teamText == "蓝方")
                team = Team::Blue;
            else
                throw std::invalid_argument("阵营不存在");

            RobotInfo info{id, team, robotName};

            std::unique_ptr<Robot> robot;
            if (typeText == "步兵")
                robot = std::make_unique<Infantry>(info, 100);
            else if (typeText == "英雄")
                robot = std::make_unique<Hero>(info, 150);
            else if (typeText == "哨兵")
                robot = std::make_unique<Sentry>(info, 120);
            else if (typeText == "空中机器人")
                robot = std::make_unique<AerialRobot>(info, 80);
            else
                throw std::invalid_argument("机器人类型不存在");

            refereeSystem.registerRobot(std::move(robot));

            Robot* registered = refereeSystem.findRobot(id);
            std::cout << registered->info().robotId << ' '
                      << teamName(registered->info().team) << ' '
                      << registered->typeName() << ' '
                      << registered->info().robot_name << ' '
                      << registered->hp() << '/' << registered->maxHp()
                      << " 正常" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "登记失败：" << e.what() << std::endl;
        }
    }

    int eventCount;
    if (!(std::cin >> eventCount))
        return 0;

    std::cout << "=== 处理比赛事件 ===" << std::endl;
    for (int j = 0; j < eventCount; ++j)
    {
        std::string typeText;
        int robotId;
        int value;
        if (!(std::cin >> typeText >> robotId >> value))
            break;

        std::string eventDesc;
        try
        {
            EventType type;
            if (typeText == "Damage")
            {
                type = EventType::Damage;
                eventDesc = "受到 " + std::to_string(value) + " 点伤害";
            }
            else if (typeText == "Supply")
            {
                type = EventType::Supply;
                eventDesc = "接收补给";
            }
            else
            {
                throw std::invalid_argument("未知事件类型");
            }

            int actual = refereeSystem.processEvent(MatchEvent{type, robotId, value});
            Robot* target = refereeSystem.findRobot(robotId);

            if (type == EventType::Damage)
            {
                std::cout << "事件 " << j + 1 << "：机器人 " << robotId
                          << ' ' << eventDesc
                          << " -> 成功，实际扣除 " << actual
                          << " 点，生命值：" << target->hp() << '/' << target->maxHp();
                if (!target->isAlive())
                    std::cout << "，已退场";
                std::cout << std::endl;
            }
            else
            {
                std::cout << "事件 " << j + 1 << "：机器人 " << robotId
                          << ' ' << eventDesc
                          << " -> 成功，实际恢复 " << actual
                          << " 点，生命值：" << target->hp() << '/' << target->maxHp()
                          << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "事件 " << j + 1 << "：机器人 " << robotId;
            if (!eventDesc.empty())
                std::cout << ' ' << eventDesc;
            std::cout << " -> 失败：" << e.what() << std::endl;
        }
    }

    refereeSystem.printStatus();
    return 0;
}
