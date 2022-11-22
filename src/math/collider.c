#include <math.h>
#include "../../include/objects/math/collider.h"

bool sphere_colliding(vector2_t center_1, float radius_1, vector2_t center_2, float radius_2) {
    float distance = sqrt(pow(center_1.x - center_2.x, 2) + pow(center_1.y - center_2.y, 2));
    return distance < radius_1 + radius_2;
}

bool rect_colliding_inside_rect(vector2_t pos, int width, int height, vector2_t pos_2, int width_2, int height_2) {
    return pos.x < pos_2.x + width_2 && pos.x + width > pos_2.x && pos.y < pos_2.y + height_2 && pos.y + height > pos_2.y;
}

bool rect_colliding_outside_rect(vector2_t pos, int width, int height, vector2_t pos_2, int width_2, int height_2) {
    return rect_colliding_inside_rect(pos_2, width_2, height_2, pos, width, height);
}