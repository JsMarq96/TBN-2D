#include "sprite.h"

Sprite::Sprite() {
	
}

Sprite::Sprite( Area tileSize) {
	size = tileSize;
}


bool Sprite::is_in_camera(Area& camera, int posX, int posY) {
	Area position_spr = Area(posX, posY, size.w, size.h);
	return (partially_inside(camera, position_spr) || partially_inside(position_spr, camera));
}

void Sprite::clamp_to_camera(Area &camera, int posX, int posY, Area *result, Vector2 *image_coordinates) {
	int in_camera_x = posX - camera.x;
	int in_camera_y = (posY - camera.y);

	Area position_cam = Area(in_camera_x, in_camera_y, size.w, size.h);
	int margin_x = 0, margin_y = 0;

	// Crop the area if if it overflows the camera
	if ((in_camera_x + position_cam.w) > camera.w) {
		// Oveflows fro the les  of the screen
		position_cam.w -= (in_camera_x + position_cam.w) - camera.w;
	} else if (in_camera_x < 0) {
		position_cam.x = 0;
		position_cam.w += in_camera_x;
		margin_x = in_camera_x * -1;
	}

	if ((in_camera_y + position_cam.h) > camera.h) {
		position_cam.h -= (position_cam.y + position_cam.h) - camera.h; 
	} else if (in_camera_y < 0) {
		position_cam.h += in_camera_y;
		margin_y = in_camera_y * -1;
	}

	position_cam.x = MAX(0, in_camera_x);
	position_cam.y = MAX(0, in_camera_y);

	result->x = position_cam.x;
	result->y = position_cam.y;
	result->w = position_cam.w;
	result->h = position_cam.h;
	
	image_coordinates->x = (size.w * (index_x) ) + margin_x + size.x;
	image_coordinates->y = (size.h * (index_y) ) + margin_y + size.y;
}