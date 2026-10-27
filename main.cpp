#include "percept.h"

#include <cstdlib> // std::system
#include <iostream>
#include <string>
#include <vector>

using namespace rm::judge;

namespace
{

    // "机器人 1 受到 35 点伤害" / "机器人 1 接收补给"
    std::string eventText(const MatchEvent &event)
    {
        const std::string who = "机器人 " + std::to_string(event.robotId);
        if (event.type == EventType::Damage)
            return who + " 受到 " + std::to_string(event.value) + " 点伤害";
        return who + " 接收补给";
    }

    void registerDemoRobots(RefereeSystem &referee)
    {
        struct Registration
        {
            RobotInfo info;
            RobotKind kind;
        };
        const std::vector<Registration> registrations = {
            {{1, "王八", Team::Red}, RobotKind::Infantry},
            {{2, "英雄", Team::Red}, RobotKind::Hero},
            {{3, "过洞", Team::Blue}, RobotKind::Sentry},
            {{4, "无人机", Team::Blue}, RobotKind::Aerial},
            {{1, "重复编号", Team::Blue}, RobotKind::Infantry}, // 应失败
        };

        std::cout << "=== 登记机器人 ===\n";
        for (const Registration &reg : registrations)
        {
            try
            {
                referee.registerRobot(makeRobot(reg.kind, reg.info));
                referee.printRobot(reg.info.id);
            }
            catch (const std::exception &e)
            {
                std::cout << "登记失败：" << e.what() << '\n';
            }
        }
    }

    void runEvents(RefereeSystem &referee, const std::vector<MatchEvent> &events)
    {
        std::cout << "\n=== 处理比赛事件 ===\n";
        for (std::size_t i = 0; i < events.size(); ++i)
        {
            const MatchEvent &event = events[i];
            std::cout << "事件 " << i + 1 << "：" << eventText(event) << " -> ";
            try
            {
                const int amount = referee.processEvent(event);
                const Robot *robot = referee.findRobot(event.robotId); // 成功即存在
                std::cout << "成功，实际"
                          << (event.type == EventType::Damage ? "扣除 " : "恢复 ")
                          << amount << " 点，生命值：" << robot->hp() << '/'
                          << robot->maxHp() << (robot->isAlive() ? "" : "，已退场") << '\n';
            }
            catch (const std::exception &e)
            {
                std::cout << "失败：" << e.what() << '\n';
            }
        }
    }

} // namespace

int main()
{
#ifdef _WIN32
    std::system("chcp 65001 > nul"); // 控制台切到 UTF-8
#endif

    const std::vector<MatchEvent> events = {
        {EventType::Damage, 1, 35},
        {EventType::Damage, 2, 50},
        {EventType::Supply, 1, 0},
        {EventType::Supply, 2, 0},
        {EventType::Supply, 4, 0},
        {EventType::Damage, 3, 200},
        {EventType::Supply, 3, 0},
        {EventType::Damage, 99, 10},
        {EventType::Damage, 1, -5},
        {EventType::Supply, 1, 0},
        {EventType::Supply, 1, 0},
    };

    RefereeSystem referee;
    registerDemoRobots(referee);
    runEvents(referee, events);

    std::cout << "\n=== 最终状态 ===\n";
    referee.printStatus();
    std::cout << "成功处理事件数：" << referee.successfulEvents() << '\n';

    return 0;
}