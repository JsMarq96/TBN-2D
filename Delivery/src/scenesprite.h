#ifndef _SCENESPRITE_H_
#define _SCENESPRITE_H_

#include "sprite.h"

enum Area_Type : uint8 {
    RECT,
    CIRCLE
};

struct SceneSprite {
    Sprite image;
    Vector2 position;

    Area_Type type;
    // If its a square area
    Area col_rect_area;
    // If the area is a circle
    int radius;
    bool is_overlaping(SceneSprite &a1);
    Area get_collision_rect_area();
    Vector2 get_circle_center();
};

#endif