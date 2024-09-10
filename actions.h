#ifndef MAZE_ANT

#define MAX_SIZE 100

typedef struct {
int x;
int y;
} Position;

typedef struct {
  Position stack[MAX_SIZE];
  int top;
} MemStack;

int checkNum(Position pos);
void mark(Position pos);
void move_f(Position *pos);
void move_b(Position *pos);
void move_l(Position *pos);
void move_r(Position *pos);
int cwl(Position pos);
int cwr(Position pos);
int cwf(Position pos);
int cwb(Position pos);
void reset_itch_left();
void reset_itch_right();
void reset_itch_forward();
void reset_itch_backward();
void push(MemStack *stack, Position pos);
void pop(MemStack *stack);
void peek(MemStack *stack, Position *pos);
void _clear(MemStack *stack);
void bjpi(Position *pos, int num_positions);
void cjpi(Position *pos);
void backtrack(Position *pos, Position backTrackPos);
void RP(int n, int t, Position *pos, MemStack *stack, Position backTrackPos);

#endif
