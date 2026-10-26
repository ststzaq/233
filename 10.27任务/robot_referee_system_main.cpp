// ============================================================
// 主程序
// ============================================================

#include "robot_referee_system.h"

using namespace robot_system;

int main() {
    RefereeSystem system;  // 创建裁判系统实例

    // === 第一阶段：登记机器人 ===
    std::cout << "=== 登记机器人 ===" << std::endl;

    // 登记机器人1：步兵"王八"，红方，最大生命值100
    try {
        system.registerRobot(std::make_unique<Infantry>(RobotInfo{1, "王八", Team::Red}));
    } catch (const std::exception& e) {
        std::cout << "登记失败：" << e.what() << std::endl;
    }

    // 登记机器人2：英雄"英雄"，红方，最大生命值150
    try {
        system.registerRobot(std::make_unique<Hero>(RobotInfo{2, "英雄", Team::Red}));
    } catch (const std::exception& e) {
        std::cout << "登记失败：" << e.what() << std::endl;
    }

    // 登记机器人3：哨兵"过洞"，蓝方，最大生命值120
    try {
        system.registerRobot(std::make_unique<Sentry>(RobotInfo{3, "过洞", Team::Blue}));
    } catch (const std::exception& e) {
        std::cout << "登记失败：" << e.what() << std::endl;
    }

    // 登记机器人4：空中机器人"无人机"，蓝方，最大生命值80
    try {
        system.registerRobot(std::make_unique<AerialRobot>(RobotInfo{4, "无人机", Team::Blue}));
    } catch (const std::exception& e) {
        std::cout << "登记失败：" << e.what() << std::endl;
    }

    // 打印已登记机器人的初始状态（全部满血）
    system.printStatus();

    // 尝试登记编号重复的机器人（编号1已存在），应失败且原机器人保留
    try {
        system.registerRobot(std::make_unique<Infantry>(RobotInfo{1, "重复编号", Team::Blue}));
    } catch (const std::exception& e) {
        std::cout << "登记失败：" << e.what() << std::endl;
    }

    // === 第二阶段：处理比赛事件 ===
    std::cout << std::endl << "=== 处理比赛事件 ===" << std::endl;

    // 定义比赛事件序列（共11个事件）
    const std::vector<MatchEvent> events = {
        {EventType::Damage, 1, 35},    // 事件1：机器人1受到35点伤害
        {EventType::Damage, 2, 50},    // 事件2：机器人2受到50点伤害
        {EventType::Supply, 1, 0},     // 事件3：机器人1接收补给
        {EventType::Supply, 2, 0},     // 事件4：机器人2接收补给
        {EventType::Supply, 4, 0},     // 事件5：机器人4接收补给（空中机器人应失败）
        {EventType::Damage, 3, 200},   // 事件6：机器人3受到200点伤害
        {EventType::Supply, 3, 0},     // 事件7：机器人3接收补给（已退场应失败）
        {EventType::Damage, 99, 10},   // 事件8：机器人99受到10点伤害（不存在应失败）
        {EventType::Damage, 1, -5},    // 事件9：机器人1受到-5点伤害（值非法应失败）
        {EventType::Supply, 1, 0},     // 事件10：机器人1接收补给
        {EventType::Supply, 1, 0}      // 事件11：机器人1接收补给（满血恢复0点）
    };

    // 遍历处理每个事件
    for (std::size_t i = 0; i < events.size(); i++) {
        try {
            // 处理事件（内部通过多态调用派生类的supply/takeDamage）
            int result = system.processEvent(events[i]);
            const MatchEvent& event = events[i];

            if (event.type == EventType::Damage) {
                // 伤害事件成功：输出伤害结果
                Robot* robot = system.findRobot(event.robotId);
                std::cout << "事件 " << (i + 1) << "：机器人 " << event.robotId
                          << " 受到 " << event.value << " 点伤害 -> 成功，实际扣除 " << result
                          << " 点，生命值：" << robot->hp() << "/" << robot->maxHp();
                // 如果机器人因这次伤害退场，追加退场提示
                if (!robot->isAlive()) {
                    std::cout << "，已退场";
                }
                std::cout << std::endl;
            } else {
                // 补给事件成功：输出补给结果
                Robot* robot = system.findRobot(event.robotId);
                std::cout << "事件 " << (i + 1) << "：机器人 " << event.robotId
                          << " 接收补给 -> 成功，实际恢复 " << result << " 点，生命值："
                          << robot->hp() << "/" << robot->maxHp() << std::endl;
            }
        } catch (const std::exception& e) {
            // 捕获所有标准异常，输出失败原因，继续处理下一条事件
            // 失败事件不增加成功事件计数
            const MatchEvent& event = events[i];

            if (event.type == EventType::Damage) {
                std::cout << "事件 " << (i + 1) << "：机器人 " << event.robotId
                          << " 受到 " << event.value << " 点伤害 -> 失败：" << e.what() << std::endl;
            } else {
                std::cout << "事件 " << (i + 1) << "：机器人 " << event.robotId
                          << " 接收补给 -> 失败：" << e.what() << std::endl;
            }
        }
    }

    // === 第三阶段：输出最终状态 ===
    std::cout << std::endl << "=== 最终状态 ===" << std::endl;
    system.printStatus();
    std::cout << "成功处理事件数：" << system.successfulEvents() << std::endl;

    return 0;
}