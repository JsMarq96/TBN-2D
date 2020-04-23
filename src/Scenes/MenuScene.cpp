#include "MenuScene.h"

void MenuScene::button_press_events(int pressed_contrls) {
    if (pressed_contrls != 0) {
        if (pressed_contrls != prev_keys) {
            index++;
            prev_keys = 6;
        }
    }

    prev_keys = pressed_contrls;

    if (index > 4) {
        *(curr_level_index) = *curr_level_index + 1;
        std::cout << index << std::endl;
    }

}

void MenuScene::render(Image *framebuffer) {
    Image *back_img = ImageManager::getImage(data[index]);

    framebuffer->drawImage(*back_img, 0, 0);
}