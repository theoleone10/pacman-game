// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "colours.h"
#include "map.h"

extern char *map, *dot_map;
extern int width, height;

/**
 * You should use this function to print out individual characters
 * in the colours used in the demo.
 * @param c
 */
static void printc(char c) {
    switch (c) {
        case WALL:
            change_text_colour(BLUE);
            break;
        case GHOST:
            change_text_colour(PINK);
            break;
        case PLAYER:
            change_text_colour(YELLOW);
            break;
        case DOT:
            change_text_colour(WHITE);
            break;
        default:
            change_text_colour(WHITE);
    }
    printf("%c ", c);
}

void print_map(void) {
    //print row of walls
    for (int k = 0; k<width+2;k++)
        printc(WALL);
    printf("\n");
    //print first wall in first row
    printc(WALL);
    //print out map given
    for (int i = 0; i<height; i++){
        for(int j = 0; j<width;j++)
            printc(map[i*width+j]);
        //print last wall in current row
        printc(WALL);
        printf("\n");
        //print first wall in next row
        printc(WALL);
    }
    //print final row of walls
    for (int l = 0; l<width+1;l++)
        printc(WALL);
    printf("\n");
}

int is_wall(int y, int x) {
    //if out of bounds of map is wall
    if (y < 0 || y >= height || x < 0 || x >= width) {
        return YES_WALL;
    }

    //if wall on map is wall
    if (map[y * width + x] == WALL) {
        return YES_WALL;
    }
    return NOT_WALL;
}

char * load_map(char * filename, int *map_height, int *map_width) {
    //intial mapFile pointer
    FILE *mapFile = NULL;

    //open file to mapFile
    mapFile = fopen(filename,"r");

    //if map is null return null
    if (mapFile == NULL){
        printf("file not found\n");
        return NULL;
    } else {
        //initialize height and width characters to 0
        int i=0;
        int j=0;
        //count first row of characters
        while(getc(mapFile) != '\n') {
            j++;
        }
        //decrease count by 1 to ignore final character for now
        j--;
        //divide count by 3 to ignore spaces between characters
        j /= 3;
        //add one to account for final character that was ignored
        j++;
        //set width to j
        width = j;

        //account for first row when iterating through file
        i++;
        //count each time a new line is found and add one to height count
        while(!feof(mapFile)){
            if (getc(mapFile) == '\n'){
                i++;
            }
        }
        //add one to height to account for final row with no new line character
        i++;
        //set height to i
        height = i;
        //set pointers to width and height found
        *map_height = i;
        *map_width = j;
    }
    //close and reopen file to start reading again
    fclose(mapFile);
    mapFile = fopen(filename,"r");
    //allacote memory for map
    map = (char*) malloc((width) * (height) * sizeof(char));
    //iterate through file sending characters found to map
    for(int i = 0; i<height;i++){
        for(int j = 0; j<width;j++) {
            //get value of character
            char value = getc(mapFile);
            //set position in map to specific character found
            switch(value) {
                case ('G'):
                    map[i*width+j] = GHOST;
                    break;
                case ('.'):
                    map[i*width+j] = DOT;
                    break;
                case ('P'):
                    map[i*width+j] = PLAYER;
                    break;
                case ('W'):
                    map[i*width+j] = WALL;
                    break;
                case (' '):
                    map[i*width+j] = EMPTY;
                    break;
            }
            //if loop number is not the final character skip 2 characters to account for spaces
            if(j!=(width-1)){
                getc(mapFile);
                getc(mapFile);
            }
        }
        //skip new line character
        getc(mapFile);
    }

    //close file
    fclose(mapFile);
    //return updated map
    return map;

}