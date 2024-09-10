//Group Members: Arafat Farouque, Anirvan Kothuri, and Saphal Baral
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "actions.h"

char copiedMaze[MAX_SIZE][MAX_SIZE];
int maze_rows = 0, maze_cols = 0;

void detectMazeDimensions(FILE *input) {
    int row = 0, max_cols = 0;
    char c;

    // Count rows and find the maximum columns per row
    while ((c = fgetc(input)) != EOF) {
        if (c == '\n') {
            row++;
            if (max_cols < maze_cols) {
                max_cols = maze_cols;
            }
            maze_cols = 0;
        } else {
            maze_cols++;
        }
    }
    maze_rows = row;
    maze_cols = max_cols;

    // Rewind to start reading the actual maze data
    rewind(input);
}

// Function to read maze data based on detected dimensions
void readMazeFromFile(FILE *input) {
    int row = 0, col = 0;
    char c;

    detectMazeDimensions(input); 

    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            copiedMaze[i][j] = '*';
        }
    }

    while ((c = fgetc(input)) != EOF) {
        if (c == '\n') {
            row++;
            col = 0;
        } else {
            if (col < MAX_SIZE && row < MAX_SIZE) {
                copiedMaze[row][col] = c;
                col++;
            }
        }

        // Stop reading if we've read the specified number of rows
        if (row >= maze_rows) {
            break;
        }
    }
}

// Function to generate a random starting position within the maze
Position generateRandomStartPosition() {
    Position valid_positions[MAX_SIZE * MAX_SIZE];
    int count = 0;

    // Find all open positions that can be a valid starting point
    for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_cols; j++) {
            if (copiedMaze[i][j] == ' ') {
                valid_positions[count++] = (Position){i, j};
            }
        }
    }

    if (count == 0) {
        printf("No valid starting positions available in the maze.\n");
        exit(1);
    }

    // select a random starting position
    srand((unsigned int)time(NULL));
    int index = rand() % count;
    return valid_positions[index];
}

int main(void) {
    FILE *mazeInput = fopen("maze.txt", "r");

    if (mazeInput == NULL) {
        printf("Error in opening maze.txt file. Please try again.\n");
        return 1;
    }

    // Read the maze data from the maze file
    readMazeFromFile(mazeInput);

    // Generate a random starting position within the maze
    Position start_pos = generateRandomStartPosition();
    printf("Random starting position: (%d, %d)\n", start_pos.x, start_pos.y);

    fclose(mazeInput);

    return 0;
}
