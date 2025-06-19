#ifndef OBJECT_H
#define OBJECT_H

#include <utility>

struct Coordinate
{
    Coordinate(float x, float y) : x(x), y(y) {}
    float x;
    float y;
};

// 获取给定坐标的视野范围的左下角和右上角坐标
std::pair<Coordinate, Coordinate> getBottomLeftAndTopRightCoordOfObjectView(float x, float y, int viewRange);

class Object
{
public:
    enum class Type {
        PLAYER,     // 玩家
        NPC,        // NPC
        LIGHTHOUSE, // 灯塔
        NONE,
    };
    Object(float x, float y, int viewRange = 0) : x(x), y(y), type(Type::NONE), viewRange(viewRange) {}
    virtual ~Object() {}
    Type getType() const { return type; }
    int getViewRange() const { return viewRange; }
    // 获取其视野范围的左下角和右上角坐标
    std::pair<Coordinate, Coordinate> getBottomLeftAndTopRightCoordOfView();
public:
    float x;
    float y;
protected:
    Type type;
    int viewRange;
};

const char* getObjectTypeString(Object::Type objectType);

class Player : public Object
{
public:
    Player(float x, float y, int viewRange = 3) : Object(x, y, viewRange) { type = Type::PLAYER; }
    ~Player() override {}
};

class NPC : public Object
{
public:
    NPC(float x, float y) : Object(x, y) { type = Type::NPC; }
    ~NPC() override {}
};

#endif // OBJECT_H
