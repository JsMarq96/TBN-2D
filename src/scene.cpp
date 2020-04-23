#include "scene.h"

// GENERIC SCENE METHODS ---------------
Scene::Scene() {
	last_inserted = 0;
}

// Scene customization
int Scene::add_obj_to_scene(Sprite &image, int x, int y, bool active) {
	if (last_inserted >= MAX_ELEMENTS_PER_SCENE) {
		return -1;
	}
	// Sotore image
	objects_in_scene[last_inserted].image = image;
    objects_in_scene[last_inserted].position = Vector2(x, y);
	object_enabled[last_inserted] = active;

	// Return Index
	return last_inserted++;
}

void Scene::update_obj_position(int index, int new_x, int new_y) {
	objects_in_scene[index].position.x = new_x;
	objects_in_scene[index].position.y = new_y;
}

void Scene::activate_obj(int index, bool new_state) {
	object_enabled[index] = new_state;
}

// Game engine events
void Scene::render(Image *framebuffer) {
    int i;
	for (i = 0; i < last_inserted; i++) {
		if (object_enabled[i]) {
			
			render_sprite(framebuffer, camera, objects_in_scene[i].position.x, objects_in_scene[i].position.y, objects_in_scene[i].image);
		}
	}
}

void Scene::render_sprite(Image *framebuffer, Area &camera, int x, int y, Sprite &spr) {
	// Check if is in display
	if (!spr.is_in_camera(camera, x, y)) {
		return;
	}
	Area postion_camera_space;
	Vector2 in_image_coordinates;
	spr.clamp_to_camera(camera, x, y,&postion_camera_space, &in_image_coordinates );
	
	framebuffer->drawImage(level_tileset,
							postion_camera_space.x, postion_camera_space.y,
							in_image_coordinates.x, in_image_coordinates.y,
							postion_camera_space.w, postion_camera_space.h);
}

// Add object with a default collider area (16x16) witha  tag
int Scene::add_obj_to_scene(Sprite &spr, int x, int y, bool active, std::string labels) {
    // Call parent method
    int index = add_obj_to_scene(spr, x, y, active);

    Area size = objects_in_scene[index].image.getSize();

    obj_labels[index].labels[0] = labels;
    objects_in_scene[index].col_rect_area = Area(0, 0, size.w, size.h);
    objects_in_scene[index].type = RECT;

    return index;
}

// Add object with custom rect collision area
int Scene::add_obj_to_scene(Sprite &spr, int x, int y, Area col_area, bool active, std::string labels) {
    // Call parent method
    int index = add_obj_to_scene(spr, x, y, active);

    obj_labels[index].labels[0] = labels;
    objects_in_scene[index].col_rect_area = col_area;
    objects_in_scene[index].type = RECT;

    return index;
}

// Add object to scene with circular collision area
int Scene::add_obj_to_scene(Sprite &spr, int x, int y, int radius, bool active, std::string labels) {
    // Call parent method
    int index = add_obj_to_scene(spr, x, y, active);

    obj_labels[index].labels[0] = labels;
    objects_in_scene[index].radius = radius;
    objects_in_scene[index].type  = CIRCLE;

    return index;
}

std::stack<Collision> Scene::collision_detection() {
	std::stack<Collision> call_stack = std::stack<Collision>();
	col_man.calculate_collisions(objects_in_scene, obj_labels, last_inserted, &call_stack);
	return call_stack;
}

void Scene::close() {
	last_inserted = 0;
	camera = Area(0,0, 160, 120);
}

