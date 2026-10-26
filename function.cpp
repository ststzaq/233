#include"main.h"
#include<iostream>
#include<string>
#include<algorithm>


Robot* referee::findRobot(int id){
    auto find=std::find_if(
        robots.begin(),
        robots.end(),
        [id](const auto& robots){return robots->info().id==id;}
    );
    if(find==robots.end()){return nullptr;}
    return find->get();


};

void referee::registerRobot(std::unique_ptr<Robot> robot){
    if(robot==nullptr){throw std::invalid_argument("没有机器人");}
    if(robot->info().name.empty()){throw std::invalid_argument("未命名机器人");}
    if(robot->info().id<0){throw std::invalid_argument("机器人编号必须大于零");}
    if(findRobot(robot->info().id)!=nullptr){throw std::invalid_argument("机器人编号重复");}
    robots.push_back(std::move(robot));
};

int Robot::takeDamage(int damage){
    requirealive();
    int val=damage<rhp?damage:rhp;
    rhp-=val;
    return val;
};

int Robot::recover(int recover){
    requirealive();
    int temp=max-rhp;
    int val=recover<temp?recover:temp;
    rhp+=val;
    return val;
};

void Robot::requirealive(){
    if(!isAlive()){throw std::logic_error("机器人退场");}
}


int referee::processEvent(const MatchEvent& event){
    if(event.type==EventType::Damage){
        if(event.value<=0){throw std::invalid_argument("伤害值必须大于0");}
        Robot* robot=findRobot(event.robotId);
        if(robot==nullptr){throw std::invalid_argument("机器人不存在");}
        int val=robot->takeDamage(event.value);
        succeddCount++;
        return val;
    }
    else if(event.type==EventType::Supply){
        Robot* robot=findRobot(event.robotId);
        if(robot==nullptr){throw std::invalid_argument("机器人不存在");}
        int val=robot->supply();
        succeddCount++;
        return val;
    }
    else throw std::invalid_argument("未知的事件类型");

};
std::string trans(Team team){return team==Team::Red?"红方":"蓝方";}
void referee::printStatus() const{
    for(const auto& robot:robots){
        std::cout<<robot->info().id<<" "<<trans(robot->info().team)<<" "<<robot->typeName()<<" "<<robot->info().name<<" "<<robot->hp()<<"/"<<robot->maxHp()<<" "<<(robot->isAlive()?"正常":"退场")<<std::endl;
    }
};
