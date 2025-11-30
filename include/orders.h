#ifndef ORDERS_H
#define ORDERS_H
#include "common.h"

extern int orderCount;

// Delivered orders Data Structure, with delivery time as string
typedef struct {
    int id;
    char name[50];
    float weight;
    char timeStr[20];
    float cost;
    int courier;
    int status;
} DeliveredItem;

// Order Storage
void loadOrders();
void saveOrders();
void saveDelivered();

// Menu Functions
void addOrder();
void currentOrders();
void searchOrder();
void deliveredOrders();
void statisticsMenu();

//Sync and Sorting Functions
void syncDeliveredOrders();
void sortCurrentOrdersByDeliveryTime();
void sortDeliveredItems(DeliveredItem *arr, int n);

// Statistics Functions
void showBasicStats();
void showCourierBarChart();


#endif