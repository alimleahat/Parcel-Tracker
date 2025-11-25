#include <stdio.h>
#include "menu.h"

void showMenu() {
    printf("1. View Orders");
    printf("2. Order History");
    printf("3. Add Order");
    printf("4. Search Order");
    printf("5. Statistics");
    printf("0. Exit");
}

int getChoice() {
    int c;
    printf("Enter choice: ");
    scanf("%d", &c);
    return c;
}