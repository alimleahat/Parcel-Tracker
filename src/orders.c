#include <stdio.h>
#include <string.h>
#include "orders.h"
#include "common.h"

Order orders[100];
int orderCount = 0;

void orderHistoryMenu() {
    int back = 0;

    while (!back) {
        printf("\n--- Order History Menu ---\n");
        printf("Delivered Orders Placeholder\n");
        printf("0. Back\n");

        int choice;
        printf("Enter choice: ");
        scanf("%d", &choice);

        }
    }

void loadOrders() {
    FILE *f = fopen("data/orders.txt", "r");
    if (!f) {
        printf("No orders file found.");
        return;
    }

    orderCount = 0;
    while (fscanf(
        f,
        "%d %49s %f %19s %d %f %d",
        &orders[orderCount].packageID,
        orders[orderCount].name,
        &orders[orderCount].weight,
        orders[orderCount].deliverytime,
        &orders[orderCount].status,
        &orders[orderCount].cost,
        &orders[orderCount].courier
    ) == 7)
    {
        orderCount++;
    }

    fclose(f);
    printf("Loaded %d orders.", orderCount);
}

void saveOrders() {
    FILE *f = fopen("data/orders.txt", "w");
    if (!f) {
        printf("Error saving.");
        return;
    }

    for (int i = 0; i < orderCount; i++) {
        fprintf(
            f,
            "%d %s %.2f %s %d %.2f %d",
            orders[i].packageID,
            orders[i].name,
            orders[i].weight,
            orders[i].deliverytime,
            orders[i].status,
            orders[i].cost,
            orders[i].courier
        );
    }

    fclose(f);
}

void addOrder() {
    int again = 1;

    while (again) {
        Order o;

        printf("Enter package name: ");
        scanf("%49s", o.name);
        while (getchar() != '\n');

        printf("Enter package ID: ");
        scanf("%d", &o.packageID);
        while (getchar() != '\n');

        printf("Enter delivery time (YYYY-MM-DD_HH:MM): ");
        scanf("%19s", o.deliverytime);
        while (getchar() != '\n');

        printf("Enter weight (kg): ");
        scanf("%f", &o.weight);
        while (getchar() != '\n');

        printf("1.FadEx\n2.USP\n3.DLH\n4.Royal Mile\n5.PDP\nSelect courier (1-5): ");
        scanf("%d", &o.courier);
        while (getchar() != '\n');

        o.status = 0;
        o.cost = 0.0f;

        orders[orderCount++] = o;

        printf("\nOrder added!\n");
        printf("\n1. Add another order\n");
        printf("0. Return to main menu\n");
        printf("Enter choice: ");

        scanf("%d", &again);
        while (getchar() != '\n');

        if (again != 1) {
                again = 0; // exit loop
        }
    }
}

void searchOrder() {
    printf("[searchOrder Placeholder]");
}