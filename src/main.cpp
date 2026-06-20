#include <iostream>

#include "atlas.h"
#include "camera.h"
#include "animation.h"
#include "resource_manager.h"
#include "game.h"

int main()
{
    Game &game = Game::initstate();
    game.run();
    game.clean();

    return 0;
}
