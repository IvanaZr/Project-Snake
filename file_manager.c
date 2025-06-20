#define _CRT_SECURE_NO_WARNINGS

#include "file_manager.h"

// Create file
int fmCreate(const char* filename, const PLAYER_RECORD* rec) {
    FILE* fp = fopen(filename, "a");
    if (!fp) { perror("fopen"); return 0; }
    fprintf(fp, "%s %u %d\n", rec->name, rec->score, rec->snakeLook);
    fclose(fp);
    return 1;
}

// Read from file
RECORD_LIST fmReadAll(const char* filename) {
    RECORD_LIST list = { NULL, 0 };
    FILE* fp = fopen(filename, "r");
    if (!fp) return list;
    PLAYER_RECORD tmp;
    char line[256];
    size_t count = 0, cap = 8;
    list.records = malloc(cap * sizeof(PLAYER_RECORD));
    while (fgets(line, sizeof(line), fp)) {
        int look = 0; 
        int nFields = sscanf(line, "%31s %u %d", tmp.name, &tmp.score, &look);
        if (nFields >= 2) {
            tmp.snakeLook = (SNAKE_LOOK)((nFields == 3) ? look : 0); 
            if (count >= cap) {
                cap *= 2;
                list.records = realloc(list.records, cap * sizeof(PLAYER_RECORD));
            }
            list.records[count++] = tmp;
        }
    }
    list.count = count;
    fclose(fp);
    return list;
}

// Update player
int fmUpdate(const char* filename, const PLAYER_RECORD* rec) {
    RECORD_LIST list = fmReadAll(filename);
    int found = 0;
    for (size_t i = 0; i < list.count; ++i) {
        if (strcmp(list.records[i].name, rec->name) == 0) {
            list.records[i].score = rec->score;
            list.records[i].snakeLook = rec->snakeLook;
            found = 1;
            break;
        }
    }
    if (!found) { fmFreeList(&list); return 0; }
    FILE* fp = fopen(filename, "w");
    if (!fp) { perror("fopen"); fmFreeList(&list); return 0; }
    for (size_t i = 0; i < list.count; ++i)
        fprintf(fp, "%s %u %d\n", list.records[i].name, list.records[i].score, list.records[i].snakeLook);
    fclose(fp);
    fmFreeList(&list);
    return 1;
}

// Delete player
int fmDelete(const char* filename, const char* name) {
    RECORD_LIST list = fmReadAll(filename);
    size_t out = 0;
    int found = 0;
    for (size_t i = 0; i < list.count; ++i) {
        if (strcmp(list.records[i].name, name) != 0) {
            list.records[out++] = list.records[i];
        }
        else {
            found = 1;
        }
    }
    if (!found) { fmFreeList(&list); return 0; }
    FILE* fp = fopen(filename, "w");
    if (!fp) { perror("fopen"); fmFreeList(&list); return 0; }
    for (size_t i = 0; i < out; ++i)
        fprintf(fp, "%s %u %d\n", list.records[i].name, list.records[i].score, list.records[i].snakeLook);
    fclose(fp);
    fmFreeList(&list);
    return 1;
}

// Free memory
void fmFreeList(RECORD_LIST* list) {
    free(list->records);
    list->records = NULL;
    list->count = 0;
}
