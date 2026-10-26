#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#endif
#include "robot.h"
#include "referee.h"
using namespace std;

// 作用：把 Windows 控制台的输入/输出代码页切成 UTF-8
// 参数：无
// 返回：无
void setup_console_utf8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// 作用：登记一台机器人，失败时只打印原因，不中断程序
// 参数：robot —— 新机器人，所有权会交给裁判系统
// 返回：无
void register_one(unique_ptr<Robot> robot)
{
    try
    {
        referee.registerRobot(move(robot));
    }
    catch (const exception& error)  // [题目给定] 捕获标准异常
    {
        cout << "登记失败：" << error.what() << endl;
    }
}

int main()
{
    setup_console_utf8();

    cout << "=== 登记机器人 ===" << endl;

    // [题目给定] 用 make_unique 创建机器人，把所有权交给裁判系统
    register_one(make_unique<Infantry>(RobotInfo{1, "步兵", Team::Red}));
    register_one(make_unique<Hero>(RobotInfo{2, "英雄", Team::Red}));
    register_one(make_unique<Sentry>(RobotInfo{3, "哨兵", Team::Blue}));
    register_one(make_unique<AerialRobot>(RobotInfo{4, "无人机", Team::Blue}));

    referee.printStatus();

    register_one(make_unique<Infantry>(RobotInfo{1, "重复编号", Team::Blue}));

    cout << endl << "=== 处理比赛事件 ===" << endl;

    // [题目给定] 演示事件
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
        {EventType::Supply, 1, 0}
    };

    for (std::size_t i = 0; i < events.size(); i++)
    {
        const MatchEvent& event = events[i];
        int number = (int)i + 1;

        cout << "事件 " << number << "：机器人 " << event.robotId;
        if (event.type == EventType::Damage)
            cout << " 受到 " << event.value << " 点伤害";
        else if (event.type == EventType::Supply)
            cout << " 接收补给";
        else
            cout << " 未知事件";

        try
        {
            int actual = referee.processEvent(event);

            if (event.type == EventType::Damage)
                cout << " -> 成功，实际扣除 " << actual << " 点";
            else
                cout << " -> 成功，实际恢复 " << actual << " 点";

            Robot* robot = referee.findRobot(event.robotId);
            cout << "，生命值：" << robot->hp() << "/" << robot->maxHp();

            if (!robot->isAlive())
                cout << "，已退场";
            cout << endl;
        }
        catch (const exception& error)   // [题目给定] 失败不退出，继续下一条
        {
            cout << " -> 失败：" << error.what() << endl;
        }
    }

    cout << endl << "=== 最终状态 ===" << endl;
    referee.printStatus();
    cout << "成功处理事件数：" << referee.successfulEvents() << endl;

    return 0;
}
