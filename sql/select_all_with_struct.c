#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARS 100 // Maximum cars we can store

typedef struct {
    int id;
    char name[50];
    int price;
} Car;

typedef struct {
    Car cars[MAX_CARS];
    int count;
} CarList;

int storeCars(void *data, int argc, char **argv, char **azColName);

int main(void) {
    sqlite3 *db;
    char *err_msg = 0;
    CarList carList = {.count = 0}; // Initialize CarList

    int rc = sqlite3_open("cars.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *sql = "SELECT * FROM Cars;";
    rc = sqlite3_exec(db, sql, storeCars, &carList, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);

    // Print all cars after fetching data
    printf("=== Car List ===\n");
    for (int i = 0; i < carList.count; i++) {
        printf("Car ID: %d\n", carList.cars[i].id);
        printf("Car Name: %s\n", carList.cars[i].name);
        printf("Car Price: %d\n", carList.cars[i].price);
        printf("----------------------\n");
    }

    return 0;
}

// Callback function to store rows in CarList
int storeCars(void *data, int argc, char **argv, char **azColName) {
    CarList *carList = (CarList *)data;

    if (carList->count >= MAX_CARS) {
        fprintf(stderr, "Max car limit reached!\n");
        return 1; // Stop processing further
    }

    Car *car = &carList->cars[carList->count]; // Get pointer to next car slot
    car->id = atoi(argv[0]);                   // Convert string to int
    snprintf(car->name, sizeof(car->name), "%s", argv[1] ? argv[1] : "Unknown");
    car->price = atoi(argv[2]); // Convert string to int

    carList->count++; // Increment car count
    return 0;
}