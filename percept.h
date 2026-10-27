#ifndef ROBOT_JUDGE_H
#define ROBOT_JUDGE_H

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace rm
{
    namespace judge
    {

        enum class Team
        {
            Red,
            Blue
        };
        enum class EventType
        {
            Damage,
            Supply
        };

        // 身份信息，共用空间，不能为空
        struct RobotInfo
        {
            int id;
            std::string name;
            Team team;
        };

        // 伤害事件的 value 为伤害值（必须 > 0），补给事件忽略 value（填 0）
        struct MatchEvent
        {
            EventType type;
            int robotId;
            int value;
        };

        /**
         * Robot —— 抽象基类
         * 生命值等状态私有；受伤规则统一在基类，补给规则由派生类决定。
         */
        class Robot
        {
        public:
            Robot(const RobotInfo &info, int maxHp)
                : info_(info), maxHp_(maxHp), hp_(maxHp)
            {
            }

            virtual ~Robot() = default;

            const RobotInfo &info() const { return info_; }
            int hp() const { return hp_; }
            int maxHp() const { return maxHp_; }
            bool isAlive() const { return hp_ > 0; } // 生命值为 0 即退场

            int takeDamage(int damage); // 返回实际扣除量，生命值最低为 0

            virtual int supply() = 0; // 返回实际恢复量，失败抛异常
            virtual std::string typeName() const = 0;

        protected:
            int recover(int amount);   // 恢复生命值，不超过最大值；返回实际恢复量
            void requireAlive() const; // 已退场则抛 std::logic_error

        private:
            RobotInfo info_;
            int maxHp_;
            int hp_;
        };

        // 四个派生类：“初始生命值 / 单次补给量 / 类型名”
        class Infantry : public Robot
        {
        public:
            static const int kInitialHp = 100;
            static const int kSupplyAmount = 20;

            explicit Infantry(const RobotInfo &info) : Robot(info, kInitialHp) {}

            int supply() override
            {
                requireAlive();
                return recover(kSupplyAmount);
            }
            std::string typeName() const override { return "步兵"; }
        };

        class Hero : public Robot
        {
        public:
            static const int kInitialHp = 150;
            static const int kSupplyAmount = 30;

            explicit Hero(const RobotInfo &info) : Robot(info, kInitialHp) {}

            int supply() override
            {
                requireAlive();
                return recover(kSupplyAmount);
            }
            std::string typeName() const override { return "英雄"; }
        };

        class Sentry : public Robot
        {
        public:
            static const int kInitialHp = 120;
            static const int kSupplyAmount = 10;

            explicit Sentry(const RobotInfo &info) : Robot(info, kInitialHp) {}

            int supply() override
            {
                requireAlive();
                return recover(kSupplyAmount);
            }
            std::string typeName() const override { return "哨兵"; }
        };

        class AerialRobot : public Robot
        {
        public:
            static const int kInitialHp = 80;

            explicit AerialRobot(const RobotInfo &info) : Robot(info, kInitialHp) {}

            int supply() override
            {
                requireAlive();
                throw std::logic_error("空中机器人不支持地面补给");
            }
            std::string typeName() const override { return "空中机器人"; }
        };

        // 类型标签
        enum class RobotKind
        {
            Infantry,
            Hero,
            Sentry,
            Aerial
        };

        std::unique_ptr<Robot> makeRobot(RobotKind kind, const RobotInfo &info);

        // 裁判系统

        class RefereeSystem
        {
        public:
            // 登记并接管所有权：空指针 / 名称为空 / 编号非正 / 编号重复 -> invalid_argument
            void registerRobot(std::unique_ptr<Robot> robot);

            // 按编号查找；找不到返回 nullptr。返回的指针仅供访问，所有权仍在裁判系统
            Robot *findRobot(int id);
            const Robot *findRobot(int id) const;

            // 处理一条事件：返回实际扣除或恢复量，成功的计数 +1
            // 机器人不存在 / 已退场 -> logic_error；伤害值非法 / 未知事件 -> invalid_argument
            int processEvent(const MatchEvent &event);

            std::size_t successfulEvents() const { return successfulEvents_; }

            void printRobot(int id) const; // 打印指定编号的一行状态
            void printStatus() const;      // 按登记顺序打印全部状态

        private:
            void printRobotLine(const Robot &robot) const;

            std::vector<std::unique_ptr<Robot>> robots_;
            std::size_t successfulEvents_ = 0;
        };

        // 阵营枚举转中文显示名
        std::string teamName(Team team);

    } // namespace judge
} // namespace rm

#endif