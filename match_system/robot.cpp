#include "robot.h"

#include <algorithm>
#include <stdexcept>

// 构造：把信息保存下来，血量设成最大血量（初始满血）
Robot::Robot(RobotInfo info, int maxHp)
    : m_info(info), m_hp(maxHp), m_maxHp(maxHp) {}

// 已退场时抛 logic_error，供 takeDamage / recover 共用
void Robot::requireAlive() const
{
    if (!isAlive())
    {
        throw std::logic_error("机器人已退场");
    }
}

// 受到伤害：返回实际扣除量
int Robot::takeDamage(int damage)
{
    // 参数非法，用 invalid_argument
    if (damage <= 0)
    {
        throw std::invalid_argument("伤害值必须大于 0");
    }

    requireAlive();

    // 伤害超过当前血量时，实际只扣到 0
    int actual = std::min(damage, m_hp);
    m_hp -= actual;
    return actual;
}

// 恢复血量：不超过最大血量，返回实际恢复量
int Robot::recover(int amount)
{
    requireAlive();

    // 满血时剩余空间是 0，实际恢复量就是 0，但调用本身仍算成功
    int actual = std::min(amount, m_maxHp - m_hp);
    m_hp += actual;
    return actual;
}
