/*
 * tests.c
 * Lightweight console testing file for the shared processing helpers.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "processing.h"
#include "common.h"


#define ASSERT_TRUE(cond, msg) \
    if (cond) printf("[PASS] %s\n", msg); \
    else      printf("[FAIL] %s\n", msg);

#define ASSERT_FLOAT_EQ(a, b, tol, msg) \
    if (((a) > (b) ? (a)-(b) : (b)-(a)) < tol) printf("[PASS] %s\n", msg); \
    else                                      printf("[FAIL] %s\n", msg);

// Test Cost Calculation Function
void test_costCalculation() {

    loadDepots();
    printf("\nTest 1: Cost Calculation\n");

    float cost1 = calculateCost(2.0, 1);  // courier 1 (FadEx)
    ASSERT_TRUE(cost1 > 0, "Cost for weight 2.0 courier 1 should be positive");

    float cost2 = calculateCost(0.5, 3);  // courier 3 (DLH)
    ASSERT_TRUE(cost2 > 0, "Cost for weight 0.5 courier 3 should be positive");

}

// Test Date Parsing Function
void test_parseDeliveryTime() {
    printf("\nTest 2: Date Parsing\n");

    struct tm t;
    parseDeliveryTime("2025-12-01_14:30", &t);

    ASSERT_TRUE(t.tm_year == 125, "Year parsed correctly");
    ASSERT_TRUE(t.tm_mon == 11, "Month parsed correctly");
    ASSERT_TRUE(t.tm_mday == 1, "Day parsed correctly");
    ASSERT_TRUE(t.tm_hour == 14, "Hour parsed correctly");
    ASSERT_TRUE(t.tm_min == 30, "Minute parsed correctly");
}

// Test Time Remaining Function
void test_timeRemaining() {
    printf("\nTest 3: Time Remaining\n");

    char buf[50];
    int status;

    // date far in the future
    getTimeRemaining("2099-01-01_00:00", buf, &status);
    ASSERT_TRUE(status == 0, "Future delivery should have status 0");
}


// Test Time Since Delivery Function
void test_timeSinceDelivery() {
    printf("\nTest 4: Time Since Delivery\n");

    char buf[50];
    getTimeSinceDelivery("2000-01-01_00:00", buf);

    ASSERT_TRUE(strstr(buf, "ago") != NULL, "Delivered time should include 'ago'");
}


int main() {

    printf("     PACKAGE TRACKER TESTS  \n");

    test_costCalculation();
    test_parseDeliveryTime();
    test_timeRemaining();
    test_timeSinceDelivery();

    printf("\nAll tests finished.\n\n");
    return 0;
}
