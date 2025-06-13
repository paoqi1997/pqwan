#include <chrono>
#include <cstdio>
#include <thread>

#include <SDL3/SDL.h>

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::printf("error: SDL_Init(), msg: %s\n", SDL_GetError());
        return 1;
    }

    int screenWidth = 800;
    int screenHeight = 600;

    SDL_Window* window = SDL_CreateWindow("tiles", screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::printf("error: SDL_CreateWindow(), msg: %s\n", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::printf("error: SDL_CreateRenderer(), msg: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 3;
    }

    constexpr int rows = 15; // 15行，垂直方向上有15个格子
    constexpr int cols = 20; // 20列，水平方向上有20个格子
    constexpr int fps = 60;
    constexpr int ms = 1000 / fps;

    float tileWidth = screenWidth / static_cast<float>(cols);
    float tileHeight = screenHeight / static_cast<float>(rows);

    SDL_Color bgColor{30, 30, 40, 255}; // 背景颜色
    SDL_Color lineColor = bgColor;      // 网格线颜色

    enum class TileType {
        SPACE, // 空地
        WALL,  // 墙
        WATER, // 水
        GRASS, // 草地
    };

    auto includes = [](int x, int x1, int x2) {
        return x >= std::min(x1, x2) && x <= std::max(x1, x2);
    };

    auto isWater = [&](int row, int col) {
        bool waterCond1 = includes(row, 2, 4) && includes(col, 2, 4);
        bool waterCond2 = includes(row, 2, 4) && includes(col, cols - 5, cols - 3);
        bool waterCond3 = includes(row, rows - 5, rows - 3) && includes(col, 2, 4);
        bool waterCond4 = includes(row, rows - 5, rows - 3) && includes(col, cols - 5, cols - 3);
        return waterCond1 || waterCond2 || waterCond3 || waterCond4;
    };

    auto isGrass = [&](int row, int col) {
        bool grassCond1 = includes(row, rows / 2 - 1, rows / 2 + 1) && includes(col, cols / 2 - 2, cols / 2 + 1);
        return grassCond1;
    };

    auto getTileType = [&](int row, int col) {
        if (row == 0 || row == rows - 1 || col == 0 || col == cols - 1) {
            return TileType::WALL;
        }
        if (isWater(row, col)) {
            return TileType::WATER;
        }
        if (isGrass(row, col)) {
            return TileType::GRASS;
        }

        return TileType::SPACE;
    };

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
                tileWidth = screenWidth / static_cast<float>(cols);
                tileHeight = screenHeight / static_cast<float>(rows);
            }
        }

        // 设置要给背景填充的颜色
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        // 清屏
        SDL_RenderClear(renderer);

        // 绘制地图格子
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                SDL_FRect tileRect;
                tileRect.x = col * tileWidth;
                tileRect.y = row * tileHeight;
                tileRect.w = tileWidth;
                tileRect.h = tileHeight;

                TileType tileType = getTileType(row, col);

                switch (tileType) {
                case TileType::SPACE:
                    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                    break;
                case TileType::WALL:
                    SDL_SetRenderDrawColor(renderer, 70, 70, 100, 255);
                    break;
                case TileType::WATER:
                    SDL_SetRenderDrawColor(renderer, 50, 120, 200, 180);
                    break;
                case TileType::GRASS:
                    SDL_SetRenderDrawColor(renderer, 50, 180, 80, 255);
                    break;
                default:
                    break;
                }

                SDL_RenderFillRect(renderer, &tileRect);
            }
        }

        // 设置绘制网格线时用到的颜色
        SDL_SetRenderDrawColor(renderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);

        // 绘制垂直方向的线
        for (float x = 0; x <= screenWidth; x += tileWidth) {
            SDL_RenderLine(renderer, x, 0, x, screenHeight);
        }

        // 绘制水平方向的线
        for (float y = 0; y <= screenHeight; y += tileHeight) {
            SDL_RenderLine(renderer, 0, y, screenWidth, y);
        }

        SDL_RenderPresent(renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
