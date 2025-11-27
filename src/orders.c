#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "orders.h"
#include "common.h"
#include "processing.h"

Order orders[100];
int orderCount = 0;
typedef struct {
    int id;
    char name[50];
    float weight;
    char timeStr[20];
    float cost;
    int courier;
    int status;
} DeliveredItem;



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

            // ---- DELIVERY DATE ----
        int day, month, year, hour, minute;

        printf("Enter delivery date (DD/MM/YYYY): ");
        scanf("%d/%d/%d", &day, &month, &year);
        while (getchar() != '\n');

        printf("Enter delivery time (HH:MM): ");
        scanf("%d:%d", &hour, &minute);
        while (getchar() != '\n');


        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2025 ||
            hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            printf("Invalid date or time. Please try again.\n");
            continue;   // restarts add-order loop
        }

        // ---- INTERNAL STORAGE FORMAT ----
        sprintf(o.deliverytime, "%04d-%02d-%02d_%02d:%02d",
                year, month, day, hour, minute);

        o.weight = getIntInRange("Enter weight (kg): ", 0, 999);
        while (getchar() != '\n');

        o.courier = getIntInRange("1.FadEx\n2.USP\n3.DLH\n4.Royal Mile\n5.PDP\nSelect courier (1-5): ", 1, 5);
        while (getchar() != '\n');

        o.status = 0;
        o.cost = calculateCost(o.weight, o.courier);
        printf("\nCalculated Cost: £%.2f\n", o.cost);

        orders[orderCount++] = o;

        printf("\nOrder added!\n");
        printf("\n1. Add another order\n");
        printf("0. Return to main menu\n");
        printf("Enter choice: ");

        
        again = getIntInRange("Enter choice: ", 0, 1);
        while (getchar() != '\n');

        if (again != 1) {
                again = 0; // exit loop
        }
    }
}

void currentOrders() {

    sortCurrentOrdersByDeliveryTime();
    #ifdef _WIN32
    system("cls");
     #else
    system("clear");
    #endif


    if (orderCount == 0) {
        printf("\nNo orders found.\n");
    } else {

        printf("\n---------------------------- Current Orders ----------------------------\n\n");

        printf("%-10s %-15s %-8s %-20s %-8s %-12s\n",
               "ID", "Name", "Weight", "Time Left", "Cost", "Courier");

        for (int i = 0; i < orderCount; i++) {

            char timeLeft[50];
            int newStatus;

            getTimeRemaining(orders[i].deliverytime, timeLeft, &newStatus);

            // auto-mark delivered
            if (newStatus == 1 && orders[i].status == 0) {
                orders[i].status = 1;
            }

            printf("%-10d %-15s %-8.2f %-20s %-8.2f %-12s\n",
                   orders[i].packageID,
                   orders[i].name,
                   orders[i].weight,
                   timeLeft,
                   orders[i].cost,
                   getCourierName(orders[i].courier));
        }
    }


    getIntInRange("\nEnter 0 to return: ", 0, 0);
    return;
}

void searchOrder() {

    #ifdef _WIN32
    system("cls");
     #else
    system("clear");
    #endif
    
    int id;
    printf("Enter Package ID to search: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int found = 0;

    // ------------------------------------
    // 1. Search ACTIVE orders (in memory)
    // ------------------------------------
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].packageID == id) {

            found = 1;

            char timeLeft[50];
            int newStatus;

            // Calculate remaining time
            getTimeRemaining(orders[i].deliverytime, timeLeft, &newStatus);

            // Auto-update status if overdue
            if (newStatus == 1 && orders[i].status == 0) {
                orders[i].status = 1;
                saveOrders();
            }

            printf("\n--------------------------------- Order Found ---------------------------------\n\n");
            printf("%-10s %-15s %-8s %-25s %-8s %-12s\n",
                   "ID", "Name", "Weight", "Time Left",
                   "Cost", "Courier");

            printf("%-10d %-15s %-8.2f %-25s %-8.2f %-12s\n",
                   orders[i].packageID,
                   orders[i].name,
                   orders[i].weight,
                   timeLeft,                     // ✔ No status shown
                   orders[i].cost,
                   getCourierName(orders[i].courier));

            goto endSearch;
        }
    }


    // ------------------------------------
    // 2. Search DELIVERED orders (history)
    // ------------------------------------
    FILE *f = fopen("data/history.txt", "r");
    if (f) {
        int id2, courier, status;
        char name[50], timeStr[20];
        float weight, cost;

        while (fscanf(f, "%d %49s %f %19s %d %f %d",
                      &id2, name, &weight, timeStr,
                      &status, &cost, &courier) == 7)
        {
            if (id2 == id) {

                found = 1;

                // Calculate "delivered X days ago"
                char ago[50];
                getTimeSinceDelivery(timeStr, ago);

                printf("\n--------------------------------- Order Found ---------------------------------\n\n");
                printf("%-10s %-15s %-8s %-25s %-8s %-12s\n",
                       "ID", "Name", "Weight", "Delivered",
                       "Cost", "Courier");

                printf("%-10d %-15s %-8.2f %-25s %-8.2f %-12s\n",
                       id2,
                       name,
                       weight,
                       ago,                    // ✔ New field
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

    getIntInRange("\nEnter 0 to return: ", 0, 0);
    return;
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

    #ifdef _WIN32
    system("cls");
     #else
    system("clear");
    #endif
    syncDeliveredOrders();
    

    FILE *f = fopen("data/history.txt", "r");
    if (!f) {
        printf("\nNo delivered orders found.\n");
    } else {

        DeliveredItem arr[200];
        int count = 0;

        int id, courier, status;
        char name[50], timeStr[20];
        float weight, cost;

        // load delivered items
        while (fscanf(f, "%d %49s %f %19s %d %f %d",
                      &id, name, &weight, timeStr,
                      &status, &cost, &courier) == 7)
        {
            arr[count].id = id;
            strcpy(arr[count].name, name);
            arr[count].weight = weight;
            strcpy(arr[count].timeStr, timeStr);
            arr[count].cost = cost;
            arr[count].courier = courier;
            arr[count].status = status;
            count++;
        }
        fclose(f);

        printf("\n------------------------------ Delivered Orders ------------------------------\n\n");

        printf("%-10s %-15s %-8s %-25s %-8s %-12s\n",
               "ID", "Name", "Weight", "Delivered", "Cost", "Courier");

        for (int i = 0; i < count; i++) {
            char ago[50];
            getTimeSinceDelivery(arr[i].timeStr, ago);

            printf("%-10d %-15s %-8.2f %-25s %-8.2f %-12s\n",
                   arr[i].id,
                   arr[i].name,
                   arr[i].weight,
                   ago,
                   arr[i].cost,
                   getCourierName(arr[i].courier));
        }

    int sortChoice;

    sortChoice = getIntInRange("Press 1 to sort by delivery time or 0 to return: ", 0, 1);

    if (sortChoice == 0) {
        return;
    }

    if (sortChoice == 1) {

        sortDeliveredItems(arr, count);

        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n------------------- Delivered Orders (Sorted Latest First) -------------------\n\n");
        printf("%-10s %-15s %-8s %-25s %-8s %-12s\n",
            "ID", "Name", "Weight", "Delivered", "Cost", "Courier");

        for (int i = 0; i < count; i++) {
            char ago[50];
            getTimeSinceDelivery(arr[i].timeStr, ago);

            printf("%-10d %-15s %-8.2f %-25s %-8.2f %-12s\n",
                arr[i].id,
                arr[i].name,
                arr[i].weight,
                ago,
                arr[i].cost,
                getCourierName(arr[i].courier));
        }
    }

    getIntInRange("\nEnter 0 to return: ", 0, 0);
    return;
    }
}

void sortCurrentOrdersByDeliveryTime() {
    for (int i = 0; i < orderCount - 1; i++) {
        for (int j = 0; j < orderCount - i - 1; j++) {

            time_t t1 = convertToTimestamp(orders[j].deliverytime);
            time_t t2 = convertToTimestamp(orders[j+1].deliverytime);

            if (t1 > t2) {
                Order temp = orders[j];
                orders[j] = orders[j+1];
                orders[j+1] = temp;
            }
        }
    }
}


void sortDeliveredItems(DeliveredItem *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            time_t t1 = convertToTimestamp(arr[j].timeStr);
            time_t t2 = convertToTimestamp(arr[j+1].timeStr);

            // Delivered: latest FIRST
            if (t1 < t2) {
                DeliveredItem temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void statisticsMenu() {

    int choice = -1;

    while (choice != 0) {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif

        printf("\n");
        printf("──────────────────────────────────────────\n");
        printf("            📊 STATISTICS MENU          \n");
        printf("──────────────────────────────────────────\n\n");

        printf("   1) 📈 View Statistics\n");
        printf("   2) 📦 Orders Per Courier (Bar Chart)\n");
        printf("   0) Back\n\n");

        printf("──────────────────────────────────────────\n");

        choice = getIntInRange("Enter choice: ", 0, 2);

        if (choice == 1) {
            showBasicStats();
        } else if (choice == 2) {
            showCourierBarChart();
        }
    }
}

void showBasicStats() {

    int activeCount = orderCount;
    int deliveredCount = 0;
    float totalActiveCost = 0;
    float totalDeliveredCost = 0;
    float totalWeight = 0;

    // Count active
    for (int i = 0; i < orderCount; i++) {
        totalActiveCost += orders[i].cost;
        totalWeight += orders[i].weight;
    }

    // Count delivered
    FILE *f = fopen("data/history.txt", "r");
    if (f) {
        int id, courier, status;
        char name[50], timeStr[20];
        float weight, cost;

        while (fscanf(f, "%d %49s %f %19s %d %f %d",
                      &id, name, &weight, timeStr,
                      &status, &cost, &courier) == 7)
        {
            deliveredCount++;
            totalDeliveredCost += cost;
            totalWeight += weight;
        }
        fclose(f);
    }
    #ifdef _WIN32
    system("cls");
     #else
    system("clear");
    #endif

    printf("\n");
    printf("──────────────────────────────────────────────\n");
    printf("             📊  STATISTICS\n");
    printf("──────────────────────────────────────────────\n\n");

    printf("   📦 Active Orders:              %d\n", activeCount);
    printf("   ✔️  Delivered Orders:           %d\n", deliveredCount);
    printf("   🧾 Total Orders:               %d\n", activeCount + deliveredCount);
    printf("   💷 Total Cost (Active):        £%.2f\n", totalActiveCost);
    printf("   💰 Total Cost (Delivered):     £%.2f\n", totalDeliveredCost);
    printf("   ⚖️  Avg Order Weight:           %.2f kg\n",
        (activeCount + deliveredCount > 0)
        ? totalWeight / (activeCount + deliveredCount)
        : 0);

    printf("\n──────────────────────────────────────────────\n\n");

    getIntInRange("\nEnter 0 to return: ", 0, 0);
    return;
}
void showCourierBarChart() {

    int courierCount[6] = {0};

    // Count active orders
    for (int i = 0; i < orderCount; i++) {
        courierCount[orders[i].courier]++;
    }

    // Count delivered orders
    FILE *f = fopen("data/history.txt", "r");
    if (f) {
        int id, courier, status;
        char name[50], timeStr[20];
        float weight, cost;

        while (fscanf(f, "%d %49s %f %19s %d %f %d",
                      &id, name, &weight, timeStr,
                      &status, &cost, &courier) == 7)
        {
            courierCount[courier]++;
        }
        fclose(f);
    }

    #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif

    printf("\n");
    printf("──────────────────────────────────────────────\n");
    printf("        📦 ORDERS PER COURIER (BAR CHART)\n");
    printf("──────────────────────────────────────────────\n\n");

    for (int c = 1; c <= 5; c++) {
        printf("   %-15s │ ", getCourierName(c));   // aligned label
        
        // Print bar graph
        for (int i = 0; i < courierCount[c]; i++) {
            printf("█");
        }

        printf(" (%d)\n", courierCount[c]);  // numeric count at end
    }

    printf("\n──────────────────────────────────────────────\n");

    getIntInRange("\nEnter 0 to return: ", 0, 0);
    return;
}
