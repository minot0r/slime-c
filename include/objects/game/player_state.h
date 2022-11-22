#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include <stdbool.h>

typedef struct {
    int points;
    bool serving;
} player_state_t;

void create_player_state(player_state_t* player_state, bool serving);

#endif