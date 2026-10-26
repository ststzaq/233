#include "referee.h"

c_RefereeSystem referee;

// 作用：登记一台机器人，所有权交给裁判系统
// 参数：robot —— 机器人智能指针
// 返回：无，登记失败时抛出异常
void c_RefereeSystem::registerRobot(unique_ptr<Robot> robot)
{
    if (robot == nullptr)
        throw invalid_argument("机器人指针为空");

    if (findRobot(robot->info().id) != nullptr)
        throw invalid_argument("机器人编号重复");

    robots.push_back(move(robot));
}

// 作用：按编号查找机器人
// 参数：id —— 机器人编号
// 返回：找到返回指针，找不到返回 nullptr
Robot* c_RefereeSystem::findRobot(int id)
{
    for (std::size_t i = 0; i < robots.size(); i++)
    {
        if (robots[i]->info().id == id)
            return robots[i].get();
    }
    return nullptr;
}

// 作用：处理一条伤害或补给事件
// 参数：event —— 比赛事件
// 返回：实际扣除或恢复的生命值，失败时抛出异常
int c_RefereeSystem::processEvent(const MatchEvent& event)
{
    Robot* robot = findRobot(event.robotId);
    if (robot == nullptr)
        throw invalid_argument("机器人不存在");

    int actual = 0;

    if (event.type == EventType::Damage)
    {
        actual = robot->takeDamage(event.value);
    }
    else if (event.type == EventType::Supply)
    {
        actual = robot->supply();
    }
    else
    {
        throw invalid_argument("未知的事件类型");
    }

    success_count++;
    return actual;
}

// 作用：返回成功处理的事件数
// 参数：无
// 返回：成功事件数
std::size_t c_RefereeSystem::successfulEvents() const
{
    return success_count;
}

// 作用：按登记顺序输出所有机器人的状态
// 参数：无
// 返回：无
void c_RefereeSystem::printStatus() const
{
    for (std::size_t i = 0; i < robots.size(); i++)
    {
        const Robot* robot = robots[i].get();

        cout << robot->info().id << " "
             << team_name(robot->info().team) << " "
             << robot->typeName() << " "
             << robot->info().name << " "
             << robot->hp() << "/" << robot->maxHp() << " ";

        if (robot->isAlive())
            cout << "正常" << endl;
        else
            cout << "已退场" << endl;
    }
}
