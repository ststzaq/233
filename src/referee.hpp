#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "robot.hpp"

namespace rm {

class RefereeSystem {
public:
    RefereeSystem() = default;
    ~RefereeSystem() = default;

    // 禁止拷贝：内部持有 unique_ptr
    RefereeSystem(const RefereeSystem&)            = delete;
    RefereeSystem& operator=(const RefereeSystem&) = delete;

    // 登记；空指针/重复编号/信息非法抛 invalid_argument，失败不改变内部状态
    void registerRobot(std::unique_ptr<Robot> robot);

    // 只读访问；找不到返回 nullptr，调用方不得 delete
    Robot* findRobot(int id);

    // 返回实际扣除或恢复量；失败抛异常且不增加成功计数
    int processEvent(const MatchEvent& event);

    std::size_t successfulEvents() const noexcept;

    // 按登记顺序输出所有机器人
    void printStatus() const;

private:
    std::vector<std::unique_ptr<Robot>> robots_;
    std::size_t                         successCount_ = 0;
};

} // namespace rm

const std::vector<rm::MatchEvent> events = {
    {rm::EventType::Damage, 1, 35},
    {rm::EventType::Damage, 2, 50},
    {rm::EventType::Supply, 1, 0},
    {rm::EventType::Supply, 2, 0},
    {rm::EventType::Supply, 4, 0},
    {rm::EventType::Damage, 3, 200},
    {rm::EventType::Supply, 3, 0},
    {rm::EventType::Damage, 99, 10},
    {rm::EventType::Damage, 1, -5},
    {rm::EventType::Supply, 1, 0},
    {rm::EventType::Supply, 1, 0}
};