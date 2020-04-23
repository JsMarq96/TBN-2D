#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include <string>
#include "scene.h"
#include "image.h"
#include "Managers/ImageManager.h"

class MenuScene: Scene {
private:
    int *curr_level_index;
    int index = 0;

    int prev_keys;

    std::string data[5] = { "data/menu/menu_screen.tga", 
                            "data/menu/tut1.tga", 
                            "data/menu/tut2.tga",
                            "data/menu/tut3.tga",
                            "data/menu/tut4.tga"};
public:
    MenuScene(int *game_scene_pointer) { curr_level_index = game_scene_pointer; Scene(); };
    void init() {index = 0; prev_keys = 10;};
    void loadScene(int index) {};
    void update(double seconds_elapsed, double time) {};
    void close() {};
    void button_press_events( int pressed_contrls);
    void render(Image *framebuffer); 
};

#endif