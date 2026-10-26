#include "robot.h"

// 作用：把阵营转成中文
// 参数：team —— 阵营
// 返回：中文阵营名
string team_name(Team team)
{
    if (team == Team::Red)
        return "红方";
    return "蓝方";
}

// 作用：构造函数！！！保存基础信息，让机器人一开始是满血
// 参数：info —— 编号、名称、阵营；maxHp —— 最大生命值
// 返回：无，信息不合法时抛出 invalid_argument
Robot::Robot(RobotInfo info, int maxHp)
{
    if (info.id <= 0)
        throw invalid_argument("机器人编号必须大于 0");
    if (info.name.empty())
        throw invalid_argument("机器人名称不能为空");
    if (maxHp <= 0)
        throw invalid_argument("最大生命值必须大于 0");

    robot_info = info;
    max_hp = maxHp;
    hp_now = maxHp;
}

// 作用：返回基础信息，只读
// 参数：无
// 返回：RobotInfo 的常引用
const RobotInfo& Robot::info() const
{
    return robot_info;
}

// 作用：返回当前生命值
int Robot::hp() const
{
    return hp_now;
}

// 作用：返回最大生命值
int Robot::maxHp() const
{
    return max_hp;
}

// 作用：判断机器人是否还活着
bool Robot::isAlive() const
{
    return hp_now > 0;
}

// 作用：扣血，最低扣到 0
// 参数：damage —— 伤害值，必须大于 0
// 返回：实际扣除的生命值
int Robot::takeDamage(int damage)
{
    if (damage <= 0)
        throw invalid_argument("伤害值必须大于 0");

    requireAlive();

    int actual = damage;
    if (actual > hp_now)
        actual = hp_now;

    hp_now -= actual;
    return actual;
}

// 作用：回血，不能超过最大生命值，派生类的 supply 调用它
// 参数：amount —— 想恢复的生命值
// 返回：实际恢复的生命值
int Robot::recover(int amount)
{
    requireAlive();

    int space = max_hp - hp_now;
    int actual = amount;
    if (actual > space)
        actual = space;

    hp_now += actual;
    return actual;
}

// 作用：检查机器人是否退场，退场就抛出异常
// 参数：无
// 返回：无
void Robot::requireAlive()
{
    if (hp_now <= 0)
        throw logic_error("机器人已退场");
}

Infantry::Infantry(RobotInfo info) : Robot(info, INFANTRY_MAX_HP)
{
}

// 作用：步兵地面补给，恢复 20 点
// 参数：无
// 返回：实际恢复量
int Infantry::supply()
{
    return recover(INFANTRY_SUPPLY_HP);
}

string Infantry::typeName() const
{
    return "步兵";
}

Hero::Hero(RobotInfo info) : Robot(info, HERO_MAX_HP)
{
}

// 作用：英雄地面补给，恢复 30 点
// 参数：无
// 返回：实际恢复量
int Hero::supply()
{
    return recover(HERO_SUPPLY_HP);
}

string Hero::typeName() const
{
    return "英雄";
}

Sentry::Sentry(RobotInfo info) : Robot(info, SENTRY_MAX_HP)
{
}

// 作用：哨兵地面补给，恢复 10 点
// 参数：无
// 返回：实际恢复量
int Sentry::supply()
{
    return recover(SENTRY_SUPPLY_HP);
}

string Sentry::typeName() const
{
    return "哨兵";
}

AerialRobot::AerialRobot(RobotInfo info) : Robot(info, AERIAL_MAX_HP)
{
}

// 作用：空中机器人不支持地面补给，直接抛出异常
// 参数：无
// 返回：不会正常返回
int AerialRobot::supply()
{
    requireAlive();
    throw logic_error("空中机器人不支持地面补给");
}

string AerialRobot::typeName() const
{
    return "空中机器人";
}
