#ifndef COMMON_H
#define COMMON_H

//Common defenitions

typedef struct {
    int packageID;
    char name[50];
    float weight;
    char deliverytime;
    int status;   // 0 = no, 1 = yes
    float cost;
    
} Order;

#endif
