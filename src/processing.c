#include <stdio.h>
#include <string.h>
#include "processing.h"
#include "common.h"
#include <time.h>

DeliveryService depots[20];
int depotCount = 0;

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
    printf("Loaded %d depots.", depotCount);
}

void processOrder() {
    printf("[processOrder Placeholder]");
}

const char* getCourierName(int id) {
    for (int i = 0; i < depotCount; i++) {
        if (depots[i].depotID == id)
            return depots[i].name;
    }
    return "Unknown";
}

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

void parseDeliveryTime(const char *deliveryStr, struct tm *t) {
    // Example format: 2025-11-28_14:30
    sscanf(deliveryStr, "%d-%d-%d_%d:%d",
           &t->tm_year,
           &t->tm_mon,
           &t->tm_mday,
           &t->tm_hour,
           &t->tm_min);

    t->tm_year -= 1900;   // tm_year is years since 1900
    t->tm_mon -= 1;       // tm_mon is 0–11
    t->tm_sec = 0;
    t->tm_isdst = -1;     // let system determine
}

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

void getTimeSinceDelivery(const char *deliveryStr, char *output) {
    time_t now = time(NULL);
    struct tm delivery = {0};

    parseDeliveryTime(deliveryStr, &delivery);

    time_t deliveryTime = mktime(&delivery);

    double diff = difftime(now, deliveryTime);

    if (diff < 0) {
        // delivery time in the future (should never happen here)
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
