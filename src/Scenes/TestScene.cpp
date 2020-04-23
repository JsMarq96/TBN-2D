#include "TestScene.h"

// ------- Scene Events
void TestScene::init() {
    level_tileset = *ImageManager::getImage("data/tileset.tga");
    loadScene(0);
    area_img = *ImageManager::getImage("data/circle.tga");
    camera_speed = Vector2(0,0);

    moving_objs.push_back(char_id);

    col_man.add_collision_event([=](int a, int b) {
            this->block_character_collision(a, b);
        }, "player", "block");

    col_man.add_collision_event([=](int a, int b) {
            this->block_character_collision(a, b);
        }, "npc", "block");

    col_man.add_collision_event([=](int a, int b) {
            this->area_npc_collision(a, b);
        }, "area", "npc");

    // Collisions with stairs
    col_man.add_collision_event([=](int a, int b) {
            this->character_stairs_collision(a, b);
        }, "player", "stairs");

    col_man.add_collision_event([=](int a, int b) {
            this->character_stairs_collision(a, b);
        }, "npc", "stairs");

    col_man.add_collision_event([=](int a, int b) {
            this->character_death_collision(a, b);
        }, "npc", "death");

    col_man.add_collision_event([=](int a, int b) {
            this->character_death_collision(a, b);
        }, "player", "death");
}

void TestScene::update(double seconds_elapsed, double time) {
    // Calculate gravity and configure each kinetic object for the collision
    // detection
    for (auto it = active_PCs.begin(); it != active_PCs.end(); it++) {
        int obj_id = it->first;
        

        if (obj_id != char_id)
            active_PCs[obj_id].active = false;

        active_PCs[obj_id].pc_mov.calculate_gravity();

        // Unconfig Misc game flags for collisions
        active_PCs[obj_id].pc_mov.is_in_ground(false);
        active_PCs[obj_id].pc_mov.is_in_stairs(false);
    }

    std::stack<Collision> cols = collision_detection();

    // Execute collision events before calculating the movements
    while (!cols.empty()) {
        Collision col = cols.top();
        cols.pop();
        col.event(col.id_1, col.id_2);
    }

    // Apply the results of the collision and gravity calculations to the
    // objects
    for (auto it = active_PCs.begin(); it != active_PCs.end(); it++) {
        int obj_id = it->first;

        Vector2 obj_speed = active_PCs[obj_id].pc_mov.get_speed();

        objects_in_scene[obj_id].position.x += seconds_elapsed * obj_speed.x;
        objects_in_scene[obj_id].position.y += seconds_elapsed * obj_speed.y;
        
        if (obj_id == char_id){
            // Make the area trail the user in its center
            objects_in_scene[area_id].position.x = objects_in_scene[char_id].position.x - 19;
            objects_in_scene[area_id].position.y = objects_in_scene[char_id].position.y - 19;
            
            // Make the camera trail the user
            Vector2 tmp;
            tmp.x = objects_in_scene[char_id].position.x - (camera.w/2) - camera.x;
            tmp.y = objects_in_scene[char_id].position.y - (camera.h/2) - camera.y;

            camera.x += tmp.x * seconds_elapsed * 1.5;
            camera.y += tmp.y * seconds_elapsed * 1.5;

            //camera.x = objects_in_scene[char_id].position.x - (camera.w/2);
            //camera.y = objects_in_scene[char_id].position.y - (camera.h/2);
        }
                
        // Character animation
        int orientation = active_PCs[obj_id].pc_mov.get_orientation();

        if (obj_speed.x == 0 && obj_speed.y == 0) {
            // Standing
            if (orientation > 0) {
                objects_in_scene[obj_id].image.set_indexes(0,0);
            } else {
                objects_in_scene[obj_id].image.set_indexes(15,0);
            }
            
        } else  if (obj_speed.y == 0) {
            // Walking cicle
            if (orientation > 0) {
                objects_in_scene[obj_id].image.set_indexes((((int) (time * 10)) % 6) + 1,0);
            } else {
                objects_in_scene[obj_id].image.set_indexes((((int) (time * 10)) % 6) + 8,0);
            }
            
        } else {
            // Jumping
            if (orientation > 0) {
                objects_in_scene[obj_id].image.set_indexes(0,4);
            } else {
                objects_in_scene[obj_id].image.set_indexes(1,4);
            }
            
        }
    }
}

void TestScene::button_press_events(int buttons_pressed) {
    Vector2 directions;

    directions.y = 0;

    if (isPressed(LEFT, buttons_pressed)) {
        directions.x = -1;
    } else if (isPressed(RIGHT, buttons_pressed)) {
        directions.x = 1;
    } else {
        directions.x = 0;
    }

    // Apply the movement for each active obejct
    for (auto it = active_PCs.begin(); it != active_PCs.end(); it++) { 
        int obj_id = it->first;

        // If the object is not active, we stop comanding them
        if (!active_PCs[obj_id].active) {
            active_PCs[obj_id].pc_mov.move_to_direction(0, 0);
            continue;
        }

        active_PCs[obj_id].pc_mov.move_to_direction(directions.x, directions.y);

        if (isPressed(UP, buttons_pressed)) {
            active_PCs[obj_id].pc_mov.try_to_start_jump();
        }
    }
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
            PC_behaviour new_beh;
            int id;

            switch(element) {
                // Ground blocks
                case GROUND_BLOCK:
                                          // Position within the tile map
					new_spr = Sprite(Area(16 * 5, 16 * 5, 16,16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "block");
                    block_num++;
                    break;
                case LEFT_GROUND_BLOCK:
					new_spr = Sprite(Area(16 * 4, 5 * 16, 16, 16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "block");
                    block_num++;
                    break;
                case RIGHT_GROUND_BLOCK:
					new_spr = Sprite(Area(16 * 6, 5 * 16, 16, 16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "block");
                    block_num++;
                    break;

                // Goal
                case GOAL_BASE_LEFT:
					new_spr = Sprite(Area(16 * 2, 5 * 16, 16, 16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "goal");
                    //block_num++;
                    break;
                case GOAL_BASE_RIGHT:
					new_spr = Sprite(Area(16 * 3, 5 * 16, 16, 16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "goal");
                    //block_num++;
                    break;
                case GOAL_TOP_RIGHT:
					new_spr = Sprite(Area(16 * 3, 6 * 16, 16, 16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "goal");
                    //block_num++;
                    break;
                case GOAL_TOP_LEFT:
					new_spr = Sprite(Area(16 * 2, 6 * 16, 16, 16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, true, "goal");
                    //block_num++;
                    break;
                case PLAYER:
					new_spr = Sprite(Area(0,16,16,16));// 
                    char_id = add_obj_to_scene(new_spr, x_coord, y_coord, Area(4, 0, 8, 16), true, "player");
					new_spr = Sprite(Area(0,0,0,0));
					area_id = add_obj_to_scene(new_spr, x_coord, y_coord, 24, false, "area");
                    
                    // Character behebiuor movement
                    new_beh.active = true;
                    new_beh.pc_mov.config(GRAVITY, SPEED, JUMP_SPEED, JUMP_DURATION);
                    new_beh.type = PC_PLAYER;

                    active_PCs[char_id] = new_beh;
                    moving_objs.push_back(area_id);
                    std::cout << " + Added main character at " << x * CELL_SIZE << "," << y * CELL_SIZE  << " with id "<< char_id << std::endl;
                    break;
                case STAIRS:
                    new_spr = Sprite(Area(16 * 4, 0,16,16));
                    add_obj_to_scene(new_spr, x_coord, y_coord, Area(3, 0, 8, 16), true, "stairs");
                    stairs_num++;
                    break;
                case NPC1:
                    new_spr = Sprite(Area(0, 16*2 ,16,16));//
                    id = add_obj_to_scene(new_spr, x_coord, y_coord, Area(4, 0, 8, 16) ,true, "npc");
                    
                    // Add movement behabiours
                    new_beh.active = false;
                    new_beh.pc_mov.config(GRAVITY, 90, 190, JUMP_DURATION);
                    new_beh.type = PC_SMALL;

                    active_PCs[id] = new_beh;

                    moving_objs.push_back(id);
                    std::cout << " + Added NPC1 at " << x * CELL_SIZE << "," << y * CELL_SIZE  << " with id "<< char_id << std::endl;
                    break;
                /*case GOAL:
                    GameScene::add_obj_to_scene(tmp, x * CELL_SIZE, y * CELL_SIZE, true, "goal");
                    break;*/
                case DEATH_BLOCK:
                    add_obj_to_scene(new_spr, x_coord, y_coord, 16, false, "death");
                    break;
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
    Image *background = ImageManager::getImage("data/back.tga");
    framebuffer->drawImage(*background,0,0);
    Scene::render(framebuffer);

    // Render the player influence..? area
    Vector2 area_pos = objects_in_scene[area_id].position;

    // Relative to the camera
    area_pos.x -= camera.x, area_pos.y -= camera.y;
	
	framebuffer->drawImage(area_img, area_pos.x, area_pos.y, 0, 0, 48, 48);
}

std::stack<Collision> TestScene::collision_detection() {
	std::stack<Collision> call_stack = std::stack<Collision>();
    // By passing the moving object we reduce the complexity from O(n^2) to O(n)
	col_man.calculate_collisions_to(objects_in_scene, obj_labels, moving_objs, last_inserted, &call_stack);
	return call_stack;
}

// ----------- COLIDER EVENTS

// TODO: Rework event to improve clarity
void TestScene::block_character_collision(int player, int block) {
    // Get the normalized direction vector between the center of the player,
    // to the center of the block

    Area a_player = objects_in_scene[player].get_collision_rect_area(), a_block = objects_in_scene[block].get_collision_rect_area();
	Vector2 direction = direction_between_areas(a_player, a_block);
    
    // Add the obstacles
    active_PCs[player].pc_mov.add_obstacle(direction);

    Vector2 diff = distance_between_area_center(objects_in_scene[player].get_collision_rect_area(), objects_in_scene[block].get_collision_rect_area());

    // Add x distance to avoid clipping the colider areas
    if (direction.x != 0 && direction.y == 0) {
        if (direction.x == 1) {
            objects_in_scene[player].position.x += (a_block.x + a_block.w) - a_player.x;
        } else {
            objects_in_scene[player].position.x -= (a_player.x + a_player.w) - a_block.x;
        }
    }

}

void TestScene::character_stairs_collision(int player, int block) {
    active_PCs[player].pc_mov.is_in_stairs(true);
}

void TestScene::area_npc_collision(int player, int npc) {
    active_PCs[npc].active = true;
}

void TestScene::character_death_collision(int player, int death_block) {
    if (char_id == player) {
        // DEATH
    }

    // Play sound of char death
    // Remove the NPC for the active player list
    active_PCs.erase(player);
    for (auto it = moving_objs.begin(); it != moving_objs.end(); it++) {
        if (*it == player) {
            moving_objs.erase(it);
            break;
        }
        
    }

    object_enabled[player] = false;
}

// Clean the scene
void TestScene::close() {
    Scene::close();

    // Remove the active character map
    for (auto it = active_PCs.begin(); it != active_PCs.end(); it++) {
        active_PCs.erase(it->first);
        moving_objs.pop_back();
    }
}