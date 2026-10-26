#include"Robot_Referee.h"
Robot::Robot(RobotInfo info, int maxHp)
    : info_(info), maxHp_(maxHp), hp_(maxHp) 
{}

const RobotInfo& Robot::info() const 
{ 
    return info_; 
}
int  Robot::hp() const 
{ 
    return hp_; 
}
int  Robot::maxHp() const 
{ 
    return maxHp_; 
}
bool Robot::isAlive() const 
{ 
    return hp_ > 0; 
}
int Robot::takeDamage(int damage) 
{
    if (damage <= 0) 
    {
        throw std::invalid_argument("伤害值必须大于 0");
    }
    requireAlive();
    int actualdamage = damage;
    if (damage > hp_) 
    {
        actualdamage = hp_;
    }
    hp_ -= actualdamage;
    return actualdamage;
}

void Robot::requireAlive() const 
{
    if (!isAlive()) 
    {
        throw std::logic_error("机器人已退场");
    }
}

int Robot::recover(int amount) 
{
    requireAlive();
    int actualamount = amount; 
    if (hp_ + amount > maxHp_)
    {
        actualamount = maxHp_ - hp_;
    }
    hp_ += actualamount;
    return actualamount;
}

int Infantry::supply() 
{
    return recover(20);
}

int Hero::supply() 
{
    return recover(30);
}

int Sentry::supply() 
{
    return recover(10);
}

int AerialRobot::supply() 
{
    throw std::logic_error("空中机器人不支持地面补给");
}

void Referee::registerRobot(std::unique_ptr<Robot> robot) 
{
    if (!robot) 
    {
        throw std::invalid_argument("机器人指针为空");
    }
    const RobotInfo& info = robot->info();
    if (info.id <= 0) 
    {
        throw std::invalid_argument("机器人编号非法");
    }
    if (info.name.empty()) 
    {
        throw std::invalid_argument("机器人名称不能为空");
    }
    if (findRobot(info.id) != nullptr) 
    {   
        throw std::invalid_argument("机器人编号重复");
    }
    robots_.push_back(std::move(robot));
}

Robot* Referee::findRobot(int id) 
{
    for (auto& robot : robots_) 
    {
        if (robot->info().id == id) 
        {
            return robot.get();
        }
    }
    return nullptr;
}

int Referee::processEvent(const MatchEvent& event) 
{
    Robot* robot = findRobot(event.robotId);
    if (!robot) 
    {
        throw std::invalid_argument("机器人不存在");
    }
    if (event.type == EventType::Damage) 
    {
        int actualdamage = robot->takeDamage(event.value);
        successful_++;  
        return actualdamage;
    }
    if (event.type == EventType::Supply) 
    {
        int actualsupply = robot->supply();
        successful_++;
        return actualsupply;
    }
    throw std::invalid_argument("未知的事件类型");
}

std::size_t Referee::successfulEvents() const 
{
    return successful_; 
}

void Referee::printStatus() const 
{
    std::cout << "=== 最终状态 ===" << std::endl;
    for (const auto& robot : robots_) 
    {
        printRobotLine(*robot);
    }
}

void printRobotLine(const Robot& robot) 
{
    std::cout << robot.info().id << " ";
    if (robot.info().team == Team::Red)
    {
        std::cout << "红方 ";
    }
    else
    {
        std::cout << "蓝方 ";
    }
    std::cout << robot.typeName() << " ";
    std::cout << robot.info().name << " ";
    std::cout << robot.hp() << "/" << robot.maxHp() << " ";
    if (robot.isAlive()) 
    {
        std::cout << "正常" << std::endl;
    } 
    else
    {
        std::cout << "已退场" << std::endl;
    }
}
