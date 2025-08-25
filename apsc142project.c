
// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

// don't forget to update your project configuration to select "Emulate terminal in the output console"

// Make sure to include all relevant libraries
#include <stdio.h>
#include <stdlib.h>

// colours.h contains functions to change text colour and read single characters without requiring an enter
#include "colours.h"
// defines.h contains useful definitions to keep your code readable
#include "defines.h"
// map.h, game.h, and ghost.h contain prototypes of functions you must implement
#include "map.h"
#include "game.h"
#include "actor.h"

// These global variables must be used to store map information.
// Almost every function needs these variables, so keeping them as globals helps keep things organized.
// map is a pointer to a dynamically allocated map for displaying to the user
// dot_map is a pointer to a dynamically allocated map for keeping track of what dots are left

// width and height store the width and height of map, NOT counting outer walls


/**
 * This is the hardcoded map used for labs 1-3.
 * Once you implement load_map in lab 4 you should remove this map, as it cannot be used for the
 * final version of the project.
 * The map's dimensions are 9x9
 */
char *map = NULL, *dot_map = NULL;
// width and height store the width and height of map, NOT counting outer walls
int width, height;
#define ERR_MEMORY 4

// You are NOT allowed to add more global variables!
// The globals that are included in the starter code provide a convenient way to share information
// between the many functions that all need access to the same data, but all other data must be
// passed using function arguments.  If you add new globals, the autograder tests will fail to run
// and you will not receive a good mark.

/**
 * Main entry point into your program.
 * Make sure that main returns appropriate status codes depending on what
 * happens.  The codes you must use are:
 *   NO_ERROR when no error occurs
 *   ERR_NO_MAP when no map file is found
 *   ERR_NO_PLAYER when no player is found on the map
 *   ERR_NO_GHOSTS when fewer than 2 ghosts are found on the map
 *
 * Make sure that any allocated memory is freed before returning.
 * @return a status code
 */
void update_ghost(int player_y, int player_x, int * ghost_y, int * ghost_x);

int main(void) {
    // This ensures that printf writes to the console immediately, instead of buffering.
    // If you remove this, you will not be able to rely on printf to work if your program crashes.
    setbuf(stdout, NULL);


    // set the map to point to the hardcoded map
    map = load_map("map.txt",&height, &width);
    //if no map return error
    if (map == NULL) {
        return ERR_NO_MAP;
    }
    // allocate space for the dots
    dot_map = malloc(width * height);
    // copy dots into dot_map
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y * width + x] == DOT) {
                dot_map[y * width + x] = DOT;
            } else {
                dot_map[y * width + x] = EMPTY;
            }
        }
    }

    // input holds the user input
    char input = 0;
    //intialize player and ghost locations
    int player_x = -1, player_y = -1;
    int ghosts_x[NUM_GHOSTS], ghosts_y[NUM_GHOSTS];
    int ghost_count = 0;

    //loop to find player and set player locations
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char current = map[y * width + x];
            //if character on map at (x,y) is P or G set player or ghost coordinates
            if (current == PLAYER) {
                player_y = y;
                player_x = x;
            } else if (current == GHOST && ghost_count < NUM_GHOSTS) {
                ghosts_y[ghost_count] = y;
                ghosts_x[ghost_count] = x;
                ghost_count++;
            }
        }
    }

    //if player not found free maps and return error message
    if (player_x == -1 || player_y == -1) {
        free(map);
        free(dot_map);
        return ERR_NO_PLAYER;
    }
    //if two ghosts aren't found free maps and return error message
    if (ghost_count < NUM_GHOSTS) {
        free(map);
        free(dot_map);
        return ERR_NO_GHOSTS;
    }
    // loop until we hit the end of input
    while (input != EOF) {
        // print the map
        print_map();

        // get a character - blocks until one is input
        input = getch();

        // move the player
        move_player(&player_y, &player_x, input);

        //for number of ghosts update ghost location
        for (int ghost = 0; ghost < NUM_GHOSTS; ghost++) {
            update_ghost(player_y, player_x, &ghosts_y[ghost], &ghosts_x[ghost]);
        }

        //if check loss return YOU_LOSE output lose message and break loop
        if (check_loss(player_y, player_x, ghosts_y, ghosts_x) == YOU_LOSE) {
            print_map();
            printf("Sorry, you lose.\n");
            break;
        }
        //if check win return YOU_WIN output win message and break loop
        if (check_win() == YOU_WIN) {
            print_map();
            printf("Congratulations! You win!\n");
            break;
        }
    }

    // free the dot_map and map
    free(dot_map);
    free(map);

    // You must return the correct error code from defines.h from main depending on what happened
    return NO_ERROR;
}
void update_ghost(int player_y, int player_x, int * ghost_y, int * ghost_x) {
    // check if the ghost can see the player
    char direction = sees_player(player_y, player_x, *ghost_y, *ghost_x);


    // if the ghost doesn't see the player
    if (direction == SEES_NOTHING) {
        // get a random direction, keeping track of if we already tried it
        char tried_directions[] = {0,0,0,0};
        char direction_map[] = {UP, LEFT, DOWN, RIGHT};
        int move_result;
        do {
            // get a random direction
            int direction_int = rand() % 4;
            // set that we tried the direction
            tried_directions[direction_int] = 1;
            // get the input direction to move
            direction = direction_map[direction_int];
            // try it!
            move_result = move_ghost(ghost_y, ghost_x, direction);
            // if we try all the directions, bail out - we can't move
        } while (move_result != MOVED_OKAY &&
                 (!tried_directions[0] || !tried_directions[1] || !tried_directions[2] || !tried_directions[3]));
    } else if (direction != EATING_PLAYER) {
        // if the ghost does see the player (but is not eating them), move in that direction
        move_ghost(ghost_y, ghost_x, direction);
    }
}