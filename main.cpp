#include "referee.h"

int main() {
    RefereeSystem ref;
    std::cout << "=== 登记机器人 ===\n";
    ref.registerRobot(std::make_unique<Infantry>(RobotInfo{"王八", Team::Red, 1}));
    ref.registerRobot(std::make_unique<Hero>(RobotInfo{"英雄", Team::Red, 2}));
    ref.registerRobot(std::make_unique<Sentry>(RobotInfo{"过洞", Team::Blue, 3}));
    ref.registerRobot(std::make_unique<AerialRobot>(RobotInfo{"无人机", Team::Blue, 4}));

    for (const auto& r : ref.robots()) {
        std::cout << r->info().id << " "
                  << (r->info().team == Team::Red ? "红方" : "蓝方") << " "
                  << r->typeName() << " "
                  << r->info().name << " "
                  << r->hp() << "/" << r->maxHp() << " "
                  << (r->isAlive() ? "正常" : "已退场")
                  << "\n";
    }

    try {
        ref.registerRobot(std::make_unique<Infantry>(RobotInfo{"重复编号", Team::Blue, 1}));
    }
    catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }

    std::cout << "\n=== 处理比赛事件 ===\n";
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
    for (std::size_t i = 0; i < events.size(); ++i) {
        const auto& e = events[i];
        std::cout << "事件 " << (i + 1) << ":";

        Robot* robot = ref.findRobot(e.robotId);
        std::string action = (e.type == EventType::Damage)
            ? ("受到 " + std::to_string(e.value) + " 点伤害")
            : "接收补给";

        if (!robot) {
            std::cout << "机器人 " << e.robotId << " " << action << " -> ";
            try {
                ref.processEvent(e);
            }
            catch (const std::exception& ex) {
                std::cout << ex.what() << "\n";
            }
            continue;
        }

        std::cout << "机器人 " << e.robotId << " " << action << " -> ";
        try {
            int effect = ref.processEvent(e);
            std::string verb = (e.type == EventType::Damage) ? "实际扣除 " : "实际恢复 ";
            std::cout << "成功，" << verb << effect << " 点，生命值："
                      << robot->hp() << "/" << robot->maxHp();
            if (!robot->isAlive())
                std::cout << "，已退场";
            std::cout << "\n";
        }
        catch (const std::exception& ex) {
            std::cout << ex.what() << "\n";
        }
    }
    std::cout << "\n";
    ref.printStatus();

    return 0;
}