#include "referee.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace
{
    // 输出一台机器人的状态：编号 阵营 类型 名称 血量 状态
    void printRobot(const Robot &r)
    {
        std::cout << r.info().id << ' '
                  << (r.info().team == Team::Red ? "红方" : "蓝方") << ' '
                  << r.typeName() << ' '
                  << r.info().name << ' '
                  << r.hp() << '/' << r.maxHp() << ' '
                  << (r.isAlive() ? "正常" : "已退场") << '\n';
    }

    // 登记 + 打印；失败时输出原因，程序继续
    void tryRegister(RefereeSystem &sys, std::unique_ptr<Robot> robot)
    {
        const int id = robot->info().id;
        try
        {
            sys.registerRobot(std::move(robot));
            printRobot(*sys.findRobot(id));
        }
        catch (const std::exception &e)
        {
            std::cout << "登记失败：" << e.what() << '\n';
        }
    }
} // namespace

int main()
{
    RefereeSystem sys;

    // 依次登记四台机器人
    std::cout << "=== 登记机器人 ===\n";
    tryRegister(sys, std::make_unique<Infantry>(RobotInfo{1, "王八", Team::Red}));
    tryRegister(sys, std::make_unique<Hero>(RobotInfo{2, "英雄", Team::Red}));
    tryRegister(sys, std::make_unique<Sentry>(RobotInfo{3, "过洞", Team::Blue}));
    tryRegister(sys, std::make_unique<AerialRobot>(RobotInfo{4, "无人机", Team::Blue}));

    // 重复编号，应当失败，原来的 1 号保持不变
    tryRegister(sys, std::make_unique<Infantry>(RobotInfo{1, "重复编号", Team::Blue}));

    // 演示事件
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
        {EventType::Supply, 1, 0}};

    std::cout << "\n=== 处理比赛事件 ===\n";
    for (std::size_t i = 0; i < events.size(); ++i)
    {
        const MatchEvent &event = events[i];
        const bool isDamage = event.type == EventType::Damage;

        // 先输出这条事件在做什么
        std::cout << "事件 " << i + 1 << "：机器人 " << event.robotId;
        if (isDamage)
        {
            std::cout << " 受到 " << event.value << " 点伤害";
        }
        else
        {
            std::cout << " 接收补给";
        }

        try
        {
            const int delta = sys.processEvent(event);
            const Robot *robot = sys.findRobot(event.robotId);

            std::cout << " -> 成功，实际"
                      << (isDamage ? "扣除 " : "恢复 ") << delta << " 点，生命值："
                      << robot->hp() << '/' << robot->maxHp();

            // 这次伤害把机器人打到 0 血，补一句“已退场”
            if (isDamage && !robot->isAlive())
            {
                std::cout << "，已退场";
            }
            std::cout << '\n';
        }
        catch (const std::exception &e)
        {
            std::cout << " -> 失败：" << e.what() << '\n';
        }
    }

    // 最终状态
    std::cout << "\n=== 最终状态 ===\n";
    sys.printStatus();
    std::cout << "成功处理事件数：" << sys.successfulEvents() << '\n';
    return 0;
}
