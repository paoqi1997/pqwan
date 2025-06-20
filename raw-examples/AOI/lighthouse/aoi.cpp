#include "aoi.h"

const char* getEventString(AOIEventType event)
{
    switch (event) {
    case AOIEventType::ENTER:
        return "ENTER";
    case AOIEventType::LEAVE:
        return "LEAVE";
    case AOIEventType::MOVE:
        return "MOVE";
    case AOIEventType::ADD_OB:
        return "ADD_OB";
    case AOIEventType::DEL_OB:
        return "DEL_OB";
    default:
        return "";
    }
}

bool Lighthouse::includes(float obj_x, float obj_y)
{
    auto coords = getBottomLeftAndTopRightCoordOfView();
    auto bottomLeftCoord = coords.first;
    auto topRightCoord = coords.second;

    int obj_ix = static_cast<int>(obj_x);
    int obj_iy = static_cast<int>(obj_y);

    bool ok1 = obj_ix >= bottomLeftCoord.x && obj_iy >= bottomLeftCoord.y;
    bool ok2 = obj_ix < topRightCoord.x && obj_iy < topRightCoord.y;

    return ok1 && ok2;
}

bool Lighthouse::isOverlapped(float obj_x, float obj_y, int viewRange)
{
    auto coords = getBottomLeftAndTopRightCoordOfView();
    auto bottomLeftCoord = coords.first;
    auto topRightCoord = coords.second;

    auto objCoords = getBottomLeftAndTopRightCoordOfObjectView(obj_x, obj_y, viewRange);
    auto objBottomLeftCoord = objCoords.first;
    auto objTopRightCoord = objCoords.second;

    bool x_overlapped = std::max(bottomLeftCoord.x, objBottomLeftCoord.x) < std::min(topRightCoord.x, objTopRightCoord.x);
    bool y_overlapped = std::max(bottomLeftCoord.y, objBottomLeftCoord.y) < std::min(topRightCoord.y, objTopRightCoord.y);

    return x_overlapped && y_overlapped;
}

void Lighthouse::notify(AOIEventType event, Object* object, float old_x, float old_y)
{
    switch (event) {
    case AOIEventType::ENTER:
    case AOIEventType::LEAVE:
    case AOIEventType::MOVE:
        notifyAll(event, object, old_x, old_y);
        break;
    case AOIEventType::ADD_OB:
    case AOIEventType::DEL_OB:
        notifyOne(event, object, old_x, old_y);
        break;
    default:
        break;
    }
}

void Lighthouse::notifyOne(AOIEventType event, Object* object, float old_x, float old_y)
{
    const char* eventS = getEventString(event);
    const char* objectTypeS = getObjectTypeString(object->getType());

    std::printf(
        "event: %s lighthouse: (%.2f, %.2f) %s: (%.2f, %.2f) -> (%.2f, %.2f)\n",
        eventS, x, y, objectTypeS, old_x, old_y, object->x, object->y
    );

    if (event == AOIEventType::ADD_OB) {
        for (auto& object : objects) {
            const char* inObjectTypeS = getObjectTypeString(object->getType());
            std::printf(
                "%s: (%.2f, %.2f) IN lighthouse: (%.2f, %.2f)\n",
                inObjectTypeS, object->x, object->y, x, y
            );
        }
    }
}

void Lighthouse::notifyAll(AOIEventType event, Object* object, float old_x, float old_y)
{
    const char* eventS = getEventString(event);
    const char* fromObjectTypeS = getObjectTypeString(object->getType());

    for (auto& observer : observers) {
        const char* toObjectTypeS = getObjectTypeString(observer->getType());

        std::printf(
            "event: %s lighthouse: (%.2f, %.2f) to(%s): (%.2f, %.2f) from(%s): (%.2f, %.2f) -> (%.2f, %.2f)\n",
            eventS, x, y,
            toObjectTypeS, observer->x, observer->y,
            fromObjectTypeS, old_x, old_y, object->x, object->y
        );
    }
}

bool Lighthouse::update(Object* object, float old_x, float old_y, bool init)
{
    int now_ix = static_cast<int>(object->x);
    int now_iy = static_cast<int>(object->y);

    int old_ix = static_cast<int>(old_x);
    int old_iy = static_cast<int>(old_y);

    bool is_not_moved = now_ix == old_ix && now_iy == old_iy;

    if (!init && is_not_moved) {
        return false;
    }

    bool is_overlapped_old = isOverlapped(old_x, old_y, object->getViewRange());
    bool is_overlapped_now = isOverlapped(object->x, object->y, object->getViewRange());

    bool is_add_ob = !is_overlapped_old && is_overlapped_now;
    bool is_del_ob = is_overlapped_old && !is_overlapped_now;

    if (is_add_ob) {
        addObserver(object);
        notifyOne(AOIEventType::ADD_OB, object, old_x, old_y);
    }

    bool includes_old = includes(old_x, old_y);
    bool includes_now = includes(object->x, object->y);

    bool is_enter = !includes_old && includes_now;
    bool is_leave = includes_old && !includes_now;
    bool is_move = includes_old && includes_now;

    if (is_enter) {
        addObject(object);
        notifyAll(AOIEventType::ENTER, object, old_x, old_y);
    } else if (is_leave) {
        objects.erase(object);
        notifyAll(AOIEventType::LEAVE, object, old_x, old_y);
    } else if (is_move) {
        notifyAll(AOIEventType::MOVE, object, old_x, old_y);
    }

    if (is_del_ob) {
        observers.erase(object);
        notifyOne(AOIEventType::DEL_OB, object, old_x, old_y);
    }

    return true;
}

bool Lighthouse::addObject(Object* object)
{
    if (objects.find(object) == objects.end()) {
        objects.emplace(object);
        return true;
    }

    return false;
}

bool Lighthouse::addObserver(Object* object)
{
    if (observers.find(object) == observers.end()) {
        observers.emplace(object);
        return true;
    }

    return false;
}

void AOI::init(Map* map)
{
    std::vector<Object*> mapLighthouses = map->getLighthouses();

    for (auto& lighthouse : mapLighthouses) {
        lighthouses.emplace(dynamic_cast<Lighthouse*>(lighthouse));
    }

    std::vector<Object*> NPCs = map->getNPCs();

    for (auto& npc : NPCs) {
        for (auto& lighthouse : lighthouses) {
            lighthouse->update(npc, 0, 0, true);
        }
    }

    Object* player = map->getPlayer();

    for (auto& lighthouse : lighthouses) {
        lighthouse->update(player, 0, 0, true);
    }
}

void AOI::update(Object* object, float old_x, float old_y)
{
    // 如果各个灯塔按照一定的规律分布的话，就为玩家快速找到周围的灯塔提供了条件
    // 这里因为灯塔用无序集合存储，并且为了简化处理，直接走遍历的处理方式
    for (auto& lighthouse : lighthouses) {
        lighthouse->update(object, old_x, old_y);
    }
}
