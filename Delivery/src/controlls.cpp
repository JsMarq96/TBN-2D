#include "controlls.h"

bool isPressed(ButtonTypes button, int key_state) {
    return (key_state & button) == button;
}