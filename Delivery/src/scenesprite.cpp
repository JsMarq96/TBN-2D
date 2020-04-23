#include "scenesprite.h"


// --- Position overlapping
bool point_to_circle_collision(Vector2 point, Vector2 circule_center, int radius) {
    float dist = point.distance(circule_center);
    return dist <= radius;
}

/**
 * NOTE: The collision system with circles is not perfect
 * if the distance between two corners is bigger that the radius, 
 * and it does not intersect with any corner, if will fail to detect
 * the collision
 * But since this is a fringe case, and a this simpler (and faster..?) solution
 * I decided to keep it
*/
Area SceneSprite::get_collision_rect_area() {
	return Area(position.x + col_rect_area.x , position.y + col_rect_area.y, col_rect_area.w, col_rect_area.h);
}

Vector2 SceneSprite::get_circle_center() {
    Vector2 res = position;
    res.x += radius, res.y += radius;

    return res;
}

// TODO: refactor??
bool SceneSprite::is_overlaping(SceneSprite &a1) {
    if (a1.type == CIRCLE) {
		Area rect_area = get_collision_rect_area();
        Vector2 circle_center = a1.get_circle_center();
        // Calculate the distance from each corner of the box, to the cirlce
        return point_to_circle_collision(Vector2(rect_area.x, rect_area.y), circle_center, a1.radius) ||
                point_to_circle_collision(Vector2(rect_area.x + rect_area.w, rect_area.y), circle_center, a1.radius) ||
                point_to_circle_collision(Vector2(rect_area.x + rect_area.w, rect_area.y + rect_area.h), circle_center, a1.radius) ||
                point_to_circle_collision(Vector2(rect_area.x, rect_area.y + rect_area.h), circle_center, a1.radius);
    } else if (type == CIRCLE) {
		Area rect_area = a1.get_collision_rect_area();
        Vector2 circle_center = get_circle_center();
        // If the circle is the current object
        return point_to_circle_collision(Vector2(rect_area.x, rect_area.y), circle_center, radius) ||
                point_to_circle_collision(Vector2(rect_area.x + rect_area.w, rect_area.y), circle_center, radius) ||
                point_to_circle_collision(Vector2(rect_area.x + rect_area.w, rect_area.y + rect_area.h), circle_center, radius) ||
                point_to_circle_collision(Vector2(rect_area.x, rect_area.y + rect_area.h), circle_center, radius);
    }
    // If the circle is nether
	Area rect_area = get_collision_rect_area();
	Area a1_rect_area = a1.get_collision_rect_area();
    
    if (partially_inside(rect_area, a1_rect_area) || partially_inside(a1_rect_area, rect_area)) {
        int i = 0;
    }

	return (partially_inside(rect_area, a1_rect_area) || partially_inside(a1_rect_area, rect_area));
}
