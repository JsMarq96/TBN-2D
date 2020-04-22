#ifndef _TESTSCENE_H_
#define _TESTSCENE_H_

#include <unordered_map>
#include "scene.h"
#include "Managers/ImageManager.h"
#include "map.h"
#include "moving.h"
#include "tweeny.h"

#define SPEED 80
#define JUMP_SPEED 180
#define JUMP_DURATION 3
#define GRAVITY 100

enum PC_Types {
    PC_PLAYER,
    PC_BIG,
    PC_SMALL
};

struct PC_behaviour {
    PC_Types type;
    bool active;
    Movement pc_mov;
};

class TestScene : Scene {
private:
    Vector2 camera_speed;

    int char_id;
    int area_id;

    const char* levels[1] = {"data/level1.map"};
    int curr_level = 0;

    std::vector<int> moving_objs;
    std::unordered_map<int, PC_behaviour> active_PCs;

    Image area_img;
public:
    TestScene(int *game_scene_pointer) { curr_level_index = game_scene_pointer; Scene(); }

    void init();
    void loadScene(int index);
    void update(double seconds_elapsed, double time);
    void close();
    void button_press_events( int pressed_contrls);
    void render(Image *framebuffer); //{ Scene::render(framebuffer);}
    std::stack<Collision> collision_detection();

    // COLLISION EVENTS
    void block_character_collision(int char_id, int block_id);
    void character_stairs_collision(int player, int block);
    void area_npc_collision(int player, int npc);
    void character_death_collision(int player, int death_block);
};

#endif