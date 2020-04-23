#ifndef _GAMEENDSCENE_H_
#define _GAMEENDSCENE_H_

#include <string>
#include "scene.h"
#include "image.h"
#include "Managers/ImageManager.h"

class GameEndScene: Scene {
private:
    int *curr_level_index;
    int score = 0;
public:
    GameEndScene(int *game_scene_pointer) { curr_level_index = game_scene_pointer; Scene(); };
    void init();
    void loadScene(int index) {};
    void update(double seconds_elapsed, double time) {};
    void close() {};
    void button_press_events( int pressed_contrls) {};
    void render(Image *framebuffer); 
};

#endif