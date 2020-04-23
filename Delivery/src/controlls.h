#ifndef _CONTROLLS_H_
#define _CONTROLLS_H_

enum ButtonTypes : int {
    UP = 0b0001,
    DOWN = 0b0010,
    LEFT = 0b0100,
    RIGHT = 0b1000,
    A = 0b10000,
    B = 0b100000
};

bool isPressed(ButtonTypes button, int key_state);

#endif
