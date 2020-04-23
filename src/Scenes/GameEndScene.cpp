#include "GameEndScene.h"
#include "game.h"

void GameEndScene::init() {  
    score = Game::instance->score; 
}

void GameEndScene::render(Image *framebuffer) {
    Image *back_img;

    if (score > 0) {
        back_img = ImageManager::getImage("data/game_end_good.tga");
    } else {
        back_img = ImageManager::getImage("data/gamend.tga");
    }

    framebuffer->drawImage(*back_img, 0, 0);

    if (score > 0) {
        Image* font = ImageManager::getImage("data/bitmap-font-white.tga");

        framebuffer->drawText(score + "", 75, 61, *font);
    }
}