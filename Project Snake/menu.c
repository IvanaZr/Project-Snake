#define _CRT_SECURE_NO_WARNINGS

#include "menu.h"
#include "snake.h"
#include "file_manager.h"


char g_playerDataFile[21] = "players.txt";
#define CONFIG_FILE "config.txt"

// Loads filename from config.txt, or uses default if not found
void loadPlayerDataFile() {
    FILE* fp = fopen(CONFIG_FILE, "r");
    if (fp) {
        if (fgets(g_playerDataFile, sizeof(g_playerDataFile), fp)) {
            // Remove newline
            g_playerDataFile[strcspn(g_playerDataFile, "\n")] = '\0';
        }
        fclose(fp);
    }
    else {
        // If no config file, use default
        strncpy(g_playerDataFile, "players.txt", sizeof(g_playerDataFile));
        g_playerDataFile[sizeof(g_playerDataFile) - 1] = '\0';
    }
}

// Saves current player data file name to config.txt
void savePlayerDataFile() {
    FILE* fp = fopen(CONFIG_FILE, "w");
    if (fp) {
        fprintf(fp, "%s\n", g_playerDataFile);
        fclose(fp);
    }
}

// Checking duplicate nicknames
int nicknameExists(const char* nickname, RECORD_LIST* list) {
    for (size_t i = 0; i < list->count; ++i)
        if (strcmp(list->records[i].name, nickname) == 0)
            return 1;
    return 0;
}

// Sort descending by score
int compareScores(const void* a, const void* b) {
    const PLAYER_RECORD* recA = (const PLAYER_RECORD*)a;
    const PLAYER_RECORD* recB = (const PLAYER_RECORD*)b;
    if (recA->score < recB->score) return 1;
    if (recA->score > recB->score) return -1;
    return 0;
}

// Compare names 
int compareNames(const void* key, const void* elem) {
    return _stricmp((const char*)key, ((const PLAYER_RECORD*)elem)->name);
}
int compareNamesSort(const void* a, const void* b) {
    return _stricmp(((const PLAYER_RECORD*)a)->name, ((const PLAYER_RECORD*)b)->name);
}

// Inline function example 
static inline int minScore(int a, int b) { return (a < b) ? a : b; }

// Recursive search for score
int recursiveSearchByScore(const PLAYER_RECORD* arr, int left, int right, uint32_t score) {
    if (left > right) return -1;
    if (arr[left].score == score) return left;
    return recursiveSearchByScore(arr, left + 1, right, score);
}

// Snake look menu
int menuSnakeLook() {
    int sel;
    system("cls");
    printf("-- Snake Look Menu --\n");
    printf("1) Classic  ****>\n");
    printf("2) Dashed   - - >\n");
    printf("3) Double-line  ====>\n");
    printf("Select snake look: ");
    while (scanf("%d", &sel) != 1 || sel < 1 || sel > 3) {
        printf("Please enter a valid number (1-3): ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return sel - 1;
}

// Main menu
MENU_OPTION showMainMenu() {
    int sel;
    system("cls");
    printf("== Main Menu ==\n");
    printf("1) Play Snake\n");
    printf("2) Manage Records\n");
    printf("3) Exit\n");
    printf("Select: ");
    while (scanf("%d", &sel) != 1 || sel < 1 || sel > 3) {
        printf("Please enter a valid number (1-3): ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return (MENU_OPTION)sel;
}

int handleMainMenu(MENU_OPTION opt) {
    switch (opt) {
    case MENU_OPTION_PLAY: {
        RECORD_LIST list = fmReadAll(g_playerDataFile);
        PLAYER_RECORD rec;
        int duplicate;
        do {
            printf("Enter your nickname: ");
            if (fgets(rec.name, MAX_NAME_LEN, stdin))
                rec.name[strcspn(rec.name, "\n")] = '\0';
            duplicate = nicknameExists(rec.name, &list);
            if (duplicate)
                printf("Nickname '%s' already exists. Please choose a different one.\n", rec.name);
        } while (duplicate || strlen(rec.name) == 0);
        fmFreeList(&list);

        snakeLook = menuSnakeLook();

        const char* lookNames[] = { "Classic", "Dashed", "Double-line" };
        system("cls");
        printf("Starting Snake for %s (Look: %s).\n", rec.name, lookNames[snakeLook]);
        rec.score = snake(NULL);
        rec.snakeLook = snakeLook;
        printf("You scored: %u\n", rec.score);

        if (fmCreate(g_playerDataFile, &rec))
            printf("New record created for %s with score %u.\n", rec.name, rec.score);
        else
            printf("Failed to create new record.\n");

        printf("\nPress ESC to return to main menu.");
        while (_getch() != 27);
        return 0;
    }

    case MENU_OPTION_RECORDS: {
        RECORDS_OPTION sub;
        do {
            sub = showRecordsMenu();
            handleRecordsMenu(sub);
        } while (sub != RECORDS_OPTION_BACK);
        return 0;
    }

    case MENU_OPTION_EXIT: {
        char ans = 0;
        while (1) {
            printf("Are you sure you want to exit? (y/n): ");
            ans = getchar();
            while (getchar() != '\n');
            if (ans == 'y' || ans == 'Y') {
                return 1;
            }
            else if (ans == 'n' || ans == 'N') {
                printf("Exit cancelled. Returning to main menu.\n");
                printf("Press ESC to continue.\n");
                while (_getch() != 27);
                return 0;
            }
            else {
                printf("Invalid input! Please press 'y' for yes or 'n' for no.\n");
            }
        }
    }
    default: return 0;
    }
}

// Submenu
RECORDS_OPTION showRecordsMenu() {
    int sel;
    system("cls");
    printf("-- Records Menu --\n");
    printf("1) List Records\n");
    printf("2) Update Record\n");
    printf("3) Delete Record\n");
    printf("4) Search Player By Name\n");
    printf("5) Search Player By Score\n");
    printf("6) Rename Player\n");
    printf("7) Rename Player Data File\n");
    printf("8) Back to Main Menu\n");
    printf("Select: ");
    while (scanf("%d", &sel) != 1 || sel < 1 || sel > 8) {
        printf("Please enter a valid number (1-8): ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return (RECORDS_OPTION)sel;
}

void handleRecordsMenu(RECORDS_OPTION opt) {
    RECORD_LIST list;
    PLAYER_RECORD rec;
    uint32_t score;

    switch (opt) {
    case RECORDS_OPTION_LIST:
        system("cls");
        list = fmReadAll(g_playerDataFile);

        // File size demo (fseek/ftell/rewind)
        FILE* fp = fopen(g_playerDataFile, "r");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            long size = ftell(fp);
            rewind(fp);
            printf("[INFO] Data file size: %ld bytes\n", size);
            fclose(fp);
        }

        if (list.count == 0) {
            printf("No player records found.\n");
        }
        else {
            char sortChoice = 0;
            printf("Sort records by score (highest first)? (y/n): ");
            sortChoice = getchar();
            while (getchar() != '\n');
            if (sortChoice == 'y' || sortChoice == 'Y') {
                qsort(list.records, list.count, sizeof(PLAYER_RECORD), compareScores);
            }
            const char* snakeLookStr[] = { "Classic", "Dashed", "Double-line" };
            printf("Player Records:\n");
            for (size_t i = 0; i < list.count; ++i) {
                printf("%zu) %s - %u   [%s]\n", i + 1,
                    list.records[i].name,
                    list.records[i].score,
                    snakeLookStr[list.records[i].snakeLook]);
            }
        }
        fmFreeList(&list);
        printf("\nPress ESC to return.");
        while (_getch() != 27);
        break;

    case RECORDS_OPTION_UPDATE:
        list = fmReadAll(g_playerDataFile);
        if (list.count == 0) {
            system("cls");
            printf("No player records to update.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        system("cls");
        printf("Select a player to update and then play:\n");
        for (size_t i = 0; i < list.count; ++i) {
            const char* snakeLookStr[] = { "Classic", "Dashed", "Double-line" };
            printf("%zu) %s (previous high score: %u) [%s]\n",
                i + 1, list.records[i].name, list.records[i].score,
                snakeLookStr[list.records[i].snakeLook]);
        }
        printf("Enter number (ESC to cancel): ");
        char buf[32];
        if (!fgets(buf, sizeof(buf), stdin) || buf[0] == 27 || buf[0] == '\x1B') {
            fmFreeList(&list);
            break;
        }
        int idx = 0;
        if (sscanf(buf, "%d", &idx) != 1 || idx < 1 || idx >(int)list.count) {
            printf("Invalid selection.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        system("cls");
        printf("Starting Snake for %s.\n", list.records[idx - 1].name);
        printf("Previous high score: %u\n", list.records[idx - 1].score);
        printf("Snake look: %s\n", (list.records[idx - 1].snakeLook == SNAKE_LOOK_CLASSIC) ? "Classic"
            : (list.records[idx - 1].snakeLook == SNAKE_LOOK_DASHED) ? "Dashed"
            : "Double-line");
        score = snake(NULL);
        printf("Previous high score: %u\n", list.records[idx - 1].score);
        printf("You scored: %u\n", score);

        if (score > list.records[idx - 1].score) {
            strncpy(rec.name, list.records[idx - 1].name, MAX_NAME_LEN);
            rec.score = score;
            rec.snakeLook = list.records[idx - 1].snakeLook;
            if (fmUpdate(g_playerDataFile, &rec))
                printf("High score updated for %s to %u!\n", rec.name, rec.score);
            else
                printf("Failed to update record.\n");
        }
        else {
            printf("Did not beat existing high score of %u.\n", list.records[idx - 1].score);
        }
        fmFreeList(&list);
        printf("\nPress ESC to return.");
        while (_getch() != 27);
        break;

    case RECORDS_OPTION_DELETE:
        list = fmReadAll(g_playerDataFile);
        if (list.count == 0) {
            system("cls");
            printf("No player records to delete.\n");
            fmFreeList(&list);
            printf("\nPress any key to return.");
            getchar();
            break;
        }
        system("cls");
        printf("Select a player to delete:\n");
        for (size_t i = 0; i < list.count; ++i) {
            const char* snakeLookStr[] = { "Classic", "Dashed", "Double-line" };
            printf("%zu) %s (high score: %u) [%s]\n",
                i + 1, list.records[i].name, list.records[i].score,
                snakeLookStr[list.records[i].snakeLook]);
        }
        printf("Enter number (or ESC to return): ");
        char delBuf[32];
        if (!fgets(delBuf, sizeof(delBuf), stdin) || delBuf[0] == 27 || delBuf[0] == '\x1B') {
            fmFreeList(&list);
            printf("Cancelled.\nPress any key to return.");
            getchar();
            break;
        }
        int delIdx = 0;
        if (sscanf(delBuf, "%d", &delIdx) != 1 || delIdx < 1 || delIdx >(int)list.count) {
            printf("Invalid selection.\n");
            fmFreeList(&list);
            printf("\nPress any key to return.");
            getchar();
            break;
        }

        printf("Are you sure you want to delete player '%s'? (y/n): ",
            list.records[delIdx - 1].name);
        char confirm = getchar();
        while (getchar() != '\n');
        if (confirm != 'y' && confirm != 'Y') {
            printf("Deletion cancelled. Press any key to return.");
            getchar();
            fmFreeList(&list);
            break;
        }

        if (fmDelete(g_playerDataFile, list.records[delIdx - 1].name))
            printf("Record for %s deleted.\n", list.records[delIdx - 1].name);
        else
            printf("Record not found.\n");
        fmFreeList(&list);
        printf("\nPress any key to return.");
        getchar();
        break;

    case RECORDS_OPTION_SEARCH_NAME:
        list = fmReadAll(g_playerDataFile);
        if (list.count == 0) {
            printf("No player records to search.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return...");
            while (_getch() != 27);
            break;
        }
        system("cls");
        printf("Enter nickname to search (ESC to cancel): ");
        char searchName[MAX_NAME_LEN];
        if (!fgets(searchName, sizeof(searchName), stdin) || searchName[0] == 27 || searchName[0] == '\x1B') {
            fmFreeList(&list);
            break;
        }
        searchName[strcspn(searchName, "\n")] = '\0';
        qsort(list.records, list.count, sizeof(PLAYER_RECORD), compareNamesSort); // sort alphabetically
        PLAYER_RECORD* found = bsearch(searchName, list.records, list.count, sizeof(PLAYER_RECORD), compareNames);
        if (found) {
            const char* snakeLookStr[] = { "Classic", "Dashed", "Double-line" };
            printf("Found: %s (Score: %u) [%s]\n", found->name, found->score, snakeLookStr[found->snakeLook]);
        }
        else {
            printf("Player not found!\n");
        }
        fmFreeList(&list);
        printf("\nPress ESC to return.");
        while (_getch() != 27);
        break;

    case RECORDS_OPTION_SEARCH_SCORE:
        list = fmReadAll(g_playerDataFile);
        if (list.count == 0) {
            printf("No player records to search.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        system("cls");
        printf("Enter score to search (ESC to cancel): ");
        char scoreBuf[16];
        if (!fgets(scoreBuf, sizeof(scoreBuf), stdin) || scoreBuf[0] == 27 || scoreBuf[0] == '\x1B') {
            fmFreeList(&list);
            break;
        }
        // Only allow numeric input (no negative)
        int valid = 1;
        for (size_t i = 0; i < strlen(scoreBuf) && scoreBuf[i] != '\n'; ++i) {
            if (!isdigit((unsigned char)scoreBuf[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid || strlen(scoreBuf) == 1) {
            printf("Invalid score input.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        score = (uint32_t)atoi(scoreBuf);

        int foundIdx = recursiveSearchByScore(list.records, 0, (int)(list.count - 1), score);
        if (foundIdx == -1)
            printf("No player found with score %u.\n", score);
        else {
            const char* snakeLookStr[] = { "Classic", "Dashed", "Double-line" };
            printf("Found: %s (Score: %u) [%s]\n",
                list.records[foundIdx].name, list.records[foundIdx].score,
                snakeLookStr[list.records[foundIdx].snakeLook]);
        }
        fmFreeList(&list);
        printf("\nPress ESC to return.");
        while (_getch() != 27);
        break;

    case RECORDS_OPTION_RENAME_PLAYER: {
        RECORD_LIST list = fmReadAll(g_playerDataFile);
        if (list.count == 0) {
            system("cls");
            printf("No player records to rename.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        system("cls");
        printf("Select a player to rename:\n");
        for (size_t i = 0; i < list.count; ++i) {
            printf("%zu) %s (score: %u)\n", i + 1, list.records[i].name, list.records[i].score);
        }
        printf("Enter number (ESC to cancel): ");
        char buf[32];
        if (!fgets(buf, sizeof(buf), stdin) || buf[0] == 27 || buf[0] == '\x1B') {
            fmFreeList(&list);
            break;
        }
        int idx = 0;
        if (sscanf(buf, "%d", &idx) != 1 || idx < 1 || idx >(int)list.count) {
            printf("Cancelled or invalid selection.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        printf("Current nickname: %s\n", list.records[idx - 1].name);
        printf("Enter new nickname: ");
        char newName[MAX_NAME_LEN];
        if (fgets(newName, MAX_NAME_LEN, stdin)) {
            newName[strcspn(newName, "\n")] = '\0';
        }
        // Check for duplicates
        if (nicknameExists(newName, &list)) {
            printf("That nickname already exists! Rename cancelled.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        if (strlen(newName) == 0) {
            printf("Empty nickname, cancelled.\n");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        // Apply rename
        strncpy(list.records[idx - 1].name, newName, MAX_NAME_LEN);
        // Write all records back
        FILE* fp = fopen(g_playerDataFile, "w");
        if (!fp) {
            perror("fopen");
            fmFreeList(&list);
            printf("\nPress ESC to return.");
            while (_getch() != 27);
            break;
        }
        for (size_t i = 0; i < list.count; ++i) {
            fprintf(fp, "%s %u %d\n", list.records[i].name, list.records[i].score, list.records[i].snakeLook);
        }
        fclose(fp);
        printf("Nickname updated!\n");
        fmFreeList(&list);
        printf("\nPress ESC to return.");
        while (_getch() != 27);
        break;
    }

    case RECORDS_OPTION_FILE_RENAME: {
        char newFileName[21];
        printf("Current player data file is '%s'.\n", g_playerDataFile);
        printf("Enter new filename to rename it to (max 20 chars): ");
        if (fgets(newFileName, sizeof(newFileName), stdin)) {
            newFileName[strcspn(newFileName, "\n")] = '\0';
        }
        else {
            printf("Invalid input.\nPress any key to return...");
            getchar();
            break;
        }
        if (strlen(newFileName) == 0) {
            printf("New filename cannot be empty.\nPress any key to return...");
            getchar();
            break;
        }
        if (rename(g_playerDataFile, newFileName) == 0) {
            printf("Player data file successfully renamed to '%s'.\n", newFileName);
            strncpy(g_playerDataFile, newFileName, sizeof(g_playerDataFile));
            g_playerDataFile[sizeof(g_playerDataFile) - 1] = '\0';
            savePlayerDataFile(); 
        }
        else {
            perror("Failed to rename player data file");
        }

        printf("Press any key to return...");
        getchar();
        break;
    }
    case RECORDS_OPTION_BACK:
    default:
        break;
    }
}

