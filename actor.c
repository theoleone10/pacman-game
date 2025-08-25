// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "actor.h"
#include "map.h"   // included for is_wall
#include "game.h"

extern char *map, *dot_map;
extern int height;
extern int width;

char sees_player(int player_y, int player_x, int ghost_y, int ghost_x) {
    //if ghost postition is same as player retunr eating player
    if (player_y == ghost_y && player_x == ghost_x)
        return EATING_PLAYER;

    //if on the same horizontal check if wall between, if not return move direction
    if (player_y == ghost_y) {
        //if ghost is right player iterate through each row to find a wall
        if (player_x < ghost_x) {
            for (int i = player_x + 1; i < ghost_x; i++)
                //if wall found return sees nothing
                if (map[player_y * width + i] == WALL)
                    return SEES_NOTHING;
            //return direction to travel
            return LEFT;
        } else {
            //if ghost is left player iterate through each row to find a wall
            for (int j = ghost_x + 1; j < player_x; j++)
                //if wall found return sees nothing
                if (map[player_y * width + j] == WALL)
                    return SEES_NOTHING;
            //return direction to travel
            return RIGHT;
        }
    }

    //if on the same vertical check if wall between, if not return move direction
    if (player_x == ghost_x) {
        //if ghost is below player iterate through each row to find a wall
        if (player_y < ghost_y) {
            for (int k = player_y + 1; k < ghost_y; k++)
                //if wall found return sees nothing
                if (map[k * width + player_x] == WALL)
                    return SEES_NOTHING;
            //return direction to travel
            return UP;
        } else {
            //if ghost is above player iterate through each row to find a wall
            for (int l = ghost_y + 1; l < player_y; l++)
                //if wall found return sees nothing
                if (map[l * width + player_x] == WALL)
                    return SEES_NOTHING;
            //return direction to travel
            return DOWN;
        }
    }
    return SEES_NOTHING;
}

int move_player(int *y, int *x, char direction) {
    //if invalid direction return error message
    if ((direction != LEFT) && (direction != RIGHT) &&
        (direction != UP) && (direction != DOWN))
        return MOVED_INVALID_DIRECTION;

    //set new varaible to initial location
    int new_y = *y;
    int new_x = *x;

    //depending on direction change new coordinates
    if (direction == RIGHT)
        new_x++;
    else if (direction == LEFT)
        new_x--;
    else if (direction == UP)
        new_y--;
    else if (direction == DOWN)
        new_y++;

    //if wall return moved wall
    if (is_wall(new_y, new_x)) {
        return MOVED_WALL;
    }

    // check if player is going to hit ghost if so return lose
    if (map[new_y * width + new_x] == GHOST) {
        *y = new_y;
        *x = new_x;
        return YOU_LOSE;
    }

    //get empty dots when player eats and move player in the direction given
    map[*y * width + *x] = EMPTY;
    dot_map[*y * width + *x] = EMPTY;
    map[new_y * width + new_x] = PLAYER;
    dot_map[new_y * width + new_x] = EMPTY;

    //replace x and y with the new coordinates
    *y = new_y;
    *x = new_x;

    return MOVED_OKAY;
}


int move_ghost(int *y, int *x, char direction) {
    //if invalid direction return error message
    if ((direction != LEFT) && (direction != RIGHT) &&
        (direction != UP) && (direction != DOWN))
        return MOVED_INVALID_DIRECTION;

    //set new varaible to initial location
    int new_y = *y;
    int new_x = *x;

    //depending on direction change new coordinates
    if (direction == RIGHT) new_x++;
    else if (direction == LEFT) new_x--;
    else if (direction == UP) new_y--;
    else if (direction == DOWN) new_y++;

    //if wall return moved wall
    if (is_wall(new_y, new_x)) {
        return MOVED_WALL;
    }

    //ensure ghosts move within bounds
    if (*x >= 0 && *x < width && *y >= 0 && *y < height &&
        new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) {
        //replace position behind ghost with dot_map
        map[*y * width + *x] = dot_map[*y * width + *x];
        //update map with changed ghost location
        map[new_y * width + new_x] = GHOST;

        //replace x and y with the new coordinates
        *x = new_x;
        *y = new_y;

        return MOVED_OKAY;
    }
    return MOVED_WALL;
}
