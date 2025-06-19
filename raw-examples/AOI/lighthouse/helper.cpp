#include <chrono>
#include <cstdio>
#include <thread>

#include "helper.h"

SDLHelper::~SDLHelper()
{
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

int SDLHelper::init(const char *title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::printf("error: SDL_Init(), msg: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::printf("error: SDL_CreateWindow(), msg: %s\n", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    screenWidth = width;
    screenHeight = height;

    renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::printf("error: SDL_CreateRenderer(), msg: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 3;
    }

    return 0;
}

void SDLHelper::attachMap(Map* map)
{
    this->map = map;
}

void SDLHelper::pickColor(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void SDLHelper::draw()
{
    drawSpaces();
    drawWalls();
    drawObjects();
    drawLines();
    drawObjectViews();
}

void SDLHelper::drawSpaces()
{
    pickColor(Color::SpaceColor);
    SDL_RenderClear(renderer);
}

void SDLHelper::drawWalls()
{
    pickColor(Color::WallColor);

    SDL_FRect tileRect;

    tileRect.x = 0;
    tileRect.y = 0;
    tileRect.w = screenWidth;
    tileRect.h = map->TileHeight;

    // 上
    SDL_RenderFillRect(renderer, &tileRect);

    tileRect.y = (map->rows - 1) * map->TileHeight;

    // 下
    SDL_RenderFillRect(renderer, &tileRect);

    tileRect.x = 0;
    tileRect.y = 0;
    tileRect.w = map->TileWidth;
    tileRect.h = screenHeight;

    // 左
    SDL_RenderFillRect(renderer, &tileRect);

    tileRect.x = (map->cols - 1) * map->TileWidth;

    // 右
    SDL_RenderFillRect(renderer, &tileRect);
}

void SDLHelper::drawLines()
{
    pickColor(Color::LineColor);

    // 绘制垂直方向的线
    for (float x = 0; x <= screenWidth; x += map->TileWidth) {
        SDL_RenderLine(renderer, x, 0, x, screenHeight);
    }

    // 绘制水平方向的线
    for (float y = 0; y <= screenHeight; y += map->TileHeight) {
        SDL_RenderLine(renderer, 0, y, screenWidth, y);
    }
}

void SDLHelper::drawObject(Object* object, const SDL_Color& color)
{
    if (object == nullptr) {
        return;
    }

    pickColor(color);

    float x = map->Game2SDL_X(static_cast<int>(object->x));
    float y = map->Game2SDL_Y(static_cast<int>(object->y));

    SDL_FRect tileRect;
    tileRect.x = x;
    tileRect.y = y - map->TileHeight;
    tileRect.w = map->TileWidth;
    tileRect.h = map->TileHeight;

    SDL_RenderFillRect(renderer, &tileRect);
}

void SDLHelper::drawObjects()
{
    std::vector<Object*> lighthouses = map->getLighthouses();

    for (auto& lighthouse : lighthouses) {
        drawObject(lighthouse, Color::Blue);
    }

    Object* player = map->getPlayer();

    drawObject(player, Color::Red);

    std::vector<Object*> NPCs = map->getNPCs();

    for (auto& npc : NPCs) {
        drawObject(npc, Color::Green);
    }
}

void SDLHelper::drawObjectView(Object* object, const SDL_Color& color)
{
    if (object == nullptr) {
        return;
    }

    pickColor(color);

    auto coords = object->getBottomLeftAndTopRightCoordOfView();
    auto bottomLeftCoord = coords.first;
    auto topRightCoord = coords.second;

    float x1 = map->Game2SDL_X(bottomLeftCoord.x);
    float y1 = map->Game2SDL_Y(bottomLeftCoord.y);

    float x2 = map->Game2SDL_X(topRightCoord.x);
    float y2 = map->Game2SDL_Y(topRightCoord.y);

    // 上
    SDL_RenderLine(renderer, x1, y2, x2, y2);
    // 下
    SDL_RenderLine(renderer, x1, y1, x2, y1);
    // 左
    SDL_RenderLine(renderer, x1, y1, x1, y2);
    // 右
    SDL_RenderLine(renderer, x2, y1, x2, y2);
}

void SDLHelper::drawObjectViews()
{
    std::vector<Object*> lighthouses = map->getLighthouses();

    for (auto& lighthouse : lighthouses) {
        drawObjectView(lighthouse, Color::Blue);
    }

    Object* player = map->getPlayer();

    drawObjectView(player, Color::Red);
}

void SDLHelper::show(int fps)
{
    int ms = 1000 / fps;

    float SDL_x, SDL_y;

    bool running = true;

    SDL_Event ev;

    while (running) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) {
                running = false;
            }
            // 窗口大小变化时触发
            else if (ev.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_GetWindowSize(window, &screenWidth, &screenHeight);
                map->onScreenChanged(screenWidth, screenHeight);
            }
            else if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                SDL_GetMouseState(&SDL_x, &SDL_y);
                if (map->canPlayerMoveTo(SDL_x, SDL_y)) {
                    map->onUpdatePlayer(SDL_x, SDL_y);
                }
            }
        }

        draw();

        SDL_RenderPresent(renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}
