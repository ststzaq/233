#ifndef __ROBOT_H_
#define __ROBOT_H_
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

#define INFANTRY_MAX_HP     100
#define INFANTRY_SUPPLY_HP  20
#define HERO_MAX_HP         150
#define HERO_SUPPLY_HP      30
#define SENTRY_MAX_HP       120
#define SENTRY_SUPPLY_HP    10
#define AERIAL_MAX_HP       80

// [题目给定] 阵营枚举
enum class Team { Red, Blue };

// [题目给定] 机器人基础信息
struct RobotInfo {
    int id;
    string name;
    Team team;
};

// [题目给定] 事件类型
enum class EventType { Damage, Supply };

// [题目给定] 比赛事件结构体
struct MatchEvent {
    EventType type;
    int robotId;
    int value;
};

class Robot
{
public:
    Robot(RobotInfo info, int maxHp);          // [题目给定]
    virtual ~Robot() = default;                // [题目给定]

    const RobotInfo& info() const;             // [题目给定]
    int hp() const;                            // [题目给定]
    int maxHp() const;                         // [题目给定]
    bool isAlive() const;                      // [题目给定]
    int takeDamage(int damage);                // [题目给定]

    virtual int supply() = 0;                  // [题目给定]
    virtual string typeName() const = 0;       // [题目给定]

protected:
    int recover(int amount);
    void requireAlive();

private:
    RobotInfo robot_info;
    int hp_now;
    int max_hp;
};

class Infantry : public Robot
{
public:
    Infantry(RobotInfo info);
    int supply();
    string typeName() const;
};

class Hero : public Robot
{
public:
    Hero(RobotInfo info);
    int supply();
    string typeName() const;
};

class Sentry : public Robot
{
public:
    Sentry(RobotInfo info);
    int supply();
    string typeName() const;
};

class AerialRobot : public Robot
{
public:
    AerialRobot(RobotInfo info);
    int supply();
    string typeName() const;
};

// 作用：把阵营转成中文，输出时用
// 参数：team —— 阵营
// 返回：中文阵营名
string team_name(Team team);

#endif
