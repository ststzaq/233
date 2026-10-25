#include "robot_type.h"

Robot:: Robot (RobotInfo in, int hpmax) 
{
    info = in;
    maxhp = hpmax;
    hp = hpmax;
}

int Robot::hpc () const
{
    return hp;
}

int Robot::hpm () const
{
    return maxhp;
}

bool Robot::isalive () const
{
    bool alive = true;
    if (hp <= 0)
    {
        alive = false;
    }
    return alive;
}

int Robot::takedmg (int dmg)
{
    if (dmg > hp)
    {
        dmg = hp;
    }
    hp -= dmg;
    return dmg;
}

int Infantry::supply ()
{
    int rest = maxhp - hp;
    int sup = 20;
    if (rest < sup)
    {
        sup = rest;
    }
    hp += sup;
    return sup;
}

std::string Infantry::typeName () const
{
    std::string name = "步兵";
    return name;
}

int Hero::supply ()
{
    int rest = maxhp - hp;
    int sup = 30;
    if (rest < sup)
    {
        sup = rest;
    }
    hp += sup;
    return sup;
}

std::string Hero::typeName () const
{
    std::string name = "英雄";
    return name;
}

int Sentry::supply ()
{
    int rest = maxhp - hp;
    int sup = 10;
    if (rest < sup)
    {
        sup = rest;
    }
    hp += sup;
    return sup;
}

std::string Sentry::typeName () const
{
    std::string name = "哨兵";
    return name;
}

int AerialRobot::supply ()
{
    int rest = maxhp - hp;
    int sup = 10;
    if (rest < sup)
    {
        sup = rest;
    }
    hp += sup;
    return sup;
}

std::string AerialRobot::typeName () const
{
    std::string name = "哨兵";
    return name;
}

//    int hpc () const;
//    int hpm () const;
//    bool isalive () const;
//    int takedmg (int dmg);