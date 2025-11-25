#include <stdio.h>
#include "processing.h"
#include "common.h"

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