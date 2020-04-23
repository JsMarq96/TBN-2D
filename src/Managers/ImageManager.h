#ifndef _IMAGEMANAGER_H_
#define _IMAGEMANAGER_H_

#include <unordered_map>
#include "image.h"

class ImageManager {
private:
    static std::unordered_map<std::string, Image*> cache;
 public:
    static Image* getImage(std::string img_name);
    static void clear_memmory();
};

#endif