#include "percept.h"

#include <iostream>
#include <utility>
// std::move

namespace rm
{
    namespace judge
    {

        // 受伤/恢复/退场校验
        int Robot::takeDamage(int damage)
        {
            requireAlive(); // 已退场的机器人不能再受伤
            if (damage <= 0)
                throw std::invalid_argument("伤害值必须大于 0");
            const int actual = (damage < hp_) ? damage : hp_;
            hp_ -= actual;
            return actual;
        }

        int Robot::recover(int amount)
        {
            if (amount < 0)
                throw std::invalid_argument("恢复量不能为负数");
            const int space = maxHp_ - hp_; // 还能恢复多少
            const int actual = (amount < space) ? amount : space;
            hp_ += actual;
            return actual; // 满血补给返回 0，但不算失败
        }

        void Robot::requireAlive() const
        {
            if (!isAlive())
                throw std::logic_error("机器人已退场");
        }

        std::unique_ptr<Robot> makeRobot(RobotKind kind, const RobotInfo &info)
        {
            switch (kind)
            {
            case RobotKind::Infantry:
                return std::make_unique<Infantry>(info);

            case RobotKind::Hero:
                return std::make_unique<Hero>(info);
            case RobotKind::Sentry:
                return std::make_unique<Sentry>(info);
            case RobotKind::Aerial:
                return std::make_unique<AerialRobot>(info);
            }
            throw std::invalid_argument("未知的机器人类型");
        }

        void RefereeSystem::registerRobot(std::unique_ptr<Robot> robot)
        {
            // 四道校验
            if (!robot)
                throw std::invalid_argument("登记的机器人指针为空");
            if (robot->info().name.empty())
                throw std::invalid_argument("机器人名称不能为空");
            if (robot->info().id <= 0)
                throw std::invalid_argument("机器人编号必须为正整数");
            if (findRobot(robot->info().id) != nullptr)
                throw std::invalid_argument("机器人编号重复");

            robots_.push_back(std::move(robot)); // 所有权移交裁判系统
        }

        Robot *RefereeSystem::findRobot(int id)
        {
            return const_cast<Robot *>(static_cast<const RefereeSystem *>(this)->findRobot(id));
        }

        const Robot *RefereeSystem::findRobot(int id) const
        {
            for (const auto &robot : robots_)
            {
                if (robot->info().id == id)
                    return robot.get();
            }
            return nullptr;
        }

        int RefereeSystem::processEvent(const MatchEvent &event)
        {
            Robot *robot = findRobot(event.robotId);
            if (robot == nullptr)
                throw std::logic_error("机器人不存在");

            int amount = 0;
            switch (event.type)
            {
            case EventType::Damage:
                amount = robot->takeDamage(event.value);
                break;
            case EventType::Supply:
                // 恢复多少由派生类决定，裁判系统不按类型分支
                amount = robot->supply();
                break;
            default:
                throw std::invalid_argument("未知的事件类型");
            }

            ++successfulEvents_;
            return amount;
        }

        void RefereeSystem::printRobot(int id) const
        {
            const Robot *robot = findRobot(id);
            if (robot != nullptr)
                printRobotLine(*robot);
        }

        void RefereeSystem::printStatus() const
        {
            for (const auto &robot : robots_)
                printRobotLine(*robot);
        }

        void RefereeSystem::printRobotLine(const Robot &robot) const
        {
            std::cout << robot.info().id << ' '
                      << teamName(robot.info().team) << ' '
                      << robot.typeName() << ' '
                      << robot.info().name << ' '
                      << robot.hp() << '/' << robot.maxHp() << ' '
                      << (robot.isAlive() ? "正常" : "已退场") << '\n';
        }

        std::string teamName(Team team)
        {
            return (team == Team::Red) ? "红方" : "蓝方";
        }

    } // namespace judge
} // namespace rm
