#include "jud.h"

judge::judge(const std::vector<MatchEvent> a)
{
    events = a;
    sucnum = 0;
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
    for (auto& i : rbList)
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
            try
            {
                std::cout << "事件 " << ct << "：机器人 " << i.robotId << " 受到 " << i.value << " 点伤害";

                if(i.value <= 0)
                {
                    std::invalid_argument("->失败，伤害值必须大于 0");
                    std::cout << "->失败，伤害值必须大于 0" << std::endl;
                }

                if(trg == nullptr)
                {
                    std::logic_error("->失败，机器人不存在");
                }
                else if(trg->isalive() == false)
                {
                    std::logic_error("->失败：机器人已退场");
                }
                

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
            catch(const std::exception& inf)
            {
                std::cout << inf.what() << std::endl;
            }
        }
        else if(i.type == EventType::Supply)
        {
            try
            {
                std::cout << "事件 " << ct << "：机器人 " << i.robotId << " 接收补给";
                if(trg == nullptr)
                {
                    std::logic_error("->失败，机器人不存在");
                }
                if(trg->isalive() == false)
                {
                    std::logic_error("->失败：机器人已退场");
                }
                if(trg->typeName() == "空中无人机")
                {
                    std::logic_error("->失败：空中机器人不支持地面补给");
                }
                else
                {
                    sucnum ++;
                    std::cout << "->成功，实际恢复 " << trg->takedmg(i.value) << " 点，生命值：" << trg->hpc() << "/" << trg->hpm() << std::endl;
                }
            }
            catch(const std::exception& inf)
            {
                std::cout << inf.what() << std::endl;
            }
        }
        ct ++;
    }
    
}

void judge::event_out()
{
    int ct = 1;
    std::string alive;
    std::cout << "=== 最终状态 ===" << std::endl;
    for(auto& i : rbList)
    {
        if(i->isalive() == true)
        {
            alive = " 正常";
        }
        else
        {
            alive = "已退场";
        }
        std::cout << ct << " " << i->groupc() << " " << i->typeName() << " " << i->namec() << " " << i->hpc()
        << "/" << i->hpm() << alive << std::endl;
    }
    std::cout << "成功处理事件数：" << sucnum;
}