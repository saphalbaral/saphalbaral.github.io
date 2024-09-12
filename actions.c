Created by: Arafat Farouque, Anirvan Kothuri, and Saphal Baral
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

extern int MAX_NUMBER_OF_STEPS;
char str[MAX_SIZE];
char intelligence[50];
// 2D array representing the maze
int maze[MAX_SIZE][MAX_SIZE];
// Total points collected in the maze
int points = 0;

// Itch flags indicating the desire to move in each direction
int itch_left = 0;
int itch_right = 0;
int itch_forward = 0;
int itch_backward = 0;

// Position and memory structures
typedef struct {
    int x, y;
} Position;
// Stack structure for memory management in navigating the maze
typedef struct {
    Position stack[MAX_SIZE];
    int top;
} MemStack;

// Function to check and add points
int checkNum(Position pos) {
    // Verify that coordinates are within maze bounds
    if (pos.x >= 0 && pos.x < MAX_SIZE && pos.y >= 0 && pos.y < MAX_SIZE) {
        // Check if the maze value at the given position is a number (0-9)
        if (maze[pos.x][pos.y] >= 0 && maze[pos.x][pos.y] <= 9) {
            points += maze[pos.x][pos.y];
            maze[pos.x][pos.y] = '+';
        }
    }
    return points;
}

// Mark the current position with a '+' and update points
void mark(Position pos) {
    if (maze[pos.x][pos.y] == ' ') {
        maze[pos.x][pos.y] = '+';
    }
    checkNum(pos);
}

// Move forward (+1 in x direction) and update points
void move_f(Position *pos) {
    if (pos->x + 1 < MAX_SIZE && maze[pos->x + 1][pos->y] != 1) {
        pos->x += 1;
        checkNum(*pos);
    }
}

// Move backward (-1 in x direction) and update points
void move_b(Position *pos) {
    if (pos->x - 1 >= 0 && maze[pos->x - 1][pos->y] != 1) {
        pos->x -= 1;
        checkNum(*pos);
    }
}

// Move left (-1 in y direction) and update points
void move_l(Position *pos) {
    if (pos->y - 1 >= 0 && maze[pos->x][pos->y - 1] != 1) {
        pos->y -= 1;
        checkNum(*pos);
    }
}

// Move right (+1 in y direction) and update points
void move_r(Position *pos) {
    if (pos->y + 1 < MAX_SIZE && maze[pos->x][pos->y + 1] != 1) {
        pos->y += 1;
        checkNum(*pos);
    }
}

// Check if left positions are pheromone-free and update itching flag
int cwl(Position pos) {
    int checkY = pos.y - 1;
    while (checkY >= 0 && maze[pos.x][checkY] == ' ') {
        checkY--;
    }
    itch_left = (checkY != pos.y - 1) ? 1 : 0;
    return itch_left;
}

// Check if right positions are pheromone-free and update itching flag
int cwr(Position pos) {
    int checkY = pos.y + 1;
    while (checkY < MAX_SIZE && maze[pos.x][checkY] == ' ') {
        checkY++;
    }
    itch_right = (checkY != pos.y + 1) ? 1 : 0;
    return itch_right;
}

// Check if front positions are pheromone-free and update itching flag
int cwf(Position pos) {
    int checkX = pos.x + 1;
    while (checkX < MAX_SIZE && maze[checkX][pos.y] == ' ') {
        checkX++;
    }
    itch_forward = (checkX != pos.x + 1) ? 1 : 0;
    return itch_forward;
}

// Check if backward positions are pheromone-free and update itching flag
int cwb(Position pos) {
    int checkX = pos.x - 1;
    while (checkX >= 0 && maze[checkX][pos.y] == ' ') {
        checkX--;
    }
    itch_backward = (checkX != pos.x + 1) ? 1 : 0;
    return itch_backward;
}

// Functions to reset itching flags
void reset_itch_left() { 
    itch_left = 0; 
}
void reset_itch_right() { 
    itch_right = 0; 
}
void reset_itch_forward() {
    itch_forward = 0; 
}
void reset_itch_backward() { 
    itch_backward = 0; 
}

// Push position onto the memory stack
void push(MemStack *stack, Position pos) {
    if (stack->top < MAX_SIZE - 1) {
        stack->top++;
        stack->stack[stack->top].x = pos.x;
        stack->stack[stack->top].y = pos.y;
    } else {
        printf("Stack is full!\n");
    }
}

// Pop a position from the memory stack
void pop(MemStack *stack) {
    if (stack->top >= 0) {
        stack->top--;
    } else {
        printf("Stack is empty!\n");
    }
}

// Peek at the top of the memory stack without popping
void peek(MemStack *stack, Position *pos) {
    if (stack->top >= 0) {
        *pos = stack->stack[stack->top];
    } else {
        printf("Stack is empty!\n");
    }
}

// Clear the entire memory stack
void _clear(MemStack *stack) {
    stack->top = -1;
}

// Bold jump for itching (BJPI) with the given number of positions
void bjpi(Position *pos, int num_positions) {
    int i;
    if (itch_right) { // Jump right
        for (i = 0; i < num_positions && pos->y + 1 < MAX_SIZE && maze[pos->x][pos->y + 1] != 1; i++) {
            move_r(pos);
        }
        reset_itch_right();
    } else if (itch_left) { // Jump left
        for (i = 0; i < num_positions && pos->y - 1 >= 0 && maze[pos->x][pos->y - 1] != 1; i++) {
            move_l(pos);
        }
        reset_itch_left();
    } else if (itch_forward) { // Jump forward
        for (i = 0; i < num_positions && pos->x + 1 < MAX_SIZE && maze[pos->x + 1][pos->y] != 1; i++) {
            move_f(pos);
        }
        reset_itch_forward();
    } else if (itch_backward) { // Jump backward
        for (i = 0; i < num_positions && pos->x - 1 >= 0 && maze[pos->x - 1][pos->y] != 1; i++) {
            move_b(pos);
        }
        reset_itch_backward();
    }
}

// Cautious jump for itching (CJPI) with a single position jump
void cjpi(Position *pos) {
    if (itch_left) {
        move_l(pos);
        reset_itch_left();
    } else if (itch_right) {
        move_r(pos);
        reset_itch_right();
    } else if (itch_forward) {
        move_f(pos);
        reset_itch_forward();
    } else if (itch_backward) {
        move_b(pos);
        reset_itch_backward();
    }
}

// Backtrack to a specified position
void backtrack(Position *pos, Position backTrackPos) {
    pos->x = backTrackPos.x;
    pos->y = backTrackPos.y;
}

// Repeat an action set `n` times for `t` iterations
void RP(int n, int t, Position *pos, MemStack *stack, Position backTrackPos) {
    FILE *input;
    int j = 0;
    if ((input = fopen("intelligence.txt", "r")) == NULL) {
        printf("Error opening intelligence.txt");
        exit(1);
    }

    // Open the intelligence file and create an array to store `n` functions
    char funcList[50][MAX_SIZE];
    int i;
    int jump_distance=2; // created a sample variable for jump_distance since it is not being used in the intelligence file
    for (i = 0; i < n; i++) {
        if ((fscanf(input, "%s", funcList[i])) == EOF) {
            printf("Not enough lines to copy");
            exit(1);
        }
    }
    fclose(input);

    // Repeat the sequence of actions `t` times
    for (j = 0; j < t; j++) {
        for (int k = 0; k < i; k++) {
            if (strcmp(funcList[k], "mark") == 0) {
                mark(*pos);
            } else if (strcmp(funcList[k], "move_f") == 0) {
                move_f(pos);
            } else if (strcmp(funcList[k], "move_b") == 0) {
                move_b(pos);
            } else if (strcmp(funcList[k], "move_l") == 0) {
                move_l(pos);
            } else if (strcmp(funcList[k], "move_r") == 0) {
                move_r(pos);
            } else if (strcmp(funcList[k], "cwl") == 0) {
                cwl(*pos);
            } else if (strcmp(funcList[k], "cwr") == 0) {
                cwr(*pos);
            } else if (strcmp(funcList[k], "cwf") == 0) {
                cwf(*pos);
            } else if (strcmp(funcList[k], "cwb") == 0) {
                cwb(*pos);
            } else if (strcmp(funcList[k], "push") == 0) {
                push(stack, *pos);
            } else if (strcmp(funcList[k], "pop") == 0) {
                pop(stack);
            } else if (strcmp(funcList[k], "peek") == 0) {
                peek(stack, pos);
            } else if (strcmp(funcList[k], "clear") == 0) {
                _clear(stack);
            } else if (strcmp(funcList[k], "BJPI") == 0) {
                bjpi(pos, jump_distance);
            } else if (strcmp(funcList[k], "CJPI") == 0) {
                cjpi(pos);
            } else if (strcmp(funcList[k], "BACKTRACK") == 0) {
                backtrack(pos, backTrackPos);
            }
        }
    }
}
