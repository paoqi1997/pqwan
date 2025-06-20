#include <cstdio>

#include "aoi.h"
#include "map.h"

Map::Map(int rows, int cols) : rows(rows), cols(cols), TileWidth(0.0f), TileHeight(0.0f)
{

}

void Map::onScreenChanged(int screenWidth, int screenHeight)
{
    TileWidth = screenWidth / static_cast<float>(cols);
    TileHeight = screenHeight / static_cast<float>(rows);
}

float Map::SDL2Game_X(float SDL_x)
{
    return SDL_x / TileWidth;
}

float Map::SDL2Game_Y(float SDL_y)
{
    return rows - (SDL_y / TileHeight);
}

float Map::Game2SDL_X(float game_x)
{
    return game_x * TileWidth;
}

float Map::Game2SDL_Y(float game_y)
{
    return (rows - game_y) * TileHeight;
}

void Map::addObject(Object* object)
{
    objects.emplace(object);
}

Object* Map::getPlayer()
{
    for (auto& object : objects) {
        if (object->getType() == Object::Type::PLAYER) {
            return object;
        }
    }

    return nullptr;
}

std::vector<Object*> Map::getNPCs()
{
    std::vector<Object*> NPCs;

    for (auto& object : objects) {
        if (object->getType() == Object::Type::NPC) {
            NPCs.emplace_back(object);
        }
    }

    return NPCs;
}

std::vector<Object*> Map::getLighthouses()
{
    std::vector<Object*> lighthouses;

    for (auto& object : objects) {
        if (object->getType() == Object::Type::LIGHTHOUSE) {
            lighthouses.emplace_back(object);
        }
    }

    return lighthouses;
}

void Map::onUpdatePlayer(float SDL_x, float SDL_y)
{
    Object* player = getPlayer();
    if (player == nullptr) {
        return;
    }

    float old_x = player->x;
    float old_y = player->y;

    player->x = SDL2Game_X(SDL_x);
    player->y = SDL2Game_Y(SDL_y);

    std::printf("Game_xy: (%.2f, %.2f) SDL_xy: (%.2f, %.2f)\n", player->x, player->y, SDL_x, SDL_y);

    AOI::getInstance()->update(player, old_x, old_y);
}

Map::TileType Map::getTileTypeByCoord(float SDL_x, float SDL_y)
{
    int x = static_cast<int>(SDL2Game_X(SDL_x));
    int y = static_cast<int>(SDL2Game_Y(SDL_y));

    if (x == 0 || x == cols - 1 || y == 0 || y == rows - 1) {
        return TileType::WALL;
    }

    for (auto& object : objects) {
        int obj_x = static_cast<int>(object->x);
        int obj_y = static_cast<int>(object->y);

        if (x == obj_x && y == obj_y) {
            switch (object->getType()) {
            case Object::Type::PLAYER:
                return TileType::PLAYER;
            case Object::Type::NPC:
                return TileType::NPC;
            case Object::Type::LIGHTHOUSE:
                return TileType::LIGHTHOUSE;
            default:
                break;
            }
        }
    }

    return TileType::SPACE;
}

bool Map::canPlayerMoveTo(float SDL_x, float SDL_y)
{
    TileType tileType = getTileTypeByCoord(SDL_x, SDL_y);
    return tileType != TileType::WALL && tileType != TileType::NPC;
}
