#ifndef ROBOT_TYPE.H
#define ROBOT_TYPE.H

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
    bool isalive () const;
    int takedmg (int dmg);
    int idc();
    virtual int supply () = 0;
    virtual std::string typeName () const = 0;
};

class Infantry : public Robot
{
public:
    ~Infantry ();
    int supply();
    std::string typeName () const;
};

class Hero : public Robot
{
public:
    ~Hero ();
    int supply ();
    std::string typeName () const;
};

class Sentry : public Robot
{
public:
    ~Sentry ();
    int supply ();
    std::string typeName () const;
};

class AerialRobot : public Robot
{
public:
    ~AerialRobot ();
    int supply ();
    std::string typeName () const;
};

#endif