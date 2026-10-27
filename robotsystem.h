#pragma once

#include <string>
#include <vector>
#include <memory>

namespace robotsystem
{
    enum class Team { Red , Blue };            //Team枚举

    struct RobotInfo                           //RobotInfo结构体
    {
        int id;
        std::string name;
        Team team;
    };
    enum class EventType { Damage, Supply };    //Eventtype枚举

    struct MatchEvent                            //Match结构体
    {
        EventType type;
        int robotId;
        int value;
    };
    class Robot                                   //抽象基类Robot
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

    protected:
        int recover(int amount);
        void requireAlive() const;

    private:
        RobotInfo m_info;
        int m_hp;
        int m_maxHp;
    };

    class Infantry : public Robot                 //步兵
    {
    public:
        Infantry(RobotInfo info);
        int supply() override;
        std::string typeName() const override;
    };
        
        
    class Hero : public Robot                      //英雄
    {
    public:
        Hero(RobotInfo info);
        int supply() override;
        std::string typeName() const override;
    };
    

    class Sentry : public Robot                    //哨兵
    {
    public:
        Sentry(RobotInfo info);
        int supply() override;
        std::string typeName() const override;
        };


    class AerialRobot : public Robot                //空中机器人
    {
    public:
        AerialRobot(RobotInfo info);
        int supply() override;
        std::string typeName() const override;
    };

            
    class RefereeSystem                             //裁判系统
    {
    public:
        void registerRobot(std::unique_ptr<Robot> robot);
        Robot* findRobot(int id);
        int processEvent(const MatchEvent& event);
        std::size_t successfulEvents() const;
        void printStatus() const;
    private:
        std::vector<std::unique_ptr<Robot>> m_robots;
        std::size_t m_successfulEventsCount = 0;
    };
    
}