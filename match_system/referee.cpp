#include "referee.h"

#include <iostream>
#include <stdexcept>

// 登记机器人
void RefereeSystem::registerRobot(std::unique_ptr<Robot> robot)
{
    if (!robot)
    {
        throw std::invalid_argument("机器人指针不能为空");
    }

    const RobotInfo &info = robot->info();

    // 基础信息校验
    if (info.id <= 0)
    {
        throw std::invalid_argument("机器人编号非法");
    }
    if (info.name.empty())
    {
        throw std::invalid_argument("机器人名称不能为空");
    }

    // 编号全系统唯一，红蓝双方也不能重复
    if (findRobot(info.id) != nullptr)
    {
        throw std::invalid_argument("机器人编号重复");
    }

    // 用 move 把所有权交给裁判系统，而不是复制
    m_robots.push_back(std::move(robot));
}

// 按编号查找；找不到返回 nullptr
Robot *RefereeSystem::findRobot(int id)
{
    for (auto &robot : m_robots)
    {
        if (robot->info().id == id)
        {
            return robot.get();   // 从 unique_ptr 取出裸指针，只借不拥有
        }
    }
    return nullptr;
}

// 处理一条事件
int RefereeSystem::processEvent(const MatchEvent &event)
{
    // 只认 Damage 和 Supply 两种类型
    if (event.type != EventType::Damage && event.type != EventType::Supply)
    {
        throw std::invalid_argument("未知的事件类型");
    }

    Robot *robot = findRobot(event.robotId);
    if (robot == nullptr)
    {
        throw std::invalid_argument("机器人不存在");
    }

    int delta = 0;

    if (event.type == EventType::Damage)
    {
        delta = robot->takeDamage(event.value);
    }
    else
    {
        // 通过基类指针调用，具体恢复量由子类决定
        delta = robot->supply();
    }

    // 只有前面全部成功，才增加成功计数；
    // 中途抛异常时这一行不会执行，满足“失败不能增加成功事件数”
    ++m_successful;
    return delta;
}

// 按登记顺序输出全部机器人
void RefereeSystem::printStatus() const
{
    for (const auto &robot : m_robots)
    {
        const Robot &r = *robot;
        std::cout << r.info().id << ' '
                  << (r.info().team == Team::Red ? "红方" : "蓝方") << ' '
                  << r.typeName() << ' '
                  << r.info().name << ' '
                  << r.hp() << '/' << r.maxHp() << ' '
                  << (r.isAlive() ? "正常" : "已退场") << '\n';
    }
}
