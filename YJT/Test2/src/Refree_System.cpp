#include "Refree_System.h"
#include <algorithm>
#include <iostream>

std::string teamName(Team team)
{
    return team == Team::Red ? "红方" : "蓝方";
}

Robot::Robot(RobotInfo info, int maxHp)
    : m_info(info), m_hp(maxHp), m_maxHp(maxHp)
{
}
const RobotInfo& Robot::info() const
{
    return m_info;
}
int Robot::hp() const
{
    return m_hp;
}
int Robot::maxHp() const
{
    return m_maxHp;
}
bool Robot::isAlive() const
{
    return m_hp > 0;
}
int Robot::takeDamage(int damage)
{
    if (damage <= 0)
        throw std::invalid_argument("伤害值必须大于 0");
    requireAlive();

    int oldHp = m_hp;
    m_hp = std::max(0, m_hp - damage);
    return oldHp - m_hp;
}
int Robot::recover(int amount)
{
    int oldHp = m_hp;
    m_hp = std::min(m_maxHp, m_hp + amount);
    return m_hp - oldHp;
}
void Robot::requireAlive()
{
    if (!isAlive())
        throw std::logic_error("机器人已退场");
}


int Infantry::supply()
{
    requireAlive();
    return recover(20);
}
std::string Infantry::typeName() const
{
    return "步兵";
}


int Hero::supply()
{
    requireAlive();
    return recover(30);
}
std::string Hero::typeName() const
{
    return "英雄";
}


int Sentry::supply()
{
    requireAlive();
    return recover(10);
}
std::string Sentry::typeName() const
{
    return "哨兵";
}


int AerialRobot::supply()
{
    requireAlive();
    throw std::logic_error("空中机器人不支持地面补给");
}
std::string AerialRobot::typeName() const
{
    return "空中机器人";
}
//裁判系统函数
RefereeSystem::RefereeSystem()
    : m_successfulEvents(0)
{
}

void RefereeSystem::registerRobot(std::unique_ptr<Robot> robot)
{
    if (!robot)
        throw std::invalid_argument("机器人指针为空");
    if (robot->info().robotId <= 0)
        throw std::invalid_argument("机器人编号非法");
    if (robot->info().robot_name.empty())
        throw std::invalid_argument("机器人名称不能为空");
    if (findRobot(robot->info().robotId) != nullptr)
        throw std::invalid_argument("机器人编号重复");
    m_robots.push_back(std::move(robot));
}

Robot*  RefereeSystem::findRobot(int id)
{
    for (const auto& robot : m_robots)
    {
        if (robot->info().robotId == id)
            return robot.get();
    }
    return nullptr;
}

int RefereeSystem::processEvent(const MatchEvent& event)
{
    Robot* targetRobot = findRobot(event.robotId);
    if (!targetRobot)
        throw std::invalid_argument("机器人不存在");

    if (event.type == EventType::Damage)
    {
        int actualDamage = targetRobot->takeDamage(event.value);
        ++m_successfulEvents;
        return actualDamage;
    }
    if (event.type == EventType::Supply)
    {
        int supplyAmount = targetRobot->supply();
        ++m_successfulEvents;
        return supplyAmount;
    }

    throw std::invalid_argument("未知事件类型");
}

std::size_t RefereeSystem::successfulEvents() const
{
    return m_successfulEvents;
}

void RefereeSystem::printStatus() const
{
    std::cout << "=== 最终状态 ===" << std::endl;
    for (const auto& robot : m_robots)
    {
        std::cout << robot->info().robotId << ' '
                  << teamName(robot->info().team) << ' '
                  << robot->typeName() << ' '
                  << robot->info().robot_name << ' '
                  << robot->hp() << '/' << robot->maxHp()
                  << (robot->isAlive() ? " 正常" : " 已退场") << std::endl;
    }
    std::cout << "成功处理事件数：" << m_successfulEvents << std::endl;
}