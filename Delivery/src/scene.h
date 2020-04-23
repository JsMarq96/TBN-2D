#ifndef _SCENE_H_
#define _SCENE_H_

#include <unordered_map>
#include <functional>
#include <string>
#include "sprite.h"
#include "collisions.h"
#include "controlls.h"
#include "map.h"
#include "Managers/ImageManager.h"
#include "scenesprite.h"

typedef std::unordered_map<std::string, int> map_scene_ids;

/**
 * Game Scene Live cicle:
 *  1) Init_event
 *  2) (if necessary) button_press_event
 *  3) Collision calculation
 *  4) update
 *  5) Render
 *  6) go to 2
 *  7) delete_event
*/
/**
 * TODO: 
 *  Overload del metodo de cambiar las coordenadas de un objeto
 * la clase Scene y GameScene
*/

class Scene {
    /**
     * The Scene class
    */
protected:
    // Scene Elements
    std::array<SceneSprite, MAX_ELEMENTS_PER_SCENE> objects_in_scene;
    std::array<bool, MAX_ELEMENTS_PER_SCENE> object_enabled;
    std::array<Labels, MAX_ELEMENTS_PER_SCENE> obj_labels;

    CollisionsManager col_man;

    int last_inserted;
    // Scene config
    Area camera = Area(0,0, 160, 120);

    Image level_tileset;
    
    int *curr_level_index;
    
public:
    map_scene_ids in_scene_elements;
    // Scene events
    virtual void close();
    virtual void update(double seconds_elapsed, double time) {};
    virtual void button_press_events(int pressed_controls) {};
    virtual void init() {};

    Scene();
    Scene(int *level_index) { curr_level_index = level_index; Scene();};

    void loadScene(const char * map_dir);
    
    int add_obj_to_scene(Sprite &spr, int x, int y, bool active);
    int add_obj_to_scene(Sprite &spr, int x, int y, bool active, std::string labels);
    int add_obj_to_scene(Sprite &spr, int x, int y, Area col_area, bool active, std::string labels);
    int add_obj_to_scene(Sprite &spr, int x, int y, int radius, bool active, std::string labels);
    virtual std::stack<Collision> collision_detection();


    virtual void update_obj_position(int index, int new_x, int new_y);
    virtual void activate_obj(int index, bool new_state);
    virtual void render(Image *framebuffer);
    void render_sprite(Image *framebuffer, Area &camera, int x, int y, Sprite &spr);
};

#endif