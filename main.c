#define _CRT_SECURE_NO_WARNINGS

#include "menu.h"

int main() {
    MENU_OPTION choice;
    while (1) {
        choice = showMainMenu();
        if (handleMainMenu(choice))
            break;
    }
    printf("Goodbye!\n");
    return 0;
}
