#ifndef PROCESSING_H
#define PROCESSING_H

void processOrder();
void loadDepots();
const char* getCourierName(int id);
float calculateCost(float weight, int courierID);
char* getCurrentTimestamp();


#endif