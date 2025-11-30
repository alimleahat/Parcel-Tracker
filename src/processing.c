#include <stdio.h>
#include <string.h>
#include "processing.h"
#include "common.h"
#include <time.h>
#include "orders.h"

DeliveryService depots[20];
int depotCount = 0;

// Loads depot data from file

void loadDepots() {
    FILE *f = fopen("data/depots.txt", "r");
    if (!f) {
        printf("No depots file found.");
        return;
    }

    depotCount = 0;
    while (fscanf(
        f,
        "%d %49s %f %f %f %f",
        &depots[depotCount].depotID,
        depots[depotCount].name,
        &depots[depotCount].depotDistance,
        &depots[depotCount].baseRate,
        &depots[depotCount].ratePerKm,
        &depots[depotCount].ratePerKg
    ) == 6)
    {
        depotCount++;
    }

    fclose(f);
}

// Converts Courier ID to it's restpective name
const char* getCourierName(int id) {
    for (int i = 0; i < depotCount; i++) {
        if (depots[i].depotID == id)
            return depots[i].name;
    }
    return "Unknown";
}// Simple Delivery Cost Calculation Based on Rates, Distance and Weight of the product
float calculateCost(float weight, int courierID) {
    for (int i = 0; i < depotCount; i++) {
        if (depots[i].depotID == courierID) {

            float base = depots[i].baseRate;
            float distanceCost = depots[i].depotDistance * depots[i].ratePerKm;
            float weightCost = weight * depots[i].ratePerKg;

            return base + distanceCost + weightCost;
        }
    }

    return 0.0f;  // fallback
}

// Gets Current System Time and converts to struct to compare with delivery time
char* getCurrentTimestamp() {
    static char buffer[40];   // increased size

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    snprintf(buffer, sizeof(buffer),
             "%04d-%02d-%02d_%02d:%02d",
             t->tm_year + 1900,
             t->tm_mon + 1,
             t->tm_mday,
             t->tm_hour,
             t->tm_min);

    return buffer;
}

// Convert Delivery time to struct tm

void parseDeliveryTime(const char *deliveryStr, struct tm *t) {
    // Format: YYYY-MM-DD_HH:MM
    sscanf(deliveryStr, "%d-%d-%d_%d:%d",
           &t->tm_year,
           &t->tm_mon,
           &t->tm_mday,
           &t->tm_hour,
           &t->tm_min);

    t->tm_year -= 1900;   
    t->tm_mon -= 1;       
    t->tm_sec = 0;
    t->tm_isdst = -1;     
}

// Compares System time with delivery time to get a remaining time string and status (Delivered or Not)
void getTimeRemaining(const char *deliveryStr, char *output, int *status) {
    time_t now = time(NULL);
    struct tm delivery = {0};

    parseDeliveryTime(deliveryStr, &delivery);

    time_t deliveryTime = mktime(&delivery);

    double diff = difftime(deliveryTime, now);

    if (diff <= 0) {
        // Delivery time has passed
        strcpy(output, "Delivered");
        *status = 1;   // mark delivered
        return;
    }

    int seconds = diff;
    int days = seconds / 86400;
    seconds %= 86400;
    int hours = seconds / 3600;

    if (days == 0 && hours < 1) {
        strcpy(output, "Less than an hour");
    } else {
        sprintf(output, "%d days %d hours", days, hours);
    }

    *status = 0;
}

// Calculates time since delivery for delivered orders
void getTimeSinceDelivery(const char *deliveryStr, char *output) {
    time_t now = time(NULL);
    struct tm delivery = {0};

    parseDeliveryTime(deliveryStr, &delivery);

    time_t deliveryTime = mktime(&delivery);

    double diff = difftime(now, deliveryTime);

    if (diff < 0) { // Safety check
        strcpy(output, "Not delivered");
        return;
    }

    int seconds = (int)diff;
    int days = seconds / 86400;
    seconds %= 86400;
    int hours = seconds / 3600;

    if (days == 0 && hours < 1) {
        strcpy(output, "Less than an hour ago");
    } else {
        sprintf(output, "%d days %d hours ago", days, hours);
    }
}

// 
time_t convertToTimestamp(const char *deliveryStr) {
    struct tm t = {0};
    parseDeliveryTime(deliveryStr, &t);
    return mktime(&t);
}

// Used in multiple places to validate integer inputs within a specified range 
int getIntInRange(const char *prompt, int min, int max) {
    int value;
    int valid = 0;

    while (!valid) {
        printf("%s", prompt);

        if (scanf("%d", &value) == 1) {
            if (value >= min && value <= max) {
                valid = 1;
            } else {
                printf("Input must be between %d and %d.\n", min, max);
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
        }

        while (getchar() != '\n'); 
    }

    return value;
}
