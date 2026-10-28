#pragma once

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

enum class EventType { Damage, Supply };//枚举操作类型
enum class Team { Red, Blue };//枚举队伍类型

std::string teamName(Team team);
struct MatchEvent 
{
    EventType type;
    int robotId;//编号
    int value;
};

struct RobotInfo
{
    int robotId;
    Team team;
    std::string robot_name;//名字
};

class Robot//抽象基类
{
public:
    Robot(RobotInfo info, int maxHp);
    virtual ~Robot() = default;
    const RobotInfo& info() const;
    int hp() const;
    int maxHp() const;
    bool isAlive() const;
    int takeDamage(int damage);

    virtual int supply() = 0;
    virtual std::string typeName() const = 0;
private:
    RobotInfo m_info;
    int m_hp;
    int m_maxHp;

protected:
    int recover(int amount);
    void requireAlive();
};
//步兵子类
class Infantry : public Robot
{
    public:
        Infantry(RobotInfo info, int maxHp) : Robot(info, maxHp) {}
        int supply() override;
        std::string typeName() const override;
};
//英雄子类
class Hero : public Robot
{
    public:
        Hero(RobotInfo info, int maxHp) : Robot(info, maxHp) {}
        int supply() override;
        std::string typeName() const override;
};
//哨兵子类
class Sentry : public Robot
{
    public:
        Sentry(RobotInfo info, int maxHp) : Robot(info, maxHp) {}
        int supply() override;
        std::string typeName() const override;
};
//空中机器人子类
class AerialRobot : public Robot
{
    public:
        AerialRobot(RobotInfo info, int maxHp) : Robot(info, maxHp) {}
        int supply() override;
        std::string typeName() const override;
};

//裁判系统
class RefereeSystem
{   
    private:
        std::vector<std::unique_ptr<Robot>> m_robots;
        std::size_t m_successfulEvents = 0;
    public:
        RefereeSystem();
        void registerRobot(std::unique_ptr<Robot> robot);
        Robot* findRobot(int id);
        int processEvent(const MatchEvent& event);
        std::size_t successfulEvents() const;
        void printStatus() const;
};