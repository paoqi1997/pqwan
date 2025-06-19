#ifndef MAP_H
#define MAP_H

#include <unordered_set>
#include <vector>

#include "object.h"

class Map
{
public:
    enum class TileType {
        SPACE,      // 空地
        WALL,       // 墙
        PLAYER,     // 玩家
        NPC,        // NPC
        LIGHTHOUSE, // 灯塔
    };
    Map(int rows, int cols);
    void onScreenChanged(int screenWidth, int screenHeight);
    float SDL2Game_X(float SDL_x);
    float SDL2Game_Y(float SDL_y);
    float Game2SDL_X(float game_x);
    float Game2SDL_Y(float game_y);
    void addObject(Object* object);
    Object* getPlayer();
    std::vector<Object*> getNPCs();
    std::vector<Object*> getLighthouses();
    void onUpdatePlayer(float SDL_x, float SDL_y);
    TileType getTileTypeByCoord(float SDL_x, float SDL_y);
    bool canPlayerMoveTo(float SDL_x, float SDL_y);
public:
    int rows;
    int cols;
    float TileWidth;
    float TileHeight;
private:
    std::unordered_set<Object*> objects;
};

#endif // MAP_H
