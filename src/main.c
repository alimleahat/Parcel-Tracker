#include <stdio.h>
#include "menu.h"
#include "orders.h"

int main() {
    int running = 1;

    while (running) {
        showMenu();
        int choice = getChoice();

        switch (choice) {
            case 1: loadOrders(); break;
            case 2: saveOrders(); break;
            case 3: addOrder(); break;
            case 4: searchOrder(); break;
            case 0: running = 0; break;
            default: printf("Invalid."); break;
        }
    }

    return 0;
}