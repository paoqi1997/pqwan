#ifndef HELPER_H
#define HELPER_H

#include <SDL3/SDL.h>

#include "map.h"
#include "singleton.h"

namespace Color
{
    constexpr SDL_Color SpaceColor = {150, 150, 150, 255};
    constexpr SDL_Color  WallColor = {70, 70, 100, 255};
    constexpr SDL_Color  LineColor = {30, 30, 40, 255};
    constexpr SDL_Color        Red = {187, 36, 56, 255};
    constexpr SDL_Color      Green = {50, 180, 80, 255};
    constexpr SDL_Color       Blue = {50, 120, 200, 180};
}

class SDLHelper : public Singleton<SDLHelper>
{
public:
    int init(const char *title = "Hello SDL", int width = 800, int height = 600);
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    void attachMap(Map* map);
    void show(int fps = 60);
private:
    ~SDLHelper();
    void pickColor(const SDL_Color& color);
    void draw();
    void drawSpaces();
    void drawWalls();
    void drawLines();
    void drawObject(Object* object, const SDL_Color& color);
    void drawObjects();
    void drawObjectView(Object* object, const SDL_Color& color);
    void drawObjectViews();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    Map* map;
};

#endif // HELPER_H
