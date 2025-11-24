#include <stdio.h>
#include "orders.h"
#include "common.h"

// Orders: Manages adding/loading/saving/searching orders.

Order orders[100];
int orderCount = 0;

void load_order() {
    FILE *f = fopen("data/orders.txt", "r");
    if (!f) {
        printf("No orders file.\n");
        return;
    }

    orderCount = 0;
    while (fscanf(f, "%d %f %d",
        &orders[orderCount].packageID,
        &orders[orderCount].weight,
        &orders[orderCount].delivered) == 3)
    {
        orderCount++;
    }

    fclose(f);
    printf("Loaded %d orders.\n", orderCount);
}

void save_order() {
    FILE *f = fopen("data/orders.txt", "w");
    if (!f) return;

    for (int i = 0; i < orderCount; i++) {
        fprintf(f, "%d %.2f %d\n",
            orders[i].packageID,
            orders[i].weight,
            orders[i].delivered);
    }

    fclose(f);
}

void add_order() {
    printf("Order Added\n");
}

void search_order() {
    printf("Order Searched\n");
}
