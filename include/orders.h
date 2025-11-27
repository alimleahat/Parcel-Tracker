#ifndef ORDERS_H
#define ORDERS_H

extern int orderCount;
void loadOrders();
void saveOrders();
void addOrder();
void searchOrder();
void deliveredOrders();
void currentOrders();
void syncDeliveredOrders();
void saveDelivered();
void sortCurrentOrdersByDeliveryTime();
void sortDeliveredItems();
void statisticsMenu();
void showBasicStats();
void showCourierBarChart();


#endif