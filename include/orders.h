#ifndef ORDERS_H
#define ORDERS_H
#include "common.h"

extern int orderCount;

typedef struct {
    int id;
    char name[50];
    float weight;
    char timeStr[20];
    float cost;
    int courier;
    int status;
} DeliveredItem;


void loadOrders();
void saveOrders();
void addOrder();
void searchOrder();
void deliveredOrders();
void currentOrders();
void syncDeliveredOrders();
void saveDelivered();
void sortCurrentOrdersByDeliveryTime();
void sortDeliveredItems(DeliveredItem *arr, int n);
void statisticsMenu();
void showBasicStats();
void showCourierBarChart();


#endif