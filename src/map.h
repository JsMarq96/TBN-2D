#ifndef _MAP_H_
#define _MAP_H_

#include <stdio.h>
#include <assert.h>
#include <string>

#define CELL_SIZE 16

struct sMapHeader {
    int w;
    int h;
    unsigned char bytes;
    unsigned char extra[7];
};

enum MapElements : unsigned char {
    GROUND_BLOCK = 2,
    STAIRS = 4,
    PLAYER = 16,
    GOAL = 1,
    NPC1 = 32,
    NPC2 = 21,
    CAMERA = 3
};

class Map { 
public:
    int width;
    int height;
    unsigned char* cells;
    Map();
    Map(int n_width, int n_height);

    ~Map();
    
    static Map* loadMap(const char* filename);
};

#endif