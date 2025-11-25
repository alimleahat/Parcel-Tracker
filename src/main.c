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
            case 2: orderHistoryMenu(); break;
            case 3: addOrder(); saveOrders(); break;   // auto-save
            case 4: searchOrder(); break;
            case 5: printf("[Statistics Placeholder]"); break;
            case 0: saveOrders(); running = 0; break;   // auto-save on exit
            default: printf("Invalid."); break;
        }
    }

    return 0;
}