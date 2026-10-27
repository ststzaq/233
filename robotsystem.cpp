#include "robotsystem.h"
#include <iostream>
#include <string>

namespace robotsystem
{
    Robot::Robot(RobotInfo info, int maxHp) 
        : m_info(info), m_hp(maxHp), m_maxHp(maxHp) {}

    const RobotInfo& Robot::info() const { return m_info; }
    int Robot::hp() const { return m_hp; }
    int Robot::maxHp() const { return m_maxHp; }
    bool Robot::isAlive() const { return m_hp > 0; }
    
    void Robot::requireAlive() const 
    {
        if (!isAlive()) {
            throw std::logic_error("机器人已退场");
        }
    }

    int Robot::recover(int amount) 
    {
        requireAlive(); 
        int actualRecover = (m_hp + amount > m_maxHp) ? (m_maxHp - m_hp) : amount;
        m_hp += actualRecover;
        return actualRecover;
    }

    int Robot::takeDamage(int damage) 
    {
        requireAlive(); 
        if (damage <= 0) {
            throw std::invalid_argument("伤害值必须大于 0");
        }
        int actualDamage = (damage > m_hp) ? m_hp : damage;
        m_hp -= actualDamage;
        return actualDamage;
    }
    
    Infantry::Infantry(RobotInfo info) : Robot(info, 100) {}
    int Infantry::supply() { return recover(20); }
    std::string Infantry::typeName() const { return "步兵"; }

    Hero::Hero(RobotInfo info) : Robot(info, 150) {}
    int Hero::supply() { return recover(30); }
    std::string Hero::typeName() const { return "英雄"; }

    Sentry::Sentry(RobotInfo info) : Robot(info, 120) {}
    int Sentry::supply() { return recover(10); }
    std::string Sentry::typeName() const { return "哨兵"; }

    AerialRobot::AerialRobot(RobotInfo info) : Robot(info, 80) {}
    int AerialRobot::supply() { 
        throw std::logic_error("空中机器人不支持地面补给"); 
    }
    std::string AerialRobot::typeName() const { return "空中机器人"; }

    void RefereeSystem::registerRobot(std::unique_ptr<Robot> robot) 
    {

        if (!robot) 
        {
            throw std::invalid_argument("登记的机器人指针为空");
        }

        if (robot->info().name.empty()) 
        {
            throw std::invalid_argument("机器人名称不能为空");
        }

        if (findRobot(robot->info().id) != nullptr) 
        {
            throw std::invalid_argument("机器人编号重复");
        }

        m_robots.push_back(std::move(robot));
    }

    Robot* RefereeSystem::findRobot(int id) 
    {
        for (const auto& robot : m_robots) 
        {
            if (robot->info().id == id) 
            {
                return robot.get(); 
            }
        }
        return nullptr;
    }


    int RefereeSystem::processEvent(const MatchEvent& event) 
    {

        Robot* robot = findRobot(event.robotId);
        if (!robot) {
            throw std::invalid_argument("机器人不存在");
        }

        int actualValue = 0;
        if (event.type == EventType::Damage) 
        {
            actualValue = robot->takeDamage(event.value);
        } 
        else if (event.type == EventType::Supply) 
        {

            actualValue = robot->supply(); 
        } 
        else {
            throw std::invalid_argument("未知的事件类型");
        }

        m_successfulEventsCount++;
        return actualValue;
    }

    std::size_t RefereeSystem::successfulEvents() const 
    {
        return m_successfulEventsCount;
    }


    void RefereeSystem::printStatus() const 
    {
        for (const auto& robot : m_robots) 
        {
            std::cout << robot->info().id << " ";
            
        
            if (robot->info().team == Team::Red) std::cout << "红方 ";
            else std::cout << "蓝方 ";

            std::cout << robot->typeName() << " "
                      << robot->info().name << " "
                      << robot->hp() << "/" << robot->maxHp() << " "
                      << (robot->isAlive() ? "正常" : "已退场") << "\n";
        }
    }
}