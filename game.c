// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <printf.h>
#include "defines.h"
#include "game.h"

extern char * map, * dot_map;
extern int height;
extern int width;

int check_win(void) {
    //if dot is found in dot map return keep going, if not dot found return win
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            //if dot is found keep going
            if (dot_map[y * width + x] == DOT) {
                return KEEP_GOING;
            }
        }
    }
    return YOU_WIN;
}

int check_loss(int player_y, int player_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]) {
    //for all ghosts check if player is at same position as a ghost
    for (int i = 0; i<NUM_GHOSTS;i++){
        //if player and ghost at same position lose
        if (player_x == ghosts_x[i] && player_y == ghosts_y[i]) {
            return YOU_LOSE;
        }
    }
    return KEEP_GOING;
}

