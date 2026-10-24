#include "referee.h"

void RefereeSystem::registerRobot(std::unique_ptr<Robot> robot) {
    if (!robot)
        throw std::invalid_argument("登记失败：机器人指针为空");
    if (findRobot(robot->info().id) != nullptr)
        throw std::invalid_argument("登记失败：机器人编号重复");
    robots_.push_back(std::move(robot));
}
Robot* RefereeSystem::findRobot(int id) {
    for (auto& r : robots_)
        if (r->info().id == id) return r.get();
    return nullptr;
}
int RefereeSystem::processEvent(const MatchEvent& event) {
    Robot* robot = findRobot(event.robotId);
    if (!robot)
        throw std::invalid_argument("失败：机器人不存在");
    robot->requireAlive();
    int effect = 0;
    if (event.type == EventType::Damage) {
        if (event.value <= 0) {
            throw std::invalid_argument("失败：伤害值必须大于 0");
        }
        effect = robot->takeDamage(event.value);
    }
    else if (event.type == EventType::Supply) {
        effect = robot->supply();
    }
    else {
        throw std::invalid_argument("失败：未知事件类型");
    }
    ++successfulEvents_;
    return effect;
}
std::size_t RefereeSystem::successfulEvents() const {
    return successfulEvents_;
}
void RefereeSystem::printStatus() const {
    std::cout << "=== 最终状态 ===\n";
    for (const auto& r : robots_) {
        std::cout << r->info().id << " "
                  << (r->info().team == Team::Red ? "红方" : "蓝方") << " "
                  << r->typeName() << " "
                  << r->info().name << " "
                  << r->hp() << "/" << r->maxHp() << " "
                  << (r->isAlive() ? "正常" : "已退场")
                  << "\n";
    }
    std::cout << "成功处理事件数：" << successfulEvents() << "\n";
}

int Robot::takeDamage(int damage) {
    if (damage <= 0) {
        throw std::invalid_argument("失败：伤害值必须大于 0");
    }
    requireAlive();
    int before = hp_;
    hp_ = std::max(0, hp_ - damage);
    return before - hp_;
}


