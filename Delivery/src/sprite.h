#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "utils.h"
#include "image.h"

#include <algorithm>

class Sprite {
private:
    // The area contains the tileset coordinates
    Area size;
    int index_x = 0;
    int index_y = 0;
public:
    Sprite();
    Sprite(Area size);
    //Sprite(Image *newImg, Area tile_size);
    // Paints the image in the framebuffer
    

    void set_indexes(int x, int y) {index_x = x, index_y = y;};

    bool is_in_camera(Area &camera, int posX, int posY);
    void clamp_to_camera(Area &camera, int posX, int posY, Area *result, Vector2 *image_coordinates);

    Area getSize() { return size;}
};

#endif