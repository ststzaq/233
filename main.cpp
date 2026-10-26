#include "jud.h"
#include "robot_type.h"

int main()
{
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
    judge start(events);
    std::cout << "=== 登记机器人 ===" << std::endl;
    start.registerRobot(
        std::make_unique<Infantry>(RobotInfo{1, "王八", Team::Red}, 100)
    );
    start.registerRobot(
        std::make_unique<Hero>(RobotInfo{2, "英雄", Team::Red}, 150)
    );
    start.registerRobot(
        std::make_unique<Sentry>(RobotInfo{3, "过洞", Team::Blue}, 120)
    );
    start.registerRobot(
        std::make_unique<AerialRobot>(RobotInfo{4, "无人机", Team::Blue}, 80)
    );
    start.registerRobot(
        std::make_unique<Infantry>(RobotInfo{1, "王八", Team::Red}, 100)
    );
    start.event_go();
    start.event_out();
}