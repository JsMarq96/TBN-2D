#ifndef _MOVING_H_
#define _MOVING_H_

#include "tweeny.h"
#include "framework.h"

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

public:
    Movement() {};
    Movement(int w, int s, int js, int jd) {
        weight = w, speed = s, jump_speed = js, jump_duration = js;
        jumpspeed = tweeny::from(jump_speed).to(0).during(jump_duration).via(tweeny::easing::circularInOut);
    };

    void config(int w, int s, int js, int jd) {
        weight = w, speed = s, jump_speed = js, jump_duration = js;
        jumpspeed = tweeny::from(jump_speed).to(0).during(jump_duration).via(tweeny::easing::circularInOut);
    };

    // Setters
    void is_in_stairs(bool is) {in_stairs = is;};
    void is_in_ground(bool is) {in_groud = is;};
    void try_to_start_jump();

    

    void calculate_gravity();

    void move_to(int dx, int dy);
    void move_to_direction(int x, int y);
    void set_accleration(int x, int y);

    void restrict_move_on_x_axis(bool left);
    void restrict_move_on_y_axis(bool up);

    Vector2 get_speed();

    void clean();
};

#endif