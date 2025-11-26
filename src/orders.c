#include <stdio.h>
#include <string.h>
#include "orders.h"
#include "common.h"
#include "processing.h"

Order orders[100];
int orderCount = 0;


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
            "%d %s %.2f %s %d %.2f %d\n",
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

void currentOrders() {

    if (orderCount == 0) {
        printf("\nNo orders found.\n");
    } else {

        printf("\n--- Current Orders ---\n");
        printf("%-10s %-15s %-8s %-20s %-8s %-12s\n",
                "ID", "Name", "Weight", "Delivery Time", "Cost", "Courier");

        for (int i = 0; i < orderCount; i++) {
            printf("%-10d %-15s %-8.2f %-20s %-8.2f %-12s\n",
                   orders[i].packageID,
                   orders[i].name,
                   orders[i].weight,
                   orders[i].deliverytime,
                   orders[i].cost,
                   getCourierName(orders[i].courier));
        }
    }

    // Return to main menu prompt

    int back = -1;

    while (back != 0) {
        printf("\nEnter 0 to return to the main menu: ");
        scanf("%d", &back);
        while (getchar() != '\n'); // clear buffer
    }

    // user returns to main menu after this function ends
}

void searchOrder() {
    printf("[searchOrder Placeholder]");
}

void syncDeliveredOrders() {

    // Append delivered orders to delivered.txt
    FILE *f = fopen("data/history.txt", "a");
    if (!f) {
        printf("Error opening delivered file.\n");
        return;
    }

    for (int i = 0; i < orderCount; i++) {
        if (orders[i].status == 1) {

            // Write to delivered.txt
            fprintf(f, "%d %s %.2f %s %d %.2f %d\n",
                orders[i].packageID,
                orders[i].name,
                orders[i].weight,
                orders[i].deliverytime,
                orders[i].status,
                orders[i].cost,
                orders[i].courier
            );
        }
    }

    fclose(f);

    // Remove delivered orders from active list
    int j = 0;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].status == 0) {
            orders[j++] = orders[i];
        }
    }

    orderCount = j;
    saveOrders();     // rewrite orders.txt with only active orders
}

void deliveredOrders() {

    // Automatically move delivered items from orders[] to delivered.txt
    syncDeliveredOrders();

    int back = 0;

    while (!back) {
        printf("\n--- Delivered Orders Menu ---\n");
        printf("1. View Delivered Orders\n");
        printf("0. Back\n");

        int c;
        printf("Enter choice: ");
        scanf("%d", &c);
        while (getchar() != '\n');

        if (c == 1) {
            deliveredOrders();
        }
        else if (c == 0) {
            back = 1;
        }
        else {
            printf("Invalid.\n");
        }
    }
}
