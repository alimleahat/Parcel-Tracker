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
        o.cost = calculateCost(o.weight, o.courier);
        printf("\nCalculated Cost: £%.2f\n", o.cost);

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
    int id;
    printf("Enter Package ID to search: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int found = 0;

    // 1. Search ACTIVE orders (in memory)

    for (int i = 0; i < orderCount; i++) {
        if (orders[i].packageID == id) {
            found = 1;

            printf("\n--- Order Found ---\n");
            printf("%-10s %-15s %-8s %-20s %-15s %-8s %-12s\n",
                   "ID", "Name", "Weight", "Delivery Time",
                   "Status", "Cost", "Courier");

            printf("%-10d %-15s %-8.2f %-20s %-15s %-8.2f %-12s\n",
                   orders[i].packageID,
                   orders[i].name,
                   orders[i].weight,
                   orders[i].deliverytime,
                   "NOT DELIVERED",
                   orders[i].cost,
                   getCourierName(orders[i].courier));

            goto endSearch;
        }
    }

    // 2. Search DELIVERED orders (delivered.txt)

    FILE *f = fopen("data/history.txt", "r");
    if (f) {
        int id2, courier;
        char name[50], time[20];
        float weight, cost;

        while (fscanf(f, "%d %49s %f %19s %f %d",
                      &id2, name, &weight, time,
                      &cost, &courier) == 6)
        {
            if (id2 == id) {
                found = 1;

                printf("\n--- Order Found ---\n");
                printf("%-10s %-15s %-8s %-20s %-15s %-8s %-12s\n",
                       "ID", "Name", "Weight", "Delivery Time",
                       "Status", "Cost", "Courier");

                printf("%-10d %-15s %-8.2f %-20s %-15s %-8.2f %-12s\n",
                       id2, name, weight, time,
                       "DELIVERED",
                       cost,
                       getCourierName(courier));

                fclose(f);
                goto endSearch;
            }
        }

        fclose(f);
    }

endSearch:
    if (!found) {
        printf("\nNo order with ID %d found.\n", id);
    }

    int back = -1;
    while (back != 0) {
        printf("\nEnter 0 to return to main menu: ");
        scanf("%d", &back);
        while (getchar() != '\n');
    }
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
    saveOrders(); // rewrite orders.txt with only active orders
}

void deliveredOrders() {

    // Step 1: Auto-sync delivered orders
    syncDeliveredOrders();

    // Step 2: Open delivered.txt and display everything
    FILE *f = fopen("data/history.txt", "r");
    if (!f) {
        printf("No delivered orders found.\n");
    } else {

        printf("\n--- Delivered Orders ---\n");
        printf("%-10s %-15s %-8s %-20s %-8s %-12s\n",
               "ID", "Name", "Weight", "Delivery Time", "Cost", "Courier");

        int id, courier, status;
        char name[50], time[20];
        float weight, cost;

        // Read each delivered entry
        while (fscanf(f, "%d %49s %f %19s %d %f %d",
                      &id, name, &weight, time, &status, &cost, &courier) == 7)
        {
            printf("%-10d %-15s %-8.2f %-20s %-8.2f %-12s\n",
                   id, name, weight, time, cost, getCourierName(courier));
        }

        fclose(f);
    }

    // Step 3: Pause and return
    int back = -1;
    while (back != 0) {
        printf("\nEnter 0 to return to main menu: ");
        scanf("%d", &back);
        while (getchar() != '\n');  // Clear buffer
    }
}

