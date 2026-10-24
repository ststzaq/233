#ifndef __PERCEPT_H_
#define __PERCEPT_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>

enum class Team { Red, Blue };
enum class EventType { Damage, Supply };

struct MatchEvent {
    EventType type;
    int robotId;
    int value;
};

struct RobotInfo {
    std::string name;
    Team team;
    int id;
};

class Robot {
public:
    Robot(RobotInfo info, int maxHp) {
        if (maxHp <= 0)
            throw std::invalid_argument("机器人 maxHp 非法");
        if (info.id <= 0)
            throw std::invalid_argument("机器人 id 非法");
        if (info.name.empty())
            throw std::invalid_argument("机器人名字为空");
        info_ = std::move(info);
        hp_ = maxHp;
        maxHp_ = maxHp;
    }
    int recover(int amount) {
        int healed = std::min(amount, maxHp_ - hp_);
        hp_ += healed;
        return healed;
    }
    void requireAlive() const {
        if (hp_ <= 0)
            throw std::logic_error("失败：机器人已退场");
    }
private:
    RobotInfo info_;
    int hp_;
    int maxHp_;
public:
    virtual ~Robot() = default;
    const RobotInfo& info() const {
        return info_;
    }
    int hp() const {
        return hp_;
    }
    int maxHp() const {
        return maxHp_;
    }
    bool isAlive() const {
        if (hp_ > 0)
            return true;
        else
            return false;
    }
    int takeDamage(int damage);
    virtual int supply() = 0;
    virtual std::string typeName() const = 0;
};

//步兵
class Infantry : public Robot{
public:
    explicit Infantry(RobotInfo info) : Robot(std::move(info), 100) {}
    int supply() override{
        requireAlive();
        return recover(20);
    }
    std::string typeName() const override
    {
        return "步兵";
    }
};

//英雄
class Hero : public Robot{
public:
    explicit Hero(RobotInfo info) : Robot(std::move(info), 150) {}
    int supply() override{
        requireAlive();
        return recover(30);
    }
    std::string typeName() const override
    {
        return "英雄";
    }
};

//哨兵
class Sentry : public Robot{
public:
    explicit Sentry(RobotInfo info) : Robot(std::move(info), 120) {}
    int supply() override{
        requireAlive();
        return recover(10);
    }
    std::string typeName() const override
    {
        return "哨兵";
    }
};

//空中机器人
class AerialRobot : public Robot{
public:
    explicit AerialRobot(RobotInfo info) : Robot(std::move(info), 80) {}
    int supply() override {
        throw std::logic_error("失败：空中机器人不支持地面补给");
    }
    std::string typeName() const override
    {
        return "空中机器人";
    }
};

class RefereeSystem {
private:
    std::vector<std::unique_ptr<Robot>> robots_;
    std::size_t successfulEvents_ = 0;
public:
    void registerRobot(std::unique_ptr<Robot> robot);
    Robot* findRobot(int id);
    int processEvent(const MatchEvent& event);
    std::size_t successfulEvents() const;
    void printStatus() const;
    const std::vector<std::unique_ptr<Robot>>& robots() const {
        return robots_;
    }
};



#endif