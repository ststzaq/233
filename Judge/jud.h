#ifndef JUD.H
#define JUD.H

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
protected:
    judge(const std::vector<MatchEvent> a);
    void registerRobot (std::unique_ptr<Robot> robot);
    Robot* findrb (int id);
    void event_in();
    void event_go();
    std::size_t successfulEvents() const;
};

#endif