#include <stdio.h>
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

