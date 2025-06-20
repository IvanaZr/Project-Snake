#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define UP    72
#define DOWN  80
#define LEFT  75
#define RIGHT 77

typedef enum {
    SNAKE_LOOK_CLASSIC = 0,
    SNAKE_LOOK_DASHED,
    SNAKE_LOOK_DOUBLE_LINE
} SNAKE_LOOK;

extern SNAKE_LOOK snakeLook;

typedef struct {
    int x;
    int y;
    int direction;
} COORDINATE;

extern int length;
extern int bendNo;
extern int len;
extern char key;
extern int life;

extern COORDINATE head, bend[500], food, body[30];

void delay(long double k);
void move();
void foodFunc();
int  score();
void print();
void goToXY(int x, int y);
void bendFunc();
void boarder();
void down();
void left();
void up();
void right();
int  scoreOnly();

#endif // SNAKE_H
