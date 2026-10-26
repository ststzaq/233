#ifndef __REFEREE_H_
#define __REFEREE_H_
#include <vector>
#include <memory>
#include <cstddef>
#include "robot.h"
using namespace std;

class c_RefereeSystem
{
public:
    void registerRobot(unique_ptr<Robot> robot);       // [题目给定]
    Robot* findRobot(int id);                          // [题目给定]
    int processEvent(const MatchEvent& event);         // [题目给定]
    std::size_t successfulEvents() const;              // [题目给定]
    void printStatus() const;                          // [题目给定]

private:
    // [题目给定] 用 vector<unique_ptr<Robot>> 保存机器人
    std::vector<std::unique_ptr<Robot>> robots;
    std::size_t success_count = 0;
};

extern c_RefereeSystem referee;

#endif
