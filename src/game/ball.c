#include "../../include/objects/game/ball.h"
#include "../../include/objects/game/texture.h"

ball_t* create_ball(SDL_Texture* texture, rect_collide_t* ground_collider, rect_collide_t* world_collider, rect_collide_t* net_collider) {
    ball_t* ball = malloc(sizeof(ball_t));
    // net is sized 10*35
    ball->position = (vector2_t) { 0, 100 };
    ball->center = (vector2_t) { BALL_RADIUS, BALL_RADIUS};
    ball->width = BALL_RADIUS * 2;
    ball->height = BALL_RADIUS * 2;
    ball->on_ground = false;
    ball->velocity = (vector2_t) { 0, 0 };
    ball->acceleration = (vector2_t) { 0, 0 };
    ball->ground_collider = ground_collider;
    ball->world_collider = world_collider;
    ball->net_collider = net_collider;
    ball->texture = texture;
    return ball;
}
void destroy_ball(ball_t* ball) {
    free(ball);
}

void update_ball(ball_t* ball, float delta_time, slime_t* slime1, slime_t* slime2){    
    ball->position.x += ball->velocity.x * delta_time;
    ball->center.x = ball->position.x + ball->width / 2;

    ball->position.y += ball->velocity.y * delta_time;
    ball->center.y = ball->position.y + ball->height / 2;

    ball->velocity.y += 0.5 * delta_time;
    
    check_collision(ball, slime1);
    check_collision(ball, slime2);

    if(ball->velocity.y > 11){
        ball->velocity.y = 11;
    } else if(ball->velocity.y < -11){
        ball->velocity.y = -11;
    }


    collide_dir_e collide_dir;
    if((collide_dir = is_colliding_x(ball->world_collider, ball->position, ball->width, ball->height)) != NONE) {
        if(collide_dir == LEFT && ball->velocity.x < 0) {
            ball->velocity.x = -ball->velocity.x;
        } else if(collide_dir == RIGHT && ball->velocity.x > 0) {
            ball->velocity.x = -ball->velocity.x;
        }
    } else if((collide_dir = is_colliding_x(ball->net_collider, (vector2_t) { ball->position.x + ball->width / 4, ball->position.y + ball->height / 4 },
     ball->width / 2, ball->height / 2)) != NONE) {
        /* if(collide_dir == LEFT) {
            printf("collision with left\n");
        } else if(collide_dir == RIGHT) {
            printf("collision with right\n");
        } */
        //ball->velocity.y = (collide_dir == TOP ? -ball->velocity.y : ball->velocity.y);
        ball->position.x -= ball->velocity.x * delta_time;
        ball->center.x = ball->position.x + ball->width / 2;
        ball->velocity.x = (collide_dir == RIGHT || collide_dir == LEFT ? -ball->velocity.x : ball->velocity.x);
    }

    // check collision with ground
    if(is_colliding_y(ball->ground_collider, ball->position, ball->width, ball->height) != NONE) {
        ball->velocity.y = -11;
        ball->position.y = ball->ground_collider->position.y - ball->height;
        ball->center.y = ball->position.y + ball->height / 2;
    } else if((collide_dir = is_colliding_y(ball->net_collider, (vector2_t) { ball->position.x + ball->width / 4, ball->position.y + ball->height / 4 },
     ball->width / 2, ball->height / 2)) != NONE) {
        if(collide_dir == TOP) {
            /* printf("collision with net top\n"); */
            ball->position.y = ball->net_collider->position.y - ball->height - 10;
            ball->center.y = ball->position.y + ball->height / 2;
            ball->velocity.y = -ball->velocity.y;
        }
    }
}

void render_ball(ball_t* ball, engine_renderer_t* renderer) {
    /* SDL_SetRenderDrawColor(renderer->r_w, 255, 0, 0, 255);

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
    } */

    SDL_RenderCopyEx(renderer->r_w, ball->texture, NULL, 
        &(SDL_Rect) { ball->position.x * renderer->scale, ball->position.y * renderer->scale, ball->width * renderer->scale, ball->height * renderer->scale },
        0, NULL, SDL_FLIP_NONE);
    
   /*  SDL_SetRenderDrawColor(renderer->r_w, 255, 255, 0, 255);
    // draw rect for position
    SDL_Rect rect = { ball->position.x - 1, ball->position.y - 1, 2, 2 };
    // draw rect for center
    SDL_Rect rect2 = { ball->center.x - 1, ball->center.y - 1, 2, 2 };
    SDL_RenderFillRect(renderer->r_w, &rect);
    SDL_RenderFillRect(renderer->r_w, &rect2);

    SDL_Rect hitbox = { ball->position.x + ball->width / 4, ball->position.y + ball->height / 4,
     ball->width / 2, ball->height / 2 };
    SDL_SetRenderDrawColor(renderer->r_w, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer->r_w, &hitbox); */

}

void ball_set_position(ball_t* ball, vector2_t position) {
    ball->position = position;
    ball->center.x = ball->position.x + ball->width / 2;
    ball->center.y = ball->position.y + ball->height / 2;
    ball->velocity = (vector2_t) { 0, 0 };
}

void check_collision(ball_t* ball, slime_t* slime) {
    float MAX_VELOCITY_X = 13.0;
    float MAX_VELOCITY_Y = 10.0;

    // calculate distance between centers
    float dx = ball->center.x - slime->center.x;
    float dy = ball->center.y - slime->center.y;
    float dist = sqrt(dx * dx + dy * dy);
    
    // check if collision
    if(dy < 0 && dist < BALL_RADIUS + SLIME_RADIUS) {
        // set ball postion relative to dist
        ball->center.x = slime->center.x + (SLIME_RADIUS + BALL_RADIUS) * dx / dist;
        ball->center.y = slime->center.y + (SLIME_RADIUS + BALL_RADIUS) * dy / dist;
        ball->position.x = ball->center.x - BALL_RADIUS;
        ball->position.y = ball->center.y - BALL_RADIUS;

        float dVelocityX = ball->velocity.x - slime->velocity.x;
        float dVelocityY = ball->velocity.y - slime->velocity.y;

        // calculate new velocity
        float something = (dx * dVelocityX + dy * dVelocityY) / dist;
        if(something < 0) {
            ball->velocity.x += (slime->velocity.x - 1.92 * something * dx / dist);
            ball->velocity.y += (slime->velocity.y - 1.92 * something * dy / dist);
        }
        
        //limit velocity
        if(ball->velocity.x > MAX_VELOCITY_X) ball->velocity.x = MAX_VELOCITY_X;
        if(ball->velocity.x < -MAX_VELOCITY_X) ball->velocity.x = -MAX_VELOCITY_X;
        if(ball->velocity.y > MAX_VELOCITY_Y) ball->velocity.y = MAX_VELOCITY_Y;
        if(ball->velocity.y < -MAX_VELOCITY_Y) ball->velocity.y = -MAX_VELOCITY_Y;
    }
}

/* float max(float a, float b) {
    return a > b ? a : b;
}

float min(float a, float b) {
    return a < b ? a : b;
} */