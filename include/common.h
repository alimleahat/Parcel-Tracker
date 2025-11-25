#ifndef COMMON_H
#define COMMON_H

typedef struct {
    int packageID;
    char name[50];
    float weight;
    char deliverytime[20];   // format: YYYY-MM-DD_HH:MM
    int status;               // 0 = not delivered, 1 = delivered
    float cost;
} Order;

typedef struct {
    int depotID;
    char name[50];
    float depotDistance;   // km
    float baseRate;        // base £
    float ratePerKm;       // £ per km
    float ratePerKg;       // £ per kg
} DeliveryService;

#endif