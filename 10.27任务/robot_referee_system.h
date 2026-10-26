#ifndef ROBOT_REFEREE_SYSTEM_H
#define ROBOT_REFEREE_SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>

namespace robot_system {

// ============================================================
// 第一部分：枚举定义与结构体定义
// ============================================================

// 阵营枚举
enum class Team {
    Red,    // 红方
    Blue    // 蓝方
};

// 事件类型枚举
enum class EventType {
    Damage,  // 伤害事件
    Supply   // 补给事件
};

// 机器人基本信息结构体
struct RobotInfo {
    int id;              // 编号
    std::string name;    // 名称
    Team team;           // 阵营
};

// 比赛事件结构体
struct MatchEvent {
    EventType type;  // 事件类型
    int robotId;     // 目标机器人编号
    int value;       // 伤害事件的伤害值
};

// ============================================================
// 第二部分：辅助函数
// ============================================================

// 转中文
inline std::string teamToString(Team team) {
    return team == Team::Red ? "红方" : "蓝方";
}

// ============================================================
// 第三部分：抽象基类 Robot 声明
// ============================================================

// 机器人抽象基类
// 定义所有机器人类型的共同接口和行为
class Robot {
protected:
    RobotInfo info_;       // 机器人基本信息（编号、名称、阵营）
    int maxHp_;            // 最大生命值（各类型不同）
    int currentHp_;        // 当前生命值（私有，派生类通过辅助函数操作）

    // 检查机器人是否存活，若已退场则抛出逻辑错误异常
    // 派生类的补给等操作前必须调用此函数检查
    void requireAlive() const;

    // 恢复生命值（派生类复用的辅助函数）
    // 恢复量不超过最大生命值与当前生命值的差值（即距离满血的空缺）
    // 返回实际恢复的量
    int recover(int amount);

public:
    // 构造函数：初始化机器人信息和最大生命值，初始生命值=最大生命值（满血开局）
    Robot(RobotInfo info, int maxHp);

    // 虚析构函数：确保通过基类指针删除派生类对象时正确释放资源
    virtual ~Robot() = default;

    // 获取机器人基本信息（常量引用，避免拷贝）
    const RobotInfo& info() const { return info_; }

    // 获取当前生命值
    int hp() const { return currentHp_; }

    // 获取最大生命值
    int maxHp() const { return maxHp_; }

    // 检查机器人是否存活（生命值>0则为存活）
    bool isAlive() const { return currentHp_ > 0; }

    // 受到伤害处理
    // 返回实际扣除的生命值（可能被当前生命值限制，不会扣到负数）
    // 若机器人已退场或伤害值不合法则抛出异常
    virtual int takeDamage(int damage);

    // 纯虚函数：补给操作
    // 由派生类实现具体补给规则，失败时抛出异常
    // 返回实际恢复的生命值
    virtual int supply() = 0;

    // 纯虚函数：获取机器人类型名称
    virtual std::string typeName() const = 0;

    // 获取机器人状态字符串（正常/已退场）
    std::string statusString() const {
        return isAlive() ? "正常" : "已退场";
    }
};

// ============================================================
// 第四部分：四个派生类声明
// ============================================================

// 步兵类：初始/最大生命值100，地面补给恢复20点
class Infantry : public Robot {
public:
    // 构造函数：调用基类构造函数，设置最大生命值为100
    Infantry(RobotInfo info);

    // 实现补给接口：步兵地面补给恢复20点生命值
    // 若机器人已退场，requireAlive()会抛出logic_error
    int supply() override;

    // 返回类型名称
    std::string typeName() const override;
};

// 英雄类：初始/最大生命值150，地面补给恢复30点
class Hero : public Robot {
public:
    // 构造函数：调用基类构造函数，设置最大生命值为150
    Hero(RobotInfo info);

    // 实现补给接口：英雄地面补给恢复30点生命值
    int supply() override;

    std::string typeName() const override;
};

// 哨兵类：初始/最大生命值120，地面补给恢复10点
class Sentry : public Robot {
public:
    // 构造函数：调用基类构造函数，设置最大生命值为120
    Sentry(RobotInfo info);

    // 实现补给接口：哨兵地面补给恢复10点生命值
    int supply() override;

    std::string typeName() const override;
};

// 空中机器人类：初始/最大生命值80，不支持地面补给
class AerialRobot : public Robot {
public:
    // 构造函数：调用基类构造函数，设置最大生命值为80
    AerialRobot(RobotInfo info);

    // 实现补给接口：空中机器人不支持地面补给，直接抛出异常
    // 无论是否存活都失败（题目要求：空中机器人即使满血，地面补给也应失败）
    int supply() override;

    std::string typeName() const override;
};

// ============================================================
// 第五部分：裁判系统类 RefereeSystem 声明
// ============================================================

// 裁判系统类
// 负责管理所有机器人的生命周期、处理比赛事件、统计成功事件数
class RefereeSystem {
private:
    // 使用智能指针vector管理机器人所有权
    // unique_ptr确保自动内存管理，禁止拷贝语义保证唯一所有权
    // 不得手动管理这些机器人的 new 和 delete
    std::vector<std::unique_ptr<Robot>> robots_;

    // 成功处理的事件计数器
    std::size_t successfulEvents_ = 0;

    // 处理伤害事件（内部辅助方法）
    // 查找机器人、验证参数、执行伤害，返回实际扣除量
    int handleDamage(const MatchEvent& event);

    // 处理补给事件（内部辅助方法）
    // 查找机器人、执行补给，返回实际恢复量
    int handleSupply(const MatchEvent& event);

public:
    // 登记机器人（通过右值引用转移所有权）
    // 使用std::make_unique创建机器人，将所有权交给裁判系统
    // 异常：空指针、重复编号、空名称时抛出std::invalid_argument
    void registerRobot(std::unique_ptr<Robot> robot);

    // 根据编号查找机器人
    // 返回指向机器人的指针（仅用于访问，调用者不拥有所有权，不能释放）
    // 找不到时返回nullptr
    Robot* findRobot(int id);

    // 处理比赛事件
    // 根据事件类型分发到handleDamage或handleSupply
    // 成功时递增successfulEvents_计数器
    // 返回实际扣除或恢复的生命值
    // 异常：未知事件类型抛出std::invalid_argument
    int processEvent(const MatchEvent& event);

    // 返回成功处理的事件数
    std::size_t successfulEvents() const {
        return successfulEvents_;
    }

    // 按登记顺序打印所有机器人的状态信息
    void printStatus() const;
};

} // namespace robot_system

#endif // ROBOT_REFEREE_SYSTEM_H