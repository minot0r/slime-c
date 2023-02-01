#include "../../include/objects/game/ball.h"

ball_t* create_ball(rect_collide_t* ground_collider, rect_collide_t* world_collider, rect_collide_t* net_collider) {
    ball_t* ball = malloc(sizeof(ball_t));
    // net is sized 10*35
    ball->position = (vector2_t) { 0, 0 };
    ball->center = (vector2_t) { 10, 10 };
    ball->width = 20;
    ball->height = 20;
    ball->on_ground = false;
    ball->velocity = (vector2_t) { 0, 0 };
    ball->acceleration = (vector2_t) { 0, 0 };
    ball->ground_collider = ground_collider;
    ball->world_collider = world_collider;
    ball->net_collider = net_collider;
    return ball;
}
void destroy_ball(ball_t* ball) {
    free(ball);
}

void update_ball(ball_t* ball, float delta_time){
    ball->velocity.y += 2700 * delta_time;
    
    //max velocity x is 700
    if(ball->velocity.x > 700){
        ball->velocity.x = 700;
    } else if(ball->velocity.x < -700){
        ball->velocity.x = -700;
    }

    ball->position.x += ball->velocity.x * delta_time;
    ball->center.x = ball->position.x + ball->width / 2;

    collide_dir_e collide_dir;
    if((collide_dir = is_colliding_x(ball->world_collider, ball->position, ball->width, ball->height)) != NONE) {
        if(collide_dir == LEFT && ball->velocity.x < 0) {
            ball->velocity.x = -ball->velocity.x;
        } else if(collide_dir == RIGHT && ball->velocity.x > 0) {
            ball->velocity.x = -ball->velocity.x;
        }
    } else if((collide_dir = is_colliding_x(ball->net_collider, (vector2_t) { ball->position.x + ball->width / 4, ball->position.y + ball->height / 4 },
     ball->width / 2, ball->height / 2)) != NONE) {
        if(collide_dir == LEFT) {
            printf("collision with left\n");
        } else if(collide_dir == RIGHT) {
            printf("collision with right\n");
        }
        //ball->velocity.y = (collide_dir == TOP ? -ball->velocity.y : ball->velocity.y);
        ball->position.x -= ball->velocity.x * delta_time;
        ball->center.x = ball->position.x + ball->width / 2;
        ball->velocity.x = (collide_dir == RIGHT || collide_dir == LEFT ? -ball->velocity.x : ball->velocity.x);
    }

    ball->position.y += ball->velocity.y * delta_time;
    ball->center.y = ball->position.y + ball->height / 2;

    // check collision with ground
    if(is_colliding_y(ball->ground_collider, ball->position, ball->width, ball->height) != NONE) {
        ball->velocity.y = -700;
        ball->position.y = ball->ground_collider->position.y - ball->height;
        ball->center.y = ball->position.y + ball->height / 2;
    } else if((collide_dir = is_colliding_y(ball->net_collider, (vector2_t) { ball->position.x + ball->width / 4, ball->position.y + ball->height / 4 },
     ball->width / 2, ball->height / 2)) != NONE) {
        if(collide_dir == TOP) {
            printf("collision with net top\n");
            ball->position.y = ball->net_collider->position.y - ball->height - 10;
            ball->center.y = ball->position.y + ball->height / 2;
            ball->velocity.y = -ball->velocity.y;
        }
    }
}

void render_ball(ball_t* ball, engine_renderer_t* renderer) {
    SDL_SetRenderDrawColor(renderer->r_w, 255, 0, 0, 255);

    int radius = ball->width / 2;

    int center_x = ball->center.x;
    int center_y = ball->center.y;

    int offset_x, offset_y, d;

    offset_x = 0;
    offset_y = radius;
    d = radius - 1;

    while (offset_y >= offset_x) {
        SDL_RenderDrawLine(
            renderer->r_w,
            (center_x - offset_x) * renderer->scale,
            (center_y + offset_y) * renderer->scale,
            (center_x + offset_x) * renderer->scale,
            (center_y + offset_y) * renderer->scale
        );
        SDL_RenderDrawLine(
            renderer->r_w,
            (center_x - offset_x) * renderer->scale,
            (center_y - offset_y) * renderer->scale,
            (center_x + offset_x) * renderer->scale,
            (center_y - offset_y) * renderer->scale
        );
        SDL_RenderDrawLine(
            renderer->r_w,
            (center_x - offset_y) * renderer->scale,
            (center_y - offset_x) * renderer->scale,
            (center_x + offset_y) * renderer->scale,
            (center_y - offset_x) * renderer->scale
        );
        SDL_RenderDrawLine(
            renderer->r_w,
            (center_x - offset_y) * renderer->scale,
            (center_y + offset_x) * renderer->scale,
            (center_x + offset_y) * renderer->scale,
            (center_y + offset_x) * renderer->scale
        );

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
    
/*     SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    // draw rect for position
    SDL_Rect rect = { ball->position.x - 1, ball->position.y - 1, 2, 2 };
    // draw rect for center
    SDL_Rect rect2 = { ball->center.x - 1, ball->center.y - 1, 2, 2 };
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect2);

    SDL_Rect hitbox = { ball->position.x + ball->width / 4, ball->position.y + ball->height / 4,
     ball->width / 2, ball->height / 2 };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &hitbox); */

}

void ball_set_position(ball_t* ball, vector2_t position) {
    ball->position = position;
}

void apply_force_to_ball(ball_t* ball, vector2_t* force) {
    ball->velocity.x = force->x;
    ball->velocity.y = force->y;
    free(force);
}

vector2_t* get_force_to_apply_ball_sphere(ball_t* ball, vector2_t center_sphere, float radius_sphere) {
    vector2_t* force = malloc(sizeof(vector2_t));

    printf("ball center: (%f, %f)\n, sphere center: (%f, %f)\n", ball->center.x, ball->center.y, center_sphere.x, center_sphere.y);
    
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

    printf("%f\n", (sphere_to_ball.y - ball_to_sphere.y));

    vector2_t resultant = (vector2_t) { sphere_to_ball.x - ball_to_sphere.x, sphere_to_ball.y - ball_to_sphere.y };

    // further away along x axis from center, the angle is more horizontal
    resultant.x *= 1 - (fabs(ball->center.x - center_sphere.x) / radius_sphere);
    resultant.x *= 2.5;

    force->x = (sphere_to_ball.x - ball_to_sphere.x) * 22;
    force->y = (sphere_to_ball.y - ball_to_sphere.y) * 18;

    return force;    
}

/* float max(float a, float b) {
    return a > b ? a : b;
}

float min(float a, float b) {
    return a < b ? a : b;
} */