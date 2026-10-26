#include"Robot_Referee.h"

int main() 
{
    Referee referee;
    std::cout << "=== 登记机器人 ===" << std::endl;
    auto registerAndPrint = [&referee](std::unique_ptr<Robot> robot) 
    {
        int id = robot->info().id;
        try 
        {
            referee.registerRobot(std::move(robot));
            printRobotLine(*referee.findRobot(id));
        } catch (const std::exception& e) 
        {
            std::cout << "登记失败：" << e.what() << std::endl;
        }
    };
    registerAndPrint(std::make_unique<Infantry>(RobotInfo{1, "王八", Team::Red}));
    registerAndPrint(std::make_unique<Hero>(RobotInfo{2, "英雄", Team::Red}));
    registerAndPrint(std::make_unique<Sentry>(RobotInfo{3, "过洞", Team::Blue}));
    registerAndPrint(std::make_unique<AerialRobot>(RobotInfo{4, "无人机", Team::Blue}));
    registerAndPrint(std::make_unique<Infantry>(RobotInfo{1, "重复编号", Team::Blue}));
    std::cout << std::endl <<"=== 处理比赛事件 ===" << std::endl;
    const std::vector<MatchEvent> events = 
    {
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
    int eventNo = 0;
    for (const auto& event : events) 
    {
        eventNo++;
        std::cout << "事件 " << eventNo << "：";
        if (event.type == EventType::Damage) 
        {
            std::cout << "机器人 " << event.robotId << " 受到 " << event.value << " 点伤害 -> ";
        } else 
        {
            std::cout << "机器人 " << event.robotId << " 接收补给 -> ";
        }
        try 
        {
            Robot* robot = referee.findRobot(event.robotId);
            int actual = referee.processEvent(event);
            if (event.type == EventType::Damage) 
            {
                std::cout << "成功，实际扣除 " << actual << " 点，生命值：" << robot->hp() << "/" << robot->maxHp();
                if (!robot->isAlive()) 
                {
                    std::cout << "，已退场";
                }
            } 
            else 
            {
                std::cout << "成功，实际恢复 " << actual << " 点，生命值：" << robot->hp() << "/" << robot->maxHp();
            }
            std::cout << std::endl;
        } catch (const std::exception& e) 
        {
            std::cout << "失败：" << e.what() << std::endl;
        }
    }
    std::cout << std::endl;
    referee.printStatus();
    std::cout << "成功处理事件数：" << referee.successfulEvents() << std::endl;
    return 0;
}
