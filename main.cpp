#include"main.h"
#include<iostream>
#include<vector>
#include<string>
int main(){
    referee system;
    try{
        std::cout<<"=== 登记机器人 ==="<<"\n";
        system.registerRobot(std::make_unique<Infantry>(RobotInfo{1,"王八",Team::Red}));
        system.registerRobot(std::make_unique<Hero>(RobotInfo{2,"英雄",Team::Red}));
        system.registerRobot(std::make_unique<Sentry>(RobotInfo{3,"过洞",Team::Blue}));
        system.registerRobot(std::make_unique<AerialRobot>(RobotInfo{4,"无人机",Team::Blue}));
    }catch(const std::exception& baocuo){std::cout<<"登记失败:"<<baocuo.what()<<std::endl;}
    system.printStatus();
    try{
        system.registerRobot(std::make_unique<Infantry>(RobotInfo{1,"重复编号",Team::Blue}));
    }catch(const std::exception& e){
        std::cout<<"登记失败："<<e.what()<<std::endl;
    }
    
    const std::vector<MatchEvent> events={
        {EventType::Damage,1,35},
        {EventType::Damage,2,50},
        {EventType::Supply,1,0},
        {EventType::Supply,2,0},
        {EventType::Supply,4,0},
        {EventType::Damage,3,200},
        {EventType::Supply,3,0},
        {EventType::Damage,99,10},
        {EventType::Damage,1,-5},
        {EventType::Supply,1,0},
        {EventType::Supply,1,0}
    };
    std::cout<<"\n=== 处理比赛事件 ==="<<std::endl;
    for(int i=0;i<events.size();i++){
        const MatchEvent& event=events[i];
        std::cout<<"事件 "<<(i+1)<<":";
        if(event.type==EventType::Damage){std::cout<<"机器人 "<<event.robotId<<" 受到 "<<event.value<<" 点伤害 -> ";}
        else if(event.type==EventType::Supply){std::cout<<"机器人 "<<event.robotId<<" 接收补给 -> ";}
        try{
            Robot* robot=system.findRobot(event.robotId);
            if(event.type==EventType::Damage){
                int deduct=system.processEvent(event);
                std::cout<<"成功，实际扣除"<<deduct<<"点，生命值："<<robot->hp()<<"/"<<robot->maxHp();
                if(!robot->isAlive()){std::cout<<"，已退场";}
                std::cout<<"\n";
            }
            else if(event.type==EventType::Supply){
                int recover=system.processEvent(event);
                std::cout<<"成功，实际回复"<<recover<<"点，生命值："<<robot->hp()<<"/"<<robot->maxHp()<<std::endl;
            }
            else system.processEvent(event);
        }catch(const std::exception& e){std::cout<<"失败："<<e.what()<<std::endl;}
    }
    std::cout<<"\n"<<"=== 最终状态 ==="<<std::endl;
    system.printStatus();
    std::cout<<"成功处理事件数："<<system.successfulEvents()<<std::endl;
}
