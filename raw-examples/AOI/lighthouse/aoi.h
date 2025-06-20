#ifndef AOI_H
#define AOI_H

#include <unordered_set>

#include "map.h"
#include "object.h"
#include "singleton.h"

enum class AOIEventType {
    ENTER,  // 有对象进入灯塔的视野范围
    LEAVE,  // 有对象离开灯塔的视野范围
    MOVE,   // 有对象在灯塔的视野范围内移动
    ADD_OB, // 将对象纳入观察者列表，即对象获得灯塔的视野范围
    DEL_OB, // 将对象移出观察者列表，即对象失去灯塔的视野范围
};

const char* getEventString(AOIEventType event);

// 灯塔
class Lighthouse : public Object
{
public:
    Lighthouse(float x, float y, int viewRange = 2) : Object(x, y, viewRange) { type = Type::LIGHTHOUSE; }
    ~Lighthouse() override {}
    bool update(Object* object, float old_x, float old_y, bool init = false);
private:
    // 给定坐标是否在灯塔的视野范围内
    bool includes(float obj_x, float obj_y);
    // 给定坐标的视野范围与灯塔的视野范围是否存在重叠
    bool isOverlapped(float obj_x, float obj_y, int viewRange);
    bool addObject(Object* object);
    bool addObserver(Object* observer);
    void notify(AOIEventType event, Object* object, float old_x, float old_y);
    void notifyOne(AOIEventType event, Object* object, float old_x, float old_y);
    void notifyAll(AOIEventType event, Object* object, float old_x, float old_y);
private:
    std::unordered_set<Object*> objects;   // 在灯塔的视野范围内的对象集合
    std::unordered_set<Object*> observers; // 获得灯塔的视野范围的对象集合
};

class AOI : public Singleton<AOI>
{
public:
    void init(Map* map);
    void update(Object* object, float old_x, float old_y);
private:
    std::unordered_set<Lighthouse*> lighthouses;
};

#endif // AOI_H
