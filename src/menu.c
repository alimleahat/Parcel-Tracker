#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "processing.h"
#include "orders.h"

void showMenu() {

    #ifdef _WIN32
    system("cls");
     #else
    system("clear");
    #endif

    printf("─────────────────────────────────────────────\n");
    printf("        📦  PACKAGE DELIVERY TRACKER\n");
    printf("─────────────────────────────────────────────\n");
    printf("   🕒  %s\n\n", getCurrentTimestamp());

    printf("   1) 📄 View Current Orders\n");
    printf("   2) ✔️  Delivered Orders\n");
    printf("   3) ➕  Add New Order\n");
    printf("   4) 🔍 Search Order\n");
    printf("   5) 📊 Statistics\n");
    printf("   0) 🚪 Exit\n\n");

    printf("─────────────────────────────────────────────\n");
    printf("Active Orders: %d \n", orderCount);
    printf("─────────────────────────────────────────────\n\n");
}

int getChoice() {
    int c;
    c = getIntInRange("👉 Enter choice (0–5): ", 0, 5);
    return c;
}
