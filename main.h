#ifndef MAIN_H
#define MAIN_H

#include<cstddef>
#include<stdexcept>
#include<set>
#include<memory>

//机器人
enum class Team { Red, Blue };
struct RobotInfo{ 
    int id;
    std::string name;
    Team team;
};

//事件
enum class EventType { Damage, Supply };
struct MatchEvent {
    EventType type;
    int robotId;
    int value;
};

class Robot{
    private:

    int rhp,max;
    RobotInfo information;
    public:

    Robot(RobotInfo info, int maxHp):information(info),rhp(maxHp),max(maxHp){};

    const RobotInfo& info() const { return information; };

    virtual ~Robot() = default;
    int hp() const{return rhp;}
    int maxHp() const{return max;}
    bool isAlive() const{return rhp>0;}
    int takeDamage(int damage);

    virtual int supply() = 0;

    virtual std::string typeName() const = 0;
    
    void requirealive();
    int recover(int recover);
};


class Infantry:public Robot{
    public:
    Infantry(RobotInfo info):Robot(info,100){};
    int supply(){return recover(20);}
    std::string typeName() const {return "步兵";}
};
class Hero:public Robot{
    public:
    Hero(RobotInfo info):Robot(info,150){};
    int supply(){return recover(30);}
    std::string typeName() const {return "英雄";}
};
class Sentry:public Robot{
    public:
    Sentry(RobotInfo info):Robot(info,120){};
    int supply(){return recover(10);}
    std::string typeName() const {return "哨兵";}
};
class AerialRobot:public Robot{
    public:
    AerialRobot(RobotInfo info):Robot(info,80){};
    int supply(){throw std::logic_error("空中机器人不支持地面不及");}
    std::string typeName() const {return "空中机器人";}
};



class referee{
    private:
    std::vector<std::unique_ptr<Robot>> robots;
    std::set<int>useIds;
    std::size_t succeddCount=0;
    public:
    void registerRobot(std::unique_ptr<Robot> robot);
    Robot* findRobot(int id);
    int processEvent(const MatchEvent& event);
    std::size_t successfulEvents() const { return succeddCount; };
    void printStatus() const;



};





#endif