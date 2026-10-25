#include "types.hpp"
#include <iostream>
#include "iomanip"

namespace rm
{
  std::string teamToString(Team team)
  {
    switch(team)
    {
      case Team::Red: return "红方";
      case Team::Blue: return "蓝方";
      default:         return "未知";
    }
  }

  std::string eventTypeToString(EventType type)
  {
    switch(type)
    {
      case EventType::Damage: return "伤害";
      case EventType::Supply: return "补给";
      default               : return "未知";
    }
  }
}