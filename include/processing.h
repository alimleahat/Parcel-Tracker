#ifndef PROCESSING_H
#define PROCESSING_H
#include <time.h>

void processOrder();
void loadDepots();
const char* getCourierName(int id);
float calculateCost(float weight, int courierID);
char* getCurrentTimestamp();
void parseDeliveryTime(const char *deliveryStr, struct tm *t);
void getTimeRemaining(const char *deliveryStr, char *output, int *status);
void getTimeSinceDelivery(const char *deliveryStr, char *output);
time_t convertToTimestamp(const char *deliveryStr);


#endif