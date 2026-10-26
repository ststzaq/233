#ifndef ROBOT_TYPE_H
#define ROBOT_TYPE_H

#include <iostream>
#include <vector>

enum class Team {Red, Blue};

struct RobotInfo 
{
    int num;
    std::string name;
    Team team;
};

class Robot
{
protected:
    RobotInfo info;
    int maxhp;
    int hp;
    bool alive;
public:
    Robot (RobotInfo in, int hpmax);
    virtual ~Robot () = default;
    int hpc () const;
    int hpm () const;
    std::string namec() const;
    std::string groupc() const;
    bool isalive () const;
    int takedmg (int dmg);
    int idc();
    virtual int supply () = 0;
    virtual std::string typeName () const = 0;
};

class Infantry : public Robot
{
public:
    using Robot::Robot;
    ~Infantry () = default;
    int supply();
    std::string typeName () const;
};

class Hero : public Robot
{
public:
    using Robot::Robot;
    ~Hero () = default;
    int supply ();
    std::string typeName () const;
};

class Sentry : public Robot
{
public:
    using Robot::Robot;
    ~Sentry () = default;
    int supply ();
    std::string typeName () const;
};

class AerialRobot : public Robot
{
public:
    using Robot::Robot;
    ~AerialRobot () = default;
    int supply ();
    std::string typeName () const;
};

#endif