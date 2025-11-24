#include <stdio.h>
#include "menu.h"
#include "orders.h"

// Main: Controls the program execution flow & handles user interactions.

int main() {
    
    int running = 1;

    while (1) {
        showMenu();
        int choice = getChoice();

        if (choice == 1) {
            loadOrders();   // placeholder
        } else if (choice == 0) {
            running = 0;
        } else {
            printf("Invalid.\n");
        }
    }

    return 0;
}