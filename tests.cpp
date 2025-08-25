// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <stdio.h>

// make sure not to modify anything in this extern block
extern "C"{
#include "defines.h"
#include "map.h"
#include "actor.h"
#include "game.h"
char *map = NULL, *dot_map = NULL;
int width, height;
}

/**
 * This file is where you should put your tests for your code.
 * Your code must have tests that execute at least 85% of the code in
 * required functions for you to get full marks for the project.
 * Make sure to check out the course videos on automated testing for
 * more information about how to write tests.
 */

/* tests for map.c */
TEST_SUITE_BEGIN("Map tests");

// tests for load_map
TEST_CASE("load_map loads a 3x2 rectangular map") {
    //open a file and write a 3x2 map to it
    FILE *temp = fopen("test_map.txt", "w");
    fprintf(temp, "G  W  .\nP     .");
    fclose(temp);

    //set map height and width to zero
    int map_h = 0, map_w = 0;
    //set map to load map function
    map = load_map("test_map.txt", &map_h, &map_w);

    //check null case
    CHECK(map != NULL);
    //check correct height and width
    CHECK(map_h == 2);
    CHECK(map_w == 3);

    //check first row detected each character correctly
    CHECK(map[0 * map_w + 0] == GHOST);
    CHECK(map[0 * map_w + 1] == WALL);
    CHECK(map[0 * map_w + 2] == DOT);

    //check second row detected each character correctly
    CHECK(map[1 * map_w + 0] == PLAYER);
    CHECK(map[1 * map_w + 1] == EMPTY);
    CHECK(map[1 * map_w + 2] == DOT);

    //free map
    free(map);
}


// tests for is_wall
TEST_CASE("Is Wall Test"){
    //initialize and declare a sample map
    char tiny_map[] = {PLAYER, DOT,
                       DOT, WALL};
    map = tiny_map;
    width = 2;
    height = 2;
    //test position with no wall returns no wall
    CHECK((is_wall(0,0)==NOT_WALL));
    //test position on map with wall is wall
    CHECK((is_wall(1,1)==YES_WALL));
    //test positions off map are a wall
    CHECK((is_wall(1,2)==YES_WALL));
    CHECK((is_wall(2,1)==YES_WALL));
}
TEST_SUITE_END();

/* tests for actor.c */
TEST_SUITE_BEGIN("Actor tests");

// tests for sees_player
TEST_CASE("See player Test"){
    //initialize and declare a sample map
    char tiny_map[] = {DOT, DOT,DOT,
                       DOT, WALL,DOT,
                       DOT, DOT,DOT};
    map = tiny_map;
    width = 3;
    height = 3;

    //check down condition
    CHECK(sees_player(2, 2, 0, 2) == DOWN);
    //check wall between player and ghost reads nothing
    CHECK(sees_player(2, 1, 0, 1) == SEES_NOTHING);
    //check up condition
    CHECK(sees_player(0, 2, 2, 2) == UP);
    //check left condition
    CHECK(sees_player(0, 0, 0, 2) == LEFT);
    //check right condition
    CHECK(sees_player(0, 2, 0, 0) == RIGHT);
    //check completely different places condition
    CHECK(sees_player(0, 0, 2, 2) == SEES_NOTHING);
    //check wall between players horizontally sees nothing
    CHECK(sees_player(1, 0, 1, 2) == SEES_NOTHING);
    //check same location
    CHECK(sees_player(0, 0, 0, 0) == EATING_PLAYER);

}
// tests for move_player
TEST_CASE("Move player Test"){
    //initialize player location
    int player_y=0;
    int player_x =0;
    //initialize and declare map
    char tiny_map[] = {PLAYER, DOT,
                       DOT, WALL};
    map = tiny_map;
    width = 2;
    height = 2;

    //initialize and declare dot map
    char tiny_dot_map[] = {EMPTY,DOT,
                           DOT, WALL};

    dot_map = tiny_dot_map;

    //check move right hits nothing then hits wall
    char direction = RIGHT;
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_OKAY));
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_WALL));
    //check move left hits nothing then hits wall
    direction = LEFT;
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_OKAY));
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_WALL));
    //check move down hits nothing then hits wall
    direction = DOWN;
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_OKAY));
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_WALL));
    //check move up hits nothing then hits wall
    direction = UP;
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_OKAY));
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_WALL));

    //check invalid direction
    direction = 'P';
    CHECK((move_player(&player_y,&player_x,direction)==MOVED_INVALID_DIRECTION));

}
// tests for move_ghost
TEST_CASE("Move ghost Test"){
    //initialize player location
    int player_y=0;
    int player_x =0;
    //initialize and declare map
    char tiny_map[] = {PLAYER, DOT,
                       DOT, WALL};
    map = tiny_map;
    width = 2;
    height = 2;

    //initialize and declare dot map
    char tiny_dot_map[] = {EMPTY,DOT,
                           DOT, WALL};

    dot_map = tiny_dot_map;

    //check move right hits nothing then hits wall
    char direction = RIGHT;
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_OKAY));
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_WALL));
    //check move left hits nothing then hits wall
    direction = LEFT;
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_OKAY));
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_WALL));
    //check move down hits nothing then hits wall
    direction = DOWN;
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_OKAY));
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_WALL));
    //check move up hits nothing then hits wall
    direction = UP;
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_OKAY));
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_WALL));

    //check invalid direction
    direction = 'Q';
    CHECK((move_ghost(&player_y,&player_x,direction)==MOVED_INVALID_DIRECTION));

}
TEST_SUITE_END();

/* tests for game.c */
TEST_SUITE_BEGIN("Game tests");
//int check_loss(int player_y, int player_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]);
// tests for check_win
TEST_CASE("check win"){
    //initialize and declare a map
    char tiny_map[] = {EMPTY, DOT,
                       DOT, WALL};
    dot_map = tiny_map;
    width = 2;
    height = 2;
    //check with dots keep going
    CHECK(check_win()==KEEP_GOING);
    //check when no dots win
    dot_map[1] = EMPTY;
    dot_map[2] = EMPTY;
    CHECK(check_win()==YOU_WIN);

}
// test for check_loss
TEST_CASE("check loss"){
    //initialize ghosts positions
    int ghost_y[2] = {0,1};
    int ghost_x[2] = {0,1};
    //check different positions
    CHECK(check_loss(2,2,ghost_y,ghost_x) == KEEP_GOING);
    //check same x different y
    CHECK(check_loss(2,0,ghost_y,ghost_x) == KEEP_GOING);
    //check same y different x
    CHECK(check_loss(0,2,ghost_y,ghost_x) == KEEP_GOING);
    //check same position
    CHECK(check_loss(0,0,ghost_y,ghost_x) == YOU_LOSE);
}
TEST_SUITE_END();