#include <stdio.h>
#include "menu.h"
#include "processing.h"

void showMenu() {
    printf("\n\n1. Current Orders\n");
    printf("2. Delivered Orders\n");
    printf("3. Add Order\n");
    printf("4. Search Order\n");
    printf("5. Statistics\n");
    printf("0. Exit\n");
    printf("Current time: %s\n", getCurrentTimestamp());

}

int getChoice() {
    int c;
    printf("Enter choice: ");
    scanf("%d", &c);
    return c;
}