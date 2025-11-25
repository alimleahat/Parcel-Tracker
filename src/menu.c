#include <stdio.h>
#include "menu.h"

void showMenu() {
    printf("1. View Orders\n");
    printf("2. Save Orders\n");
    printf("3. Add Order\n");
    printf("4. Search Order\n");
    printf("0. Exit\n");
}

int getChoice() {
    int c;
    printf("Enter choice: ");
    scanf("%d", &c);
    return c;
}