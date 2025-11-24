#include <stdio.h>
#include "menu.h"

// Menu: Displays options and captures user selections.
void showMenu() {
    printf("\n1. View Orders\n");
    printf("0. Exit\n");
}

int getChoice() {
    int c;
    printf("Enter choice: ");
    scanf("%d", &c);
    return c;
}