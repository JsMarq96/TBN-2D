#include "ImageManager.h"

std::unordered_map<std::string, Image*> ImageManager::cache = std::unordered_map<std::string, Image*>();


Image* ImageManager::getImage(std::string img_name) {
    if (cache.count(img_name)) {
        return cache[img_name];
    }
    
    // If the image does not exist in the cache
    Image *img = new Image();
    img->loadTGA(img_name.c_str());
    //img->flipY();

    cache[img_name] = img;
    return img;
 }

void ImageManager::clear_memmory() {
    for (auto it = cache.begin(); it != cache.end(); it++) {
        cache.erase(it->first);
    }
}