#include "jud.h"

judge::judge(const std::vector<MatchEvent> a)
{
    events = a;
    sucnum = 0;
}

void judge::registerRobot(std::unique_ptr<Robot> robot)
{
    if (findrb (robot->idc()) != nullptr)
    {
        std::cout << "登记失败：机器人编号重复" << std::endl;
        return;
    }
    rbList.push_back (std::move(robot));
}

Robot* judge::findrb(int id)
{
    Robot* target = nullptr;
    for (const auto& i : rbList)
    {
        if (i->idc() == id)
        {
            target = i.get();
            break;
        }
    }
    return target;
}

void judge::event_go()
{
    sucnum = 0;
    Robot* trg;
    int ct = 1;
    for(auto i : events)
    {
        trg = findrb(i.robotId);
        if(i.type == EventType::Damage)
        {
            
            std::cout << "事件 " << ct << "：机器人 " << i.robotId << " 受到 " << i.value << " 点伤害";
            if(trg == nullptr)
            {
                std::cout << "->失败，机器人不存在" << std::endl;
            }
            else if(trg->isalive() == false)
            {
                std::cout << "->失败：机器人已退场" << std::endl;
            }
            else if(i.value < 0)
            {
                std::cout << "->失败，伤害值必须大于 0" << std::endl;
            }
            else
            {
                sucnum ++;
                std::cout << "->成功，实际扣除 " << trg->takedmg(i.value) << " 点，生命值：" << trg->hpc() << "/" << trg->hpm();
                if(trg->isalive() == true)
                {
                    std::cout << std::endl;
                }
                else
                {
                    std::cout << "，已退场" << std::endl;
                }
            }
        }
        else if(i.type == EventType::Supply)
        {
            std::cout << "事件 " << ct << "：机器人 " << i.robotId << " 接收补给";
            if(trg == nullptr)
            {
                std::cout << "->失败，机器人不存在" << std::endl;
            }
            else if(trg->isalive() == false)
            {
                std::cout << "->失败：机器人已退场" << std::endl;
            }
            else
            {
                sucnum ++;
                std::cout << "->成功，实际恢复 " << trg->takedmg(i.value) << " 点，生命值：" << trg->hpc() << "/" << trg->hpm() << std::endl;
            }
        }
    }
    
}