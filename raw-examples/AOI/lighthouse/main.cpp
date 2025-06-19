#include <iostream>

#include "aoi.h"
#include "helper.h"
#include "map.h"
#include "object.h"

using std::cout;
using std::endl;

int main()
{
    SDLHelper* sdlHelper = SDLHelper::getInstance();

    if (sdlHelper->init("Hello Lighthouse AOI") != 0) {
        cout << "error: SDLHelper::init()" << endl;
        return 1;
    }

    Map map(15, 20);

    map.onScreenChanged(sdlHelper->getScreenWidth(), sdlHelper->getScreenHeight());

    sdlHelper->attachMap(&map);

    Player player(1, 1);

    map.addObject(&player);

    std::vector<NPC> NPCs = {
        {9, 11},
        {17, 12},
        {13, 7},
        {11, 3},
    };

    for (auto& npc : NPCs) {
        map.addObject(&npc);
    }

    std::vector<Lighthouse> lighthouses = {
        {10, 10},
        {15, 10},
        {10, 5},
        {15, 5},
    };

    for (auto& lighthouse : lighthouses) {
        map.addObject(&lighthouse);
    }

    AOI::getInstance()->init(&map);

    sdlHelper->show();

    return 0;
}
