#ifndef COMMON_H
#define COMMON_H

// Definition of Order Data Structure for Current Orders
typedef struct {
    int packageID;
    char name[50];
    float weight;
    char deliverytime[20];   // format: YYYY-MM-DD_HH:MM
    int status;               // 0 = not delivered, 1 = delivered
    float cost;
    int courier;            // courier ID 1-5
} Order;

// Definition of Delivery Service Data Structure
typedef struct {
    int depotID;            // depot ID 1-5
    char name[50];
    float depotDistance;   // km
    float baseRate;        // base £
    float ratePerKm;       // £ per km
    float ratePerKg;       // £ per kg
} DeliveryService;

#endif