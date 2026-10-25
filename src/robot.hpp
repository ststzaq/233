#pragma once

#include <string>

#include "types.hpp"

namespace rm {

class Robot {
public:
    Robot(RobotInfo info, int maxHp);
    virtual ~Robot() = default;

    // 多态基类禁止值拷贝，避免切片
    Robot(const Robot&)            = delete;
    Robot& operator=(const Robot&) = delete;

    // 只读访问器
    const RobotInfo& info()    const noexcept;
    int              hp()      const noexcept;
    int              maxHp()   const noexcept;
    bool             isAlive() const noexcept;

    // 非虚：扣血、退场规则对所有机型一致
    // 返回实际扣除量；damage <= 0 抛 invalid_argument，已退场抛 logic_error
    int takeDamage(int damage);

    // 纯虚：补给效果与类型名由派生类决定
    virtual int supply() = 0;
    virtual std::string typeName() const = 0;

protected:
    // 派生类复用规则用，不对外暴露
    void requireAlive() const;   // 已退场抛 logic_error
    int  recover(int amount);    // 返回实际恢复量，封顶 maxHp

private:
    RobotInfo info_;
    int       hp_;
    int       maxHp_;
};

// 统一的单机打印，格式只在这里维护
void printRobot(const Robot& robot);

} // namespace rm