// ============================================================
//任务实现
// ============================================================

#include "robot_referee_system.h"

namespace robot_system {

// ============================================================
// Robot 基类方法实现
// ============================================================

// 构造函数：初始化机器人信息和最大生命值，初始生命值=最大生命值（满血开局）
Robot::Robot(RobotInfo info, int maxHp)
    : info_(info), maxHp_(maxHp), currentHp_(maxHp) {}

// 检查机器人是否存活，若已退场则抛出逻辑错误异常
// 派生类的补给等操作前必须调用此函数检查
void Robot::requireAlive() const {
    if (!isAlive()) {
        throw std::logic_error("机器人已退场");
    }
}

// 恢复生命值（派生类复用的辅助函数）
// 恢复量不超过最大生命值与当前生命值的差值（即距离满血的空缺）
// 返回实际恢复的量
int Robot::recover(int amount) {
    // 最多恢复到满血，不能超过maxHp_
    int actual = std::min(amount, maxHp_ - currentHp_);
    currentHp_ += actual;
    return actual;
}

// 受到伤害处理
// 返回实际扣除的生命值（可能被当前生命值限制，不会扣到负数）
// 若机器人已退场或伤害值不合法则抛出异常
int Robot::takeDamage(int damage) {
    // 1. 检查机器人是否存活（退场机器人不能再受到伤害）
    requireAlive();
    // 2. 检查伤害值合法性（必须大于0）
    if (damage <= 0) {
        throw std::invalid_argument("伤害值必须大于 0");
    }
    // 3. 计算实际扣除量
    //    生命值最低为0，所以最多只能扣除当前生命值那么多
    //    注意：这里用 currentHp_（当前生命值），不是 maxHp_ - currentHp_（那是可恢复量）
    int actual = std::min(damage, currentHp_);
    currentHp_ -= actual;
    // 注意：即使生命值降为0（退场），伤害也已成功施加，返回实际扣除量
    return actual;
}

// ============================================================
// 步兵类实现
// ============================================================

// 构造函数：调用基类构造函数，设置最大生命值为100
Infantry::Infantry(RobotInfo info) : Robot(info, 100) {}

// 实现补给接口：步兵地面补给恢复20点生命值
// 若机器人已退场，requireAlive()会抛出logic_error
int Infantry::supply() {
    requireAlive();  // 检查机器人是否存活
    return recover(20);  // 恢复20点（recover内部自动限制不超过最大生命值）
}

// 返回类型名称
std::string Infantry::typeName() const {
    return "步兵";
}

// ============================================================
// 英雄类实现
// ============================================================

// 构造函数：调用基类构造函数，设置最大生命值为150
Hero::Hero(RobotInfo info) : Robot(info, 150) {}

// 实现补给接口：英雄地面补给恢复30点生命值
int Hero::supply() {
    requireAlive();
    return recover(30);
}

std::string Hero::typeName() const {
    return "英雄";
}

// ============================================================
// 哨兵类实现
// ============================================================

// 构造函数：调用基类构造函数，设置最大生命值为120
Sentry::Sentry(RobotInfo info) : Robot(info, 120) {}

// 实现补给接口：哨兵地面补给恢复10点生命值
int Sentry::supply() {
    requireAlive();
    return recover(10);
}

std::string Sentry::typeName() const {
    return "哨兵";
}

// ============================================================
// 空中机器人类实现
// ============================================================

// 构造函数：调用基类构造函数，设置最大生命值为80
AerialRobot::AerialRobot(RobotInfo info) : Robot(info, 80) {}

// 实现补给接口：空中机器人不支持地面补给，直接抛出异常
// 无论是否存活都失败（题目要求：空中机器人即使满血，地面补给也应失败）
int AerialRobot::supply() {
    throw std::logic_error("空中机器人不支持地面补给");
}

std::string AerialRobot::typeName() const {
    return "空中机器人";
}

// ============================================================
// RefereeSystem 裁判系统类方法实现
// ============================================================

// 登记机器人（通过右值引用转移所有权）
// 使用std::make_unique创建机器人，将所有权交给裁判系统
// 异常：空指针、重复编号、空名称时抛出std::invalid_argument
void RefereeSystem::registerRobot(std::unique_ptr<Robot> robot) {
    // 检查空指针
    if (!robot) {
        throw std::invalid_argument("机器人指针为空");
    }
    // 检查编号是否重复（遍历已有机器人，红蓝双方也不能重复）
    for (const auto& r : robots_) {
        if (r->info().id == robot->info().id) {
            throw std::invalid_argument("机器人编号重复");
        }
    }
    // 检查名称非空
    if (robot->info().name.empty()) {
        throw std::invalid_argument("机器人名称不能为空");
    }
    // 转移所有权并存储到容器中
    robots_.push_back(std::move(robot));
}

// 根据编号查找机器人
// 返回指向机器人的指针（仅用于访问，调用者不拥有所有权，不能释放）
// 找不到时返回nullptr
Robot* RefereeSystem::findRobot(int id) {
    for (auto& r : robots_) {
        if (r->info().id == id) {
            return r.get();  // get()返回原始指针，不转移所有权
        }
    }
    return nullptr;
}

// 处理伤害事件（内部辅助方法）
// 查找机器人、验证参数、执行伤害，返回实际扣除量
int RefereeSystem::handleDamage(const MatchEvent& event) {
    // 1. 查找目标机器人
    Robot* robot = findRobot(event.robotId);
    if (!robot) {
        throw std::logic_error("机器人不存在");
    }
    // 2. 验证伤害值合法性（必须大于0）
    if (event.value <= 0) {
        throw std::invalid_argument("伤害值必须大于 0");
    }
    // 3. 执行伤害（takeDamage内部会检查机器人是否存活）
    return robot->takeDamage(event.value);
}

// 处理补给事件（内部辅助方法）
// 查找机器人、执行补给，返回实际恢复量
int RefereeSystem::handleSupply(const MatchEvent& event) {
    // 1. 查找目标机器人
    Robot* robot = findRobot(event.robotId);
    if (!robot) {
        throw std::logic_error("机器人不存在");
    }
    // 2. 通过基类指针调用派生类的supply()（多态）
    //    由派生类决定补给效果，不得在裁判系统里按机器人类型写分支
    return robot->supply();
}

// 处理比赛事件
// 根据事件类型分发到handleDamage或handleSupply
// 成功时递增successfulEvents_计数器
// 返回实际扣除或恢复的生命值
// 异常：未知事件类型抛出std::invalid_argument
int RefereeSystem::processEvent(const MatchEvent& event) {
    int result;
    switch (event.type) {
        case EventType::Damage:
            result = handleDamage(event);
            break;
        case EventType::Supply:
            result = handleSupply(event);
            break;
        default:
            throw std::invalid_argument("未知的事件类型");
    }
    // 只有成功执行后才递增计数器（异常情况下不递增）
    successfulEvents_++;
    return result;
}

// 按登记顺序打印所有机器人的状态信息
void RefereeSystem::printStatus() const {
    for (const auto& r : robots_) {
        std::cout << r->info().id << " "
                  << teamToString(r->info().team) << " "
                  << r->typeName() << " "
                  << r->info().name << " "
                  << r->hp() << "/" << r->maxHp() << " "
                  << r->statusString() << std::endl;
    }
}

} // namespace robot_system