#include "MenuScene.h"

void MenuScene::button_press_events( int pressed_contrls) {
    if (pressed_contrls != 0) {
        index++;
    }

    if (index >= 5) {
        *curr_level_index++;
    }

}

void MenuScene::render(Image *framebuffer) {
    Image *back_img = ImageManager::getImage(data[index]);

    framebuffer->drawImage(*back_img, 0, 0);
}