#ifndef PROCESSING_H
#define PROCESSING_H
#include <time.h>

// Depot & Courier Functions
void loadDepots();
float calculateCost(float weight, int courierID);
const char* getCourierName(int id);

// Time Functions
char* getCurrentTimestamp();
void parseDeliveryTime(const char *deliveryStr, struct tm *t);
void getTimeRemaining(const char *deliveryStr, char *output, int *status);
void getTimeSinceDelivery(const char *deliveryStr, char *output);
time_t convertToTimestamp(const char *deliveryStr);

// Input Function
int getIntInRange(const char *prompt, int min, int max);



#endif