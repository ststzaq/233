#pragma once

#include <string>
#include <stdexcept>

// 阵营：只有红蓝两种
enum class Team
{
    Red,
    Blue
};

// 机器人的基础信息（编号、名称、阵营）
struct RobotInfo
{
    int id;
    std::string name;
    Team team;
};

// 事件类型：伤害或补给
enum class EventType
{
    Damage,
    Supply
};

// 一条比赛事件：类型、对哪台机器人、数值
// value 对伤害事件是伤害值；补给事件忽略，题目要求填 0
struct MatchEvent
{
    EventType type;
    int robotId;
    int value;
};

// 抽象基类：所有机器人共有的状态和规则
class Robot
{
public:
    // 构造时把血量设为满血
    Robot(RobotInfo info, int maxHp);

    // 虚析构：用基类指针 delete 时，子类的析构函数也会执行
    virtual ~Robot() = default;

    // 只读访问，外面拿不到可修改的引用
    const RobotInfo &info() const { return m_info; }
    int hp() const { return m_hp; }
    int maxHp() const { return m_maxHp; }
    bool isAlive() const { return m_hp > 0; }

    // 返回实际扣除量；参数非法抛 invalid_argument，已退场抛 logic_error
    int takeDamage(int damage);

    // 纯虚函数：基类不实现，四个子类必须各自实现
    virtual int supply() = 0;
    virtual std::string typeName() const = 0;

protected:
    // 给子类复用的两条规则
    int recover(int amount);      // 恢复血量，不超过上限，返回实际恢复量
    void requireAlive() const;    // 已退场则抛异常

private:
    // 数据私有：外面改不了，只能通过上面的函数改
    RobotInfo m_info;
    int m_hp;
    int m_maxHp;
};

// 步兵：100 血，补给 20
class Infantry : public Robot
{
public:
    // 初始化列表里调用基类构造，把最大血量定成 100
    explicit Infantry(RobotInfo info) : Robot(info, 100) {}
    int supply() override { return recover(20); }
    std::string typeName() const override { return "步兵"; }
};

// 英雄：150 血，补给 30
class Hero : public Robot
{
public:
    explicit Hero(RobotInfo info) : Robot(info, 150) {}
    int supply() override { return recover(30); }
    std::string typeName() const override { return "英雄"; }
};

// 哨兵：120 血，补给 10
class Sentry : public Robot
{
public:
    explicit Sentry(RobotInfo info) : Robot(info, 120) {}
    int supply() override { return recover(10); }
    std::string typeName() const override { return "哨兵"; }
};

// 空中机器人：80 血，不支持地面补给
class AerialRobot : public Robot
{
public:
    explicit AerialRobot(RobotInfo info) : Robot(info, 80) {}

    // 直接抛异常：状态不允许的操作，用 logic_error
    int supply() override { throw std::logic_error("空中机器人不支持地面补给"); }

    std::string typeName() const override { return "空中机器人"; }
};
