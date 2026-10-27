#include <iostream>
#include <vector>
#include "RobotSystem.h"

using namespace robotsystem;

int main() 
{
    RefereeSystem referee;


    std::cout << "=== 登记机器人 ===" << std::endl;
    

    referee.registerRobot(std::make_unique<Infantry>(RobotInfo{1, "王八", Team::Red}));
    referee.registerRobot(std::make_unique<Hero>(RobotInfo{2, "英雄", Team::Red}));
    referee.registerRobot(std::make_unique<Sentry>(RobotInfo{3, "过洞", Team::Blue}));
    referee.registerRobot(std::make_unique<AerialRobot>(RobotInfo{4, "无人机", Team::Blue}));
    

    referee.printStatus();


    try 
    {
        referee.registerRobot(std::make_unique<Infantry>(RobotInfo{1, "重复编号", Team::Blue}));
    } 
    catch (const std::exception& e) 
    {
        std::cout << "登记失败：" << e.what() << std::endl;
    }


    std::cout << "\n=== 处理比赛事件 ===" << std::endl;
    
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

    for (std::size_t i = 0; i < events.size(); ++i) 
    {
        const auto& ev = events[i];
        

        std::cout << "事件 " << (i + 1) << "：机器人 " << ev.robotId;
        if (ev.type == EventType::Damage) 
        {
            std::cout << " 受到 " << ev.value << " 点伤害 -> ";
        } 
        else 
        {
            std::cout << " 接收补给 -> ";
        }


        try 
        {
            int actualValue = referee.processEvent(ev);
            

            if (ev.type == EventType::Damage) 
            {
                std::cout << "成功，实际扣除 " << actualValue << " 点，生命值：";
            } 
            else 
            {
                std::cout << "成功，实际恢复 " << actualValue << " 点，生命值：";
            }
            

            Robot* robot = referee.findRobot(ev.robotId);
            std::cout << robot->hp() << "/" << robot->maxHp();
            

            if (!robot->isAlive()) 
            {
                std::cout << "，已退场";
            }
            std::cout << std::endl;

        } 
        catch (const std::exception& e) 
        {

            std::cout << "失败：" << e.what() << std::endl;
        }
    }

    std::cout << "\n=== 最终状态 ===" << std::endl;
    referee.printStatus();
    std::cout << "成功处理事件数：" << referee.successfulEvents() << std::endl;

    return 0;
}