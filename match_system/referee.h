#pragma once

#include "robot.h"

#include <cstddef>
#include <memory>
#include <vector>

// 裁判系统：管理所有机器人，处理比赛事件
class RefereeSystem
{
public:
    // 接管所有权；空指针、重复编号、基础信息非法时抛 invalid_argument
    void registerRobot(std::unique_ptr<Robot> robot);

    // 找不到返回 nullptr；返回的裸指针只借看不拥有，调用者不能 delete
    Robot *findRobot(int id);

    // 处理一条事件，返回实际扣除或恢复量；失败时抛异常
    int processEvent(const MatchEvent &event);

    // 成功处理的事件数
    std::size_t successfulEvents() const { return m_successful; }

    // 按登记顺序输出全部机器人状态
    void printStatus() const;

private:
    // 用 unique_ptr 保存，裁判系统销毁时机器人自动释放，不需要手动 delete
    std::vector<std::unique_ptr<Robot>> m_robots;
    std::size_t m_successful = 0;
};
