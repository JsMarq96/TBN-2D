#ifndef _SCENECONTROLLER_H_
#define _SCENECONTROLLER_H_

#include <vector>
#include "scene.h"

class SceneController {
private:
    std::vector<Scene*> scene_list;
    int curr_scene = 0;

public:

    void add_scene(Scene *new_scene);
    void go_to_next_scene();
    void go_to_prev_scene() {go_to_scene(curr_scene - 1);};
    void go_to_scene(int index);
};

#endif