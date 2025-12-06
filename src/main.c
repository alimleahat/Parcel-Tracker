/*
 * main.c
 * Entry point for the Parcel Tracker CLI. Loads persisted data and
 * coordinates the top-level menu loop for the rest of the application.
 */
#include <stdio.h>
#include "menu.h"
#include "orders.h"
#include "processing.h"

// Infinite loop, loops until the user exits.
int main() {
    loadDepots();
    loadOrders();
    
    int running = 1;

    while (running) {
        printf("\033[3J\033[2J\033[1;1H"); // Clear terminal
        
        showMenu();
        int choice = getIntInRange("Enter choice (0–5): ", 0, 5);

        switch (choice) {
            case 1: currentOrders(); break;
            case 2: deliveredOrders(); break;
            case 3: addOrder(); saveOrders(); break;   // auto-save
            case 4: searchOrder(); break;
            case 5: statisticsMenu(); break;
            case 0: saveOrders(); running = 0; break;   // auto-save on exit
            default: printf("Invalid."); break;
        }
    }

    return 0;
}
