#pragma once

#include <string>

namespace rm {

enum class Team {
    Red,
    Blue
};

enum class EventType {
    Damage,
    Supply
};

// 纯数据聚合，不含行为，成员全部 public
struct RobotInfo {
    int         id   = 0;
    std::string name;
    Team        team = Team::Red;
};

struct MatchEvent {
    EventType type    = EventType::Damage;
    int       robotId = 0;
    int       value   = 0;
};

// 显示辅助，自由函数；实现在 types.cpp
std::string teamToString(Team team);
std::string eventTypeToString(EventType type);

} // namespace rm