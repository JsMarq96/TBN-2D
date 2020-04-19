#include "TestScene.h"

// ------- Scene Events
void TestScene::init() {
    loadScene(0);
    area_img = *ImageManager::getImage("data/circle.tga");
    area_img.scale(48, 48);
    char_speed = Vector2(0,0);
    camera_speed = Vector2(0,0);

    moving_objs.push_back(char_id);

    col_man.add_collision_event([=](int a, int b) {
            this->block_character_collision(a, b);
        }, "player", "block");

    col_man.add_collision_event([=](int a, int b) {
            this->area_npc_collision(a, b);
        }, "area", "npc1");

    col_man.add_collision_event([=](int a, int b) {
            this->character_stairs_collision(a, b);
        }, "player", "stairs");
}

void TestScene::update(double seconds_elapsed) {
    // Unconfig Misc game flags for collisions
    in_stairs = false;
    std::stack<Collision> cols = collision_detection();

    // Execute collision events before calculating the movements
    while (!cols.empty()) {
        Collision col = cols.top();
        cols.pop();
        col.event(col.id_1, col.id_2);
    }

    //Update the character position
    objects_in_scene[char_id].position.x += seconds_elapsed * char_speed.x;
    objects_in_scene[char_id].position.y += seconds_elapsed * char_speed.y;

    // Make the area trail the user in its center
    objects_in_scene[area_id].position.x = objects_in_scene[char_id].position.x - 16;
    objects_in_scene[area_id].position.y = objects_in_scene[char_id].position.y - 16;
    
    // Make the camera trail the user
    camera.x = objects_in_scene[char_id].position.x - (camera.w/2);
    camera.y = objects_in_scene[char_id].position.y - (camera.h/2);
}

void TestScene::button_press_events(int buttons_pressed) {
    if (isPressed(UP, buttons_pressed) && in_stairs) {
        char_speed.y = -1 * SPEED;
    } else if (isPressed(DOWN, buttons_pressed)) { 
        char_speed.y = SPEED;
    } else {
        char_speed.y = 0;
    }

    if (isPressed(LEFT, buttons_pressed)) {
        char_speed.x = -1 * SPEED;
    } else if (isPressed(RIGHT, buttons_pressed)) {
        char_speed.x = SPEED;
    } else {
        char_speed.x = 0;
    }

    // Gravity
    char_speed.y  += 50;
}

void TestScene::loadScene(int index) {
    Map *map = (Map::loadMap(levels[index]));

	std::cout << " + Loading Map " << levels[index] << std::endl;
    int block_num = 0, stairs_num = 0;
    for (int x = 0; x < map->width; x++) {
        for (int y = 0; y < map->height; y++) {
            unsigned char element = map->cells[ x + (y *map->width)];
            if (element == 0) {
                continue;
            }

			int x_coord = x * CELL_SIZE;
			int y_coord = y * CELL_SIZE;
            Sprite new_spr;
            //std::cout << t1.height << "-" << t1.width << std::endl;            
            switch(element) {
                case GROUND_BLOCK:
					new_spr = Sprite(Area(16 * 2,0,16,16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "block");
                    block_num++;
                    break;
                case PLAYER:
					new_spr = Sprite(Area(0,16,16,16));
                    char_id = add_obj_to_scene(new_spr, x_coord, y_coord, true, "player");
					new_spr = Sprite(Area(0, 16 * 2, 32, 32));
					area_id = add_obj_to_scene(new_spr, x_coord, y_coord, 16, false, "area");
                    
                    moving_objs.push_back(area_id);
                    std::cout << " + Added main character at " << x * CELL_SIZE << "," << y * CELL_SIZE  << " with id "<< char_id << std::endl;
                    break;
                case STAIRS:
                    new_spr = Sprite(Area(16 * 4, 0,16,16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "stairs");
                    stairs_num++;
                    break;
                case NPC1:
                    new_spr = Sprite(Area(0, 16*2 ,16,16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "npc1");
                    std::cout << " + Added NPC1 at " << x * CELL_SIZE << "," << y * CELL_SIZE  << " with id "<< char_id << std::endl;
                    break;
                /*case GOAL:
                    GameScene::add_obj_to_scene(tmp, x * CELL_SIZE, y * CELL_SIZE, true, "goal");
                    break;*/
                case CAMERA:
                    camera.x = x_coord;
                    camera.y = y_coord;
                    std::cout << " + Added camera position at " << x * CELL_SIZE << "," << y * CELL_SIZE << std::endl;
                    break;
                default: int z;
            }
        }
    }
    std::cout << " + Added " << block_num << " ground blocks" << std::endl;
    std::cout << " + Added " << stairs_num << " stairs" << std::endl;
}


void TestScene::render(Image *framebuffer) {
    Scene::render(framebuffer);

    // Render the player influence..? area
    Vector2 area_pos = objects_in_scene[area_id].position;

    // Relative to the camera
    area_pos.x -= camera.x, area_pos.y -= camera.y;
	//objects_in_scene[area_id].image.clamp_to_camera(camera, user_pos.x, user_pos.y, &postion_camera_space, &in_image_coordinates );
	//std::cout << new_x << "," << new_y  << "," <<  (size.w * index_x) + (size.w - new_w)  << "," <<  (size.h * index_y) + (size.h - new_h) <<std::endl;
	
	framebuffer->drawImage(area_img, area_pos.x, area_pos.y, 0, 0, 48, 48);
}

std::stack<Collision> TestScene::collision_detection() {
	std::stack<Collision> call_stack = std::stack<Collision>();
    // By passing the moving object we reduce the complexity from O(n^2) to O(n)
	col_man.calculate_collisions_to(objects_in_scene, obj_labels, moving_objs, last_inserted, &call_stack);
	return call_stack;
}

// ----------- COLIDER EVENTS
void TestScene::block_character_collision(int player, int block) {
    // Get the normalized direction vector between the center of the player,
    // to the center of the block
	Vector2 direction = direction_between_areas(objects_in_scene[player].get_collision_rect_area(), 
                                                objects_in_scene[block].get_collision_rect_area());
	
    // Since we are only interested in "pure" lateral collisions 
    // (only left, or only right, or only bottom, or only top)
    // We can exclude the other ones (collision with the left and bottom...)
    if (direction.y == 0) {
        // We use the directions to clamp the directional speed of the character
        if (direction.x < 0) {
            char_speed.x = clamp(char_speed.x, SPEED * -1, 0);
        } else if (direction.x > 0) {
            char_speed.x = clamp(char_speed.x, 0, SPEED);
        }
    }
	
	if (direction.x == 0) {
        if (direction.y < 0) {
            char_speed.y = clamp(char_speed.y, SPEED * -1, 0);
        } else if (direction.y > 0) {
            char_speed.y = clamp(char_speed.y, 0, SPEED);
        }
    }
}

void TestScene::character_stairs_collision(int player, int block) {
    //std::cout << "Starisss"<< std::endl;
    in_stairs = true;
}

void TestScene::area_npc_collision(int player, int npc) {
    //std::cout << "In area!"<< std::endl;
}