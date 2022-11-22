#include "../../include/objects/game/player_state.h"

#include <stdlib.h>

void create_player_state(player_state_t* player_state, bool serving) {
    player_state->points = 0;
    player_state->serving = serving;
}