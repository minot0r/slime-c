#ifndef COLLIDER_H
#define COLLIDER_H

#include <stdbool.h>

#include "vector2.h"

bool sphere_colliding(vector2_t center_1, float radius_1, vector2_t center_2, float radius_2);
bool rect_colliding_inside_rect(vector2_t pos, int width, int height, vector2_t pos_2, int width_2, int height_2);
bool rect_colliding_outside_rect(vector2_t pos, int width, int height, vector2_t pos_2, int width_2, int height_2);

#endif