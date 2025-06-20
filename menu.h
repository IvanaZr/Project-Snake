#ifndef MENU_H
#define MENU_H

#define MAX_NAME_LEN 20
#define MAX_SCORE(a, b) ((a) > (b) ? (a) : (b))

#include "snake.h" 

typedef enum {
    MENU_OPTION_PLAY = 1,
    MENU_OPTION_RECORDS,
    MENU_OPTION_EXIT
} MENU_OPTION;

typedef enum {
    RECORDS_OPTION_LIST = 1,
    RECORDS_OPTION_UPDATE,
    RECORDS_OPTION_DELETE,
    RECORDS_OPTION_SEARCH_NAME,
    RECORDS_OPTION_SEARCH_SCORE,
    RECORDS_OPTION_RENAME_PLAYER,
    RECORDS_OPTION_BACK,
} RECORDS_OPTION;

typedef struct {
    char       name[MAX_NAME_LEN];
    uint32_t   score;
    SNAKE_LOOK snakeLook; 
} PLAYER_RECORD;

typedef struct {
    PLAYER_RECORD* records;
    size_t         count;
} RECORD_LIST;

MENU_OPTION showMainMenu();
RECORDS_OPTION showRecordsMenu();
int handleMainMenu(MENU_OPTION opt);
void handleRecordsMenu(RECORDS_OPTION opt);
int nicknameExists(const char* nickname, RECORD_LIST* list);
int compareScores(const void* a, const void* b);
int menuSnakeLook();

#endif // MENU_H
