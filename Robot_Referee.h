#ifndef SQUARE_H
#define SQUARE_H
#include <iostream>
#include <string>
#include <cstddef>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <exception>
#include <memory>
#include <vector>

enum class Team { Red, Blue };

struct RobotInfo 
{
    int id;      
    std::string name;    
    Team team;   
};


enum class EventType { Damage, Supply };

struct MatchEvent 
{
    EventType type;    
    int robotId;  
    int value;    
};

class Robot 
{
public:
    Robot(RobotInfo info, int maxHp);
    virtual ~Robot() = default;

    const RobotInfo& info() const;
    int  hp() const;
    int  maxHp() const;
    bool isAlive() const;
    int takeDamage(int damage);

    virtual int supply() = 0;
    virtual std::string typeName() const = 0;
protected:
    void requireAlive() const;
    int recover(int amount);
private:
    RobotInfo info_;   
    int       maxHp_;  
    int       hp_;     
};

class Infantry : public Robot 
{
public:
    Infantry(RobotInfo info) 
        : Robot(info, 100) 
    {}
    int supply();
    std::string typeName() const 
    { return "步兵"; }
};

class Hero : public Robot 
{
public:
    Hero(RobotInfo info) 
        : Robot(info, 150) 
    {}
    int supply();
    std::string typeName() const 
    { return "英雄"; }
};

class Sentry : public Robot 
{
public:
    Sentry(RobotInfo info) 
        : Robot(info, 120) 
    {}
    int supply();
    std::string typeName() const 
    { return "哨兵"; }
};

class AerialRobot : public Robot {
public:
    AerialRobot(RobotInfo info) 
        : Robot(info, 80) 
    {}
    int supply();
    std::string typeName() const 
    { return "空中机器人"; }
};

class Referee 
{
public:
    void registerRobot(std::unique_ptr<Robot> robot);
    Robot* findRobot(int id);
    int processEvent(const MatchEvent& event);
    std::size_t successfulEvents() const;
    void printStatus() const;
private:
    std::vector<std::unique_ptr<Robot>> robots_;
    std::size_t successful_ = 0;
};
void printRobotLine(const Robot& robot);

#endif