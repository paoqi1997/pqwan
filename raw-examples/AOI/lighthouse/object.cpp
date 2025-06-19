#include "object.h"

std::pair<Coordinate, Coordinate> getBottomLeftAndTopRightCoordOfObjectView(float x, float y, int viewRange)
{
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    Coordinate bottomLeftCoord(ix - viewRange, iy - viewRange);
    Coordinate topRightCoord(ix + 1 + viewRange, iy + 1 + viewRange);

    return std::make_pair(bottomLeftCoord, topRightCoord);
}

std::pair<Coordinate, Coordinate> Object::getBottomLeftAndTopRightCoordOfView()
{
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    Coordinate bottomLeftCoord(ix - viewRange, iy - viewRange);
    Coordinate topRightCoord(ix + 1 + viewRange, iy + 1 + viewRange);

    return std::make_pair(bottomLeftCoord, topRightCoord);
}

const char* getObjectTypeString(Object::Type objectType)
{
    switch (objectType) {
    case Object::Type::PLAYER:
        return "player";
    case Object::Type::NPC:
        return "npc";
    default:
        return "";
    }
}
