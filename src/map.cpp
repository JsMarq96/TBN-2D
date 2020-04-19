#include "map.h"

Map::Map(int n_width, int n_height) {
        width = n_width;
        height = n_height;

        cells = new unsigned char[width*height];
    }

Map::~Map() {
        delete[] cells;
    }

 Map* Map::loadMap(const char* filename) {
        FILE* file = fopen(filename,"rb");
        if (file == NULL) //file not found
            return NULL;

        sMapHeader header; //read header and store it in the struct
        fread( &header, sizeof(sMapHeader), 1, file);

        Map* map = new Map(header.w, header.h);

        assert(header.bytes == 1); //always control bad cases!!
        //allocate memory for the cells data and read it
        
        fread( map->cells, header.bytes, header.w*header.h, file);
        fclose(file); //always close open files
        //create the map where we will store it
        

        /*for(int x = 0; x < CELL_SIZE; x++)
            for(int y = 0; y < CELL_SIZE; y++)
                map.getCell(x,y).type = (eCellType)cells[x+y*CELL_SIZE];*/

         //always free any memory allocated!
        
        return map;

    }