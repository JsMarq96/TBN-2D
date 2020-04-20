#include "moving.h"

void Movement::calculate_gravity() {
    // Gravity
    if (is_jumping) {
        int twen_val = jumpspeed.step(1);

        obj_speed.y = twen_val * -1;
        if (jumpspeed.progress() == 1) {
            jumpspeed.seek(0);
            is_jumping = false;
        }
    }
    obj_speed.y  += weight;
}

void Movement::try_to_start_jump() {
    if (in_stairs) {
        obj_speed.y = -1 * speed;
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
    } else {
        obj_speed.y = clamp(obj_speed.y, speed * -1, 0);
    }
}

Vector2 Movement::get_speed() {
    return obj_speed;
}

void Movement::clean() {
    in_groud = false;
    in_stairs = false;

}