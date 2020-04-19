#ifndef _TESTSCENE_H_
#define _TESTSCENE_H_

#include "scene.h"
#include "Managers/ImageManager.h"
#include "map.h"
#include "tweeny.h"

#define SPEED 100

class TestScene : Scene {
private:
    Vector2 char_speed;
    Vector2 camera_speed;

    int char_id;
    int area_id;

    const char* levels[1] = {"data/level1.map"};
    int curr_level = 0;

    std::vector<int> moving_objs;

    Image area_img;

    // Misc Game flags
    bool in_stairs; // If the cheracter is in collision with a stair
public:
    TestScene() {}

    void init();
    void loadScene(int index);
    void update(double seconds_elapsed);
    void button_press_events( int pressed_contrls);
    void render(Image *framebuffer); //{ Scene::render(framebuffer);}
    std::stack<Collision> collision_detection();

    // COLLISION EVENTS
    void block_character_collision(int char_id, int block_id);
    void character_stairs_collision(int player, int block);
    void area_npc_collision(int player, int npc);
};

#endif