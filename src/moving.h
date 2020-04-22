#ifndef _MOVING_H_
#define _MOVING_H_

#include "tweeny.h"
#include "framework.h"

enum directions : int {
    D_TOP = 0,
    D_TOP_RIGHT = 1,
    D_RIGHT = 2,
    D_BOTTOM_RIGHT = 3,
    D_BOTTOM = 4,
    D_BOTTOM_LEFT = 5,
    D_LEFT = 6,
    D_TOP_LEFT = 7
};

class Movement {
private:
    bool in_stairs = false;
    bool in_groud = false;
    bool is_jumping = false;

    int weight;
    int speed;
    int jump_speed;
    int jump_duration;

    tweeny::tween<int> jumpspeed;

    Vector2 obj_speed;

    int x_orientation;

    bool collision_orientations[8] = {false};

public:
    Movement() {};
    Movement(int w, int s, int js, int jd) {
        weight = w, speed = s, jump_speed = js, jump_duration = js;
        jumpspeed = tweeny::from(jump_speed).to(0).during(jump_duration).via(tweeny::easing::bounceOut);
    };

    void config(int w, int s, int js, int jd) {
        weight = w, speed = s, jump_speed = js, jump_duration = js;
        jumpspeed = tweeny::from(jump_speed).to(0).during(jump_duration).via(tweeny::easing::bounceOut);
    };

    // Setters
    void is_in_stairs(bool is) {in_stairs = is; if (is) {jumpspeed.seek(0); is_jumping = false;}};
    void is_in_ground(bool is) {in_groud = is; if (is) {jumpspeed.seek(0); is_jumping = false;}};
    void try_to_start_jump();
    bool ground() {return in_groud;}

    Vector2 get_direction();

    void calculate_gravity();

    void move_to(int dx, int dy);
    void move_to_direction(int x, int y);
    void set_accleration(int x, int y);

    void restrict_move_on_x_axis(bool left);
    void restrict_move_on_y_axis(bool up);

    void block_movement_at(int dx, int dy);

    Vector2 get_speed();

    int get_orientation() { return x_orientation;}

    void clean();

    void add_obstacle(Vector2 position);
};

#endif