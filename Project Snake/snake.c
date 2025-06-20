#define _CRT_SECURE_NO_WARNINGS

#include "snake.h"

SNAKE_LOOK snakeLook = SNAKE_LOOK_CLASSIC;
int length, bendNo, len, life;
char key;
COORDINATE head, bend[500], food, body[30];

void goToXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void print() {
    printf("\tWelcome to the Snake game. (press any key to continue)\n");
    _getch();
    system("cls");
    printf("\tGame instructions:\n");
    printf("\n-> Use arrow keys to move the snake.\n-> You have three lives.\n-> Pause the game by pressing any button, press again to unpause.\n-> If you want to exit, press esc.\n");
    printf("\nPress any key to play game.");
    if (_getch() == 27)
        exit(0);
}

void delay(long double k) {
    score();
    long double i;
    for (i = 0; i <= (10000000); i++);
}

void boarder() {
    system("cls");
    int i;
    goToXY(food.x, food.y);
    printf("x");
    for (i = 10; i < 71; i++) {
        goToXY(i, 10); printf("!");
        goToXY(i, 30); printf("!");
    }
    for (i = 10; i < 31; i++) {
        goToXY(10, i); printf("!");
        goToXY(70, i); printf("!");
    }
}

int score() {
    int sc;
    goToXY(20, 8);
    sc = length - 5;
    printf("SCORE : %d", (length - 5));
    goToXY(50, 8);
    printf("Life : %d", life);
    return sc;
}

int scoreOnly() {
    int sc = score();
    system("cls");
    return sc;
}

void foodFunc() {
    if (head.x == food.x && head.y == food.y) {
        length++;
        time_t a = time(0);
        srand((unsigned)a);
        food.x = rand() % 70;
        if (food.x <= 10) food.x += 11;
        food.y = rand() % 30;
        if (food.y <= 10) food.y += 11;
    }
    else if (food.x == 0) {
        food.x = rand() % 70;
        if (food.x <= 10) food.x += 11;
        food.y = rand() % 30;
        if (food.y <= 10) food.y += 11;
    }
}

void move() {
    int a, i;
    int running = 1;
    while (running && life > 0) {
        foodFunc();
        fflush(stdin);
        len = 0;
        for (i = 0; i < 30; i++) {
            body[i].x = 0; body[i].y = 0;
            if (i == length) break;
        }
        delay(length);
        boarder();
        // Directions:
        if (head.direction == RIGHT) right();
        else if (head.direction == LEFT) left();
        else if (head.direction == DOWN) down();
        else if (head.direction == UP) up();

        // Collision
        int check = 0;
        for (i = 4; i < length; i++)
            if (body[0].x == body[i].x && body[0].y == body[i].y) check++;
        if (head.x <= 10 || head.x >= 70 || head.y <= 10 || head.y >= 30 || check != 0) {
            life--;
            if (life > 0) {
                head.x = 25; head.y = 20; bendNo = 0; head.direction = RIGHT;
                continue;
            }
            else break;
        }

        if (_kbhit()) {
            a = _getch();
            if (a == 27) { system("cls"); break; }
            key = _getch();
            if ((key == RIGHT && head.direction != LEFT && head.direction != RIGHT) ||
                (key == LEFT && head.direction != RIGHT && head.direction != LEFT) ||
                (key == UP && head.direction != DOWN && head.direction != UP) ||
                (key == DOWN && head.direction != UP && head.direction != DOWN)) {
                bendNo++;
                bend[bendNo] = head;
                head.direction = key;
                if (key == UP) head.y--;
                if (key == DOWN) head.y++;
                if (key == RIGHT) head.x++;
                if (key == LEFT) head.x--;
            }
            else if (key == 27) {
                system("cls");
                break;
            }
            else {
                printf("\a");
            }
        }
    }
}

void left() {
    int i;
    for (i = 0; i <= (bend[bendNo].x - head.x) && len < length; i++) {
        goToXY((head.x + i), head.y);
        if (len == 0) printf("<");
        else {
            switch (snakeLook) {
            case SNAKE_LOOK_CLASSIC: printf("*"); break;
            case SNAKE_LOOK_DASHED:  printf((i % 2 == 0) ? "-" : " "); break;
            case SNAKE_LOOK_DOUBLE_LINE: printf("="); break;
            default: printf("*");
            }
        }
        body[len].x = head.x + i;
        body[len].y = head.y;
        len++;
    }
    bendFunc();
    if (!_kbhit()) head.x--;
}
void right() {
    int i;
    for (i = 0; i <= (head.x - bend[bendNo].x) && len < length; i++) {
        body[len].x = head.x - i; body[len].y = head.y;
        goToXY(body[len].x, body[len].y);
        if (len == 0) printf(">");
        else {
            switch (snakeLook) {
            case SNAKE_LOOK_CLASSIC: printf("*"); break;
            case SNAKE_LOOK_DASHED:  printf((i % 2 == 0) ? "-" : " "); break;
            case SNAKE_LOOK_DOUBLE_LINE: printf("="); break;
            default: printf("*");
            }
        }
        len++;
    }
    bendFunc();
    if (!_kbhit()) head.x++;
}
void up() {
    int i;
    for (i = 0; i <= (bend[bendNo].y - head.y) && len < length; i++) {
        goToXY(head.x, head.y + i);
        if (len == 0) printf("^");
        else {
            switch (snakeLook) {
            case SNAKE_LOOK_CLASSIC: printf("*"); break;
            case SNAKE_LOOK_DASHED:  printf((i % 2 == 0) ? "-" : " "); break;
            case SNAKE_LOOK_DOUBLE_LINE: printf("="); break;
            default: printf("*");
            }
        }
        body[len].x = head.x; body[len].y = head.y + i; len++;
    }
    bendFunc();
    if (!_kbhit()) head.y--;
}
void down() {
    int i;
    for (i = 0; i <= (head.y - bend[bendNo].y) && len < length; i++) {
        goToXY(head.x, head.y - i);
        if (len == 0) printf("v");
        else {
            switch (snakeLook) {
            case SNAKE_LOOK_CLASSIC: printf("*"); break;
            case SNAKE_LOOK_DASHED:  printf((i % 2 == 0) ? "-" : " "); break;
            case SNAKE_LOOK_DOUBLE_LINE: printf("="); break;
            default: printf("*");
            }
        }
        body[len].x = head.x; body[len].y = head.y - i; len++;
    }
    bendFunc();
    if (!_kbhit()) head.y++;
}
void bendFunc() {
    int i, j, diff;
    for (i = bendNo; i >= 0 && len < length; i--) {
        if (bend[i].x == bend[i - 1].x) {
            diff = bend[i].y - bend[i - 1].y;
            if (diff < 0)
                for (j = 1; j <= (-diff) && len < length; j++) {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y + j;
                    goToXY(body[len].x, body[len].y);
                    switch (snakeLook) {
                    case SNAKE_LOOK_CLASSIC: printf("*"); break;
                    case SNAKE_LOOK_DASHED:  printf((j % 2 == 0) ? "|" : " "); break;
                    case SNAKE_LOOK_DOUBLE_LINE: printf("||"); break;
                    default: printf("*");
                    }
                    len++; if (len == length) break;
                }
            else if (diff > 0)
                for (j = 1; j <= diff && len < length; j++) {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y - j;
                    goToXY(body[len].x, body[len].y);
                    switch (snakeLook) {
                    case SNAKE_LOOK_CLASSIC: printf("*"); break;
                    case SNAKE_LOOK_DASHED:  printf((j % 2 == 0) ? "|" : " "); break;
                    case SNAKE_LOOK_DOUBLE_LINE: printf("||"); break;
                    default: printf("*");
                    }
                    len++; if (len == length) break;
                }
        }
        else if (bend[i].y == bend[i - 1].y) {
            diff = bend[i].x - bend[i - 1].x;
            if (diff < 0)
                for (j = 1; j <= (-diff) && len < length; j++) {
                    body[len].x = bend[i].x + j; body[len].y = bend[i].y;
                    goToXY(body[len].x, body[len].y);
                    switch (snakeLook) {
                    case SNAKE_LOOK_CLASSIC: printf("*"); break;
                    case SNAKE_LOOK_DASHED:  printf((j % 2 == 0) ? "-" : " "); break;
                    case SNAKE_LOOK_DOUBLE_LINE: printf("="); break;
                    default: printf("*");
                    }
                    len++; if (len == length) break;
                }
            else if (diff > 0)
                for (j = 1; j <= diff && len < length; j++) {
                    body[len].x = bend[i].x - j; body[len].y = bend[i].y;
                    goToXY(body[len].x, body[len].y);
                    switch (snakeLook) {
                    case SNAKE_LOOK_CLASSIC: printf("*"); break;
                    case SNAKE_LOOK_DASHED:  printf((j % 2 == 0) ? "-" : " "); break;
                    case SNAKE_LOOK_DOUBLE_LINE: printf("="); break;
                    default: printf("*");
                    }
                    len++; if (len == length) break;
                }
        }
    }
}

int snake(const char* filename) {
    (void)filename;
    print();
    system("cls");
    length = 5;
    head.x = 25; head.y = 20; head.direction = RIGHT;
    bendNo = 0; bend[0] = head;
    life = 3;
    foodFunc();
    move();
    system("cls");
    printf("Game over!\nPress any key to return to the menu.\n");
    _getch();
    return length - 5;
}
