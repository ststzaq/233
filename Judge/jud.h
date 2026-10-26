#ifndef JUD_H
#define JUD_H

#include <vector>
#include<memory>

#include "robot_type.h"

enum class EventType { Damage, Supply };

struct MatchEvent {
    EventType type;
    int robotId;
    int value;
};

class judge
{
private:
    std::vector<MatchEvent> events;
    std::size_t sucnum;
    std::vector<std::unique_ptr<Robot>> rbList;
public:
    judge(const std::vector<MatchEvent> a);
    void registerRobot (std::unique_ptr<Robot> robot);
    Robot* findrb (int id);
    std::size_t successfulEvents() const;
    void event_out();
    void event_go();
};

#endif