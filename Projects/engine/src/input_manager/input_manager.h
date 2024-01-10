#pragma once
#include "src/types/multi2.h"
#include "stdbool.h"

typedef struct input_manager {
    vector2 mouse_position;
    bool mouse_down;
    bool right_mouse_down;

    bool mouse_clicked;
} input_manager;

void set_mouse_pos(input_manager* input, vector2 pos);