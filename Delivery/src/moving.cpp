#include "moving.h"

Vector2 Movement::get_direction() {
    return obj_speed / speed;
}

void Movement::calculate_gravity() {
    // Gravity
    if (!in_stairs) {
        if (is_jumping) {
            int twen_val = jumpspeed.step(1);

            obj_speed.y = twen_val * -1;
            if (jumpspeed.progress() == 1) {
                jumpspeed.seek(0);
                is_jumping = false;
            }
        }
        obj_speed.y  += weight;
    } else {
        // Slow down fall while in stairs
        obj_speed.y  += weight / 2;
    }

    // Collision array cleanup
    for (int i = 0; i <= 8; i++) {
        collision_orientations[i] = false;
    }
}

void Movement::try_to_start_jump() {
    if (in_stairs) {
        obj_speed.y = -1 * speed;
        is_jumping = true;
    } else if (!is_jumping && in_groud) {
        is_jumping = true;
    }
}
 
void Movement::move_to(int dx, int dy) {
    obj_speed.x += dx;
    obj_speed.y += dy;
}

void Movement::move_to_direction(int x, int y) {
    obj_speed.x = x * speed;
    obj_speed.y = y * speed;
}

void Movement::set_accleration(int x, int y) {
    obj_speed.x = x;
    obj_speed.y = y;
}

void Movement::restrict_move_on_x_axis(bool left) {
    if (left) {
        obj_speed.x = clamp(obj_speed.x, 0, speed);
    } else {
        obj_speed.x = clamp(obj_speed.x, speed * -1, 0);
    }
        
}
void Movement::restrict_move_on_y_axis(bool up) {
    if (up) {
        // Restrict the displacement vector in the upwards movement
        obj_speed.y = clamp(obj_speed.y, 0, speed);
        jumpspeed.seek(0);
        is_jumping = false;
    } else {
        obj_speed.y = clamp(obj_speed.y, speed * -1, 0);
    }
}

Vector2 Movement::get_speed() {
    // Limit movement by collisions
    if (collision_orientations[D_BOTTOM]) {
        //(collision_orientations[D_BOTTOM] && (collision_orientations[D_BOTTOM_LEFT || collision_orientations[D_BOTTOM_RIGHT]]))) {
        obj_speed.y = clamp(obj_speed.y, speed * -1, 0);

        is_in_ground(true);
    } 
    
    if (collision_orientations[D_TOP] || collision_orientations[D_TOP_LEFT] || collision_orientations[D_TOP_RIGHT]) {
        obj_speed.y = clamp(obj_speed.y, 0, speed);
            is_jumping = false;
            jumpspeed.seek(0);
        
    }
    

    if (obj_speed.x != 0)
        x_orientation = obj_speed.x / speed;
    return obj_speed;
}

void Movement::clean() {
    in_groud = false;
    in_stairs = false;

}

void Movement::add_obstacle(Vector2 position) {
    int id = 0;
    if (position.x == 0 && position.y == 0){
        int i = 0;
    }
   if (position.x == 0 && position.y == 1) {
       id = D_TOP;
   } else if (position.x == 1 && position.y == 1) {
        id = D_TOP_LEFT;
   } else if (position.x == 1 && position.y == 0) {
       id = D_LEFT;
   } else if (position.x == 1 && position.y == -1) {
       id = D_BOTTOM_LEFT;
   } else if (position.x == 0 && position.y == -1) {
       id = D_BOTTOM;
   } else if (position.x == -1 && position.y == -1) {
       id = D_BOTTOM_RIGHT;
   } else if (position.x == -1 && position.y == 0) {
       id = D_RIGHT;
   } else if (position.x == -1 && position.y == 1) {
       id = D_TOP_RIGHT;
   }

   collision_orientations[id] = true;
}