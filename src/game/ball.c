#include "../../include/objects/game/ball.h"

ball_t* create_ball() {
    ball_t* ball = malloc(sizeof(ball_t));
    ball->position = (vector2_t) { 320 - (20 / 2), 0 };
    ball->center = (vector2_t) { 0, 0 };
    ball->width = 20;
    ball->height = 20;
    ball->on_ground = false;
    ball->velocity = (vector2_t) { 0, 0 };
    ball->acceleration = (vector2_t) { 0, 0 };

    return ball;
}
void destroy_ball(ball_t* ball) {
    free(ball);
}

void update_ball(ball_t* ball, float delta_time){
    ball->velocity.y += 3000 * delta_time;

    ball->position.x += ball->velocity.x * delta_time;
    ball->position.y += ball->velocity.y * delta_time;

    ball->center.x = ball->position.x + ball->width / 2;
    ball->center.y = ball->position.y + ball->height / 2;

    if(ball->center.y > 480) {
        ball->position.y = 480 - ball->height;
        ball->center.y = 480 - ball->height / 2;
    }
}

void render_ball(ball_t* ball, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int radius = ball->width / 2;

    int center_x = ball->center.x;
    int center_y = ball->center.y;

    int offset_x, offset_y, d;

    offset_x = 0;
    offset_y = radius;
    d = radius - 1;

    while (offset_y >= offset_x) {
        SDL_RenderDrawLine(renderer, center_x - offset_y, center_y + offset_x, center_x + offset_y, center_y + offset_x);
        SDL_RenderDrawLine(renderer, center_x - offset_x, center_y + offset_y, center_x + offset_x, center_y + offset_y);
        SDL_RenderDrawLine(renderer, center_x - offset_x, center_y - offset_y, center_x + offset_x, center_y - offset_y);
        SDL_RenderDrawLine(renderer, center_x - offset_y, center_y - offset_x, center_x + offset_y, center_y - offset_x);

        if (d >= 2*offset_x) {
            d -= 2*offset_x + 1;
            offset_x +=1;
        }
        else if (d < 2 * (radius - offset_y)) {
            d += 2 * offset_y - 1;
            offset_y -= 1;
        }
        else {
            d += 2 * (offset_y - offset_x - 1);
            offset_y -= 1;
            offset_x += 1;
        }
    }
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    // draw rect for position
    SDL_Rect rect = { ball->position.x - 1, ball->position.y - 1, 2, 2 };
    // draw rect for center
    SDL_Rect rect2 = { ball->center.x - 1, ball->center.y - 1, 2, 2 };
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect2);
}

void apply_force_to_ball(ball_t* ball, vector2_t* force) {
    ball->velocity.x = force->x;
    ball->velocity.y = force->y;
    free(force);
}

vector2_t* get_force_to_apply_ball_sphere(ball_t* ball, vector2_t center_sphere, float radius_sphere) {
    vector2_t* force = malloc(sizeof(vector2_t));
    
    vector2_t ball_to_sphere = (vector2_t) { center_sphere.x - ball->center.x, center_sphere.y - ball->center.y };
    // normalize ball_to_sphere to ball radius
    float magnitude = sqrt(pow(ball_to_sphere.x, 2) + pow(ball_to_sphere.y, 2));
    ball_to_sphere.x /= magnitude;
    ball_to_sphere.y /= magnitude;
    ball_to_sphere.x *= ball->width / 2;
    ball_to_sphere.y *= ball->height / 2;

    vector2_t sphere_to_ball = (vector2_t) { ball->center.x - center_sphere.x, ball->center.y - center_sphere.y };
    // normalize sphere_to_ball to sphere radius
    magnitude = sqrt(pow(sphere_to_ball.x, 2) + pow(sphere_to_ball.y, 2));
    sphere_to_ball.x /= magnitude;
    sphere_to_ball.y /= magnitude;
    sphere_to_ball.x *= radius_sphere;
    sphere_to_ball.y *= radius_sphere;

    force->x = (sphere_to_ball.x - ball_to_sphere.x) * 20;
    force->y = (sphere_to_ball.y - ball_to_sphere.y) * 20;

    return force;    
}

/* float max(float a, float b) {
    return a > b ? a : b;
}

float min(float a, float b) {
    return a < b ? a : b;
} */