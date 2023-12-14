#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_BOLD    "\x1b[1m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define MAX_CUSTOMERS 50
#define MAX_ORDERS 50
#define MAX_ITEMS 50

struct Customer {
    int id;
    char name[50];
    char contactNumber[15];
};

struct Measurement {
    float chest;
    float waist;
    float hips;
    float inseam;
};

struct Order {
    int id;
    struct Customer customer;
    struct Measurement measurements;
    float totalAmount;
    int status; 
};

struct InventoryItem {
    int id;
    char itemName[50];
    float price;
};

void displayMenu();
int customer_add(struct Customer *customers, int *numCustomers, int *customerIdCounter);
void customer_modify(struct Customer *customers, int numCustomers);
void customer_delete(struct Customer *customers, int *numCustomers);
void manageCustomers(struct Customer *customers, int *numCustomers, int *customerIdCounter);
int order_add(struct Order *orders, int *numOrders, struct Customer *customers, int numCustomers, int *orderIdCounter);
void order_modify(struct Order *orders, int numOrders);
void order_delete(struct Order *orders, int *numOrders);
void updateOrderStatus(struct Order *orders, int numOrders);
void manageOrders(struct Order *orders, int *numOrders, struct Customer *customers, int numCustomers, int *orderIdCounter);
void takeShirtMeasurements(struct Measurement *measurements);
void takePantMeasurements(struct Measurement *measurements);
void inventory_add(struct InventoryItem *inventory, int *numItems, int *itemIdCounter);
void inventory_modify(struct InventoryItem *inventory, int numItems);
void inventory_delete(struct InventoryItem *inventory, int *numItems);
void manageInventory(struct InventoryItem *inventory, int *numItems, int *itemIdCounter);
int search_customer(struct Customer *customers, int numCustomers, int customerId);
int search_order(struct Order *orders, int numOrders, int orderId);
void display_all_customers(struct Customer *customers, int numCustomers);
void display_all_orders(struct Order *orders, int numOrders);
void display_inventory(struct InventoryItem *inventory, int numItems);
void takeMeasurements(struct Measurement *measurements);
float calculateTotalAmount(struct Measurement measurements);
void saveDataToFile(struct Customer *customers, int numCustomers, struct Order *orders, int numOrders, struct InventoryItem *inventory, int numItems);
void loadDataFromFile(struct Customer *customers, int *numCustomers, struct Order *orders, int *numOrders, struct InventoryItem *inventory, int *numItems,
                      int *customerIdCounter, int *orderIdCounter, int *itemIdCounter);
void deleteAllData(struct Customer *customers, int *numCustomers,struct Order *orders, int *numOrders,struct InventoryItem *inventory,
                            int *numItems,int *customerIdCounter, int *orderIdCounter, int *itemIdCounter);

int garmentType;
float Amount;


int main() {
    struct Customer customers[MAX_CUSTOMERS];
    struct Order orders[MAX_ORDERS];
    struct InventoryItem inventory[MAX_ITEMS];

    int numCustomers = 0;
    int numOrders = 0;
    int numItems = 0;

    int customerIdCounter = 1;
    int orderIdCounter = 1;
    int itemIdCounter = 1;

    loadDataFromFile(customers, &numCustomers, orders, &numOrders, inventory, &numItems, &customerIdCounter, &orderIdCounter, &itemIdCounter);

    int choice;

    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                manageCustomers(customers, &numCustomers, &customerIdCounter);
                break;
            case 2:
                manageOrders(orders, &numOrders, customers, numCustomers, &orderIdCounter);
                break;
            case 3:
                manageInventory(inventory, &numItems, &itemIdCounter);
                break;
            case 4:
                if (numCustomers > 0 || numOrders > 0 || numItems > 0) {
                    printf(ANSI_COLOR_BLUE "\n***** Customer List *****\n" ANSI_COLOR_RESET);
                    display_all_customers(customers, numCustomers);

                    printf(ANSI_COLOR_BLUE "\n***** Order List *****\n" ANSI_COLOR_RESET);
                    display_all_orders(orders, numOrders);

                    printf(ANSI_COLOR_BLUE "\n***** Inventory List *****\n" ANSI_COLOR_RESET);
                    display_inventory(inventory, numItems);

                    saveDataToFile(customers, numCustomers, orders, numOrders, inventory, numItems);
                } else {
                    printf(ANSI_COLOR_RED "No data available.\n" ANSI_COLOR_RESET);
                }
                break;
            case 5:
                deleteAllData(customers, &numCustomers, orders, &numOrders, inventory, &numItems,
                               &customerIdCounter, &orderIdCounter, &itemIdCounter);
                break;
            case 6:
                printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA "Exiting program. Thank you!\n" ANSI_COLOR_RESET);
                break;
            default:
                printf(ANSI_COLOR_RED "Invalid choice. Please enter a valid option.\n" ANSI_COLOR_RESET);
        }
    } while (choice != 6);

    return 0;
}

void displayMenu() {
    printf(ANSI_COLOR_BLUE "\n***** AMAR TAILOR *****\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "1. Manage Customer\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "2. Manage Order\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "3. Manage Inventory\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN "4. Display All Data\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "5. Clear Database\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "6. Exit\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter your choice: " ANSI_COLOR_RESET);
}

void manageCustomers(struct Customer *customers, int *numCustomers, int *customerIdCounter) {
    int choice;

    do {
        printf(ANSI_COLOR_BLUE "\n***** Customer Management *****\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "1. Add Customer\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "2. Modify Customer\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "3. Delete Customer\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_MAGENTA "4. Back to Main Menu\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter your choice: " ANSI_COLOR_RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                customer_add(customers, numCustomers, customerIdCounter);
                break;
            case 2:
                if (*numCustomers > 0) {
                    customer_modify(customers, *numCustomers);
                } else {
                    printf(ANSI_COLOR_RED "No customers available. Please add a customer first.\n" ANSI_COLOR_RESET);
                }
                break;
            case 3:
                if (*numCustomers > 0) {
                    customer_delete(customers, numCustomers);
                } else {
                    printf(ANSI_COLOR_RED "No customers available.\n" ANSI_COLOR_RESET);
                }
                break;
            case 4:
                printf(ANSI_COLOR_MAGENTA "Returning to Main Menu.\n" ANSI_COLOR_RESET);
                break;
            default:
                printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "Invalid choice. Please enter a valid option.\n" ANSI_COLOR_RESET);
        }
    } while (choice != 4);
}

int customer_add(struct Customer *customers, int *numCustomers, int *customerIdCounter) {
    if (*numCustomers < MAX_CUSTOMERS) {
        customers[*numCustomers].id = (*customerIdCounter)++;
        printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "Enter customer name: " ANSI_COLOR_RESET) ;
        scanf("%s", customers[*numCustomers].name);

        printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "Enter contact number: " ANSI_COLOR_RESET);
        scanf("%s", customers[*numCustomers].contactNumber);

        (*numCustomers)++;
        printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN "Customer added successfully. Customer ID: %d\n", customers[*numCustomers - 1].id ,ANSI_COLOR_RESET);
        return customers[*numCustomers - 1].id;
    } else {
        printf(ANSI_COLOR_RED "Maximum number of customers reached.\n"ANSI_COLOR_RESET);
        return -1;
    }
}

void customer_modify(struct Customer *customers, int numCustomers) {
    int customerId;
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter customer ID to modify: " ANSI_COLOR_RESET);
    scanf("%d", &customerId);

    int index = search_customer(customers, numCustomers, customerId);
    if (index != -1) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "Enter new customer name: " ANSI_COLOR_RESET);
        scanf("%s", customers[index].name);

        printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "Enter new contact number: " ANSI_COLOR_RESET);
        scanf("%s", customers[index].contactNumber);

        printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA "Customer modified successfully.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Customer with ID %d not found.\n", customerId ,ANSI_COLOR_RESET);
    }
}

void customer_delete(struct Customer *customers, int *numCustomers) {
    int customerId;
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter customer ID to delete: " ANSI_COLOR_RESET);
    scanf("%d", &customerId);

    int index = search_customer(customers, *numCustomers, customerId);
    if (index != -1) {
        for (int i = index; i < *numCustomers - 1; ++i) {
            customers[i] = customers[i + 1];
        }

        (*numCustomers)--;
        printf(ANSI_COLOR_GREEN "Customer deleted successfully.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Customer with ID %d not found.\n", customerId, ANSI_COLOR_RESET);
    }
}

int order_add(struct Order *orders, int *numOrders, struct Customer *customers, int numCustomers, int *orderIdCounter) {
    if (*numOrders < MAX_ORDERS) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter customer ID for the order: " ANSI_COLOR_RESET);
        int customerId;
        scanf("%d", &customerId);

        int customerIndex = search_customer(customers, numCustomers, customerId);
        if (customerIndex != -1) {
            orders[*numOrders].id = (*orderIdCounter)++;
            orders[*numOrders].customer = customers[customerIndex];

            printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA "Taking measurements for order ID %d:\n", orders[*numOrders].id, ANSI_COLOR_RESET);
            takeMeasurements(&(orders[*numOrders].measurements));

            orders[*numOrders].status = 0; 
            orders[*numOrders].totalAmount = calculateTotalAmount(orders[*numOrders].measurements);
            (*numOrders)++;

            printf(ANSI_COLOR_GREEN "Order added successfully. Order ID: %d \t Total amount: %.2f\n", orders[*numOrders - 1].id, Amount, ANSI_COLOR_RESET);
            return orders[*numOrders - 1].id;
        } else {
            printf(ANSI_COLOR_RED "Customer with ID %d not found.\n", customerId, ANSI_COLOR_RESET);
            return -1;
        }
    } else {
        printf(ANSI_COLOR_RED "Maximum number of orders reached.\n" ANSI_COLOR_RESET);
        return -1;
    }
}

void order_modify(struct Order *orders, int numOrders) {
    int orderId;
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter order ID to modify: " ANSI_COLOR_RESET);
    scanf("%d", &orderId);

    int index = search_order(orders, numOrders, orderId);
    if (index != -1) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA "Modifying order ID %d:\n", orders[index].id, ANSI_COLOR_RESET);

        takeMeasurements(&(orders[index].measurements));
        orders[index].totalAmount = calculateTotalAmount(orders[index].measurements);

        printf(ANSI_COLOR_GREEN "Order modified successfully.\n"ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Order with ID %d not found.\n", orderId, ANSI_COLOR_RESET);
    }
}

void order_delete(struct Order *orders, int *numOrders) {
    int orderId;
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter order ID to delete: " ANSI_COLOR_RESET);
    scanf("%d", &orderId);

    int index = search_order(orders, *numOrders, orderId);
    if (index != -1) {
        for (int i = index; i < *numOrders - 1; ++i) {
            orders[i] = orders[i + 1];
        }

        (*numOrders)--;
        printf(ANSI_COLOR_GREEN "Order deleted successfully.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Order with ID %d not found.\n", orderId, ANSI_COLOR_RESET);
    }
}

void updateOrderStatus(struct Order *orders, int numOrders) {
    int orderId;
    printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA "Updating Order Status\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter order ID to update status: " ANSI_COLOR_RESET);
    scanf("%d", &orderId);

    int index = search_order(orders, numOrders, orderId);
    if (index != -1) {
        int newStatus;
        printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter new status (0 for In Progress, 1 for Completed): " ANSI_COLOR_RESET);
        scanf("%d", &newStatus);

        if (newStatus == 0 || newStatus == 1) {
            orders[index].status = newStatus;
            printf(ANSI_COLOR_GREEN "Order status updated successfully.\n"ANSI_COLOR_RESET);
        } else {
            printf(ANSI_COLOR_RED "Invalid status. Please enter 0 for In Progress or 1 for Completed.\n" ANSI_COLOR_RESET);
        }
    } else {
        printf( ANSI_COLOR_RED "Order with ID %d not found.\n", orderId, ANSI_COLOR_RESET);
    }
}

void manageOrders(struct Order *orders, int *numOrders, struct Customer *customers, int numCustomers, int *orderIdCounter) {
    int choice;

    do {
        printf(ANSI_COLOR_BLUE "\n***** Order Management *****\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "1. Add Order\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "2. Modify Order\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "3. Delete Order\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "4. Update Order Status\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_MAGENTA "5. Back to Main Menu\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter your choice: " ANSI_COLOR_RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                order_add(orders, numOrders, customers, numCustomers, orderIdCounter);
                break;
            case 2:
                if (*numOrders > 0) {
                    order_modify(orders, *numOrders);
                } else {
                    printf(ANSI_COLOR_RED "No orders available. Please add an order first.\n" ANSI_COLOR_RESET);
                }
                break;
            case 3:
                if (*numOrders > 0) {
                    order_delete(orders, numOrders);
                } else {
                    printf(ANSI_COLOR_RED "No orders available.\n" ANSI_COLOR_RESET);
                }
                break;
            case 4:
                if (*numOrders > 0) {
                updateOrderStatus(orders, numOrders);
                } else {
                    printf(ANSI_COLOR_RED "No orders available.\n" ANSI_COLOR_RESET);
                }
                break;
            case 5:
                printf(ANSI_COLOR_MAGENTA "Returning to Main Menu.\n" ANSI_COLOR_RESET);
                break;
            default:
                printf(ANSI_COLOR_RED "Invalid choice. Please enter a valid option.\n" ANSI_COLOR_RESET);
        }
    } while (choice != 5);
}

void inventory_add(struct InventoryItem *inventory, int *numItems, int *itemIdCounter) {
    if (*numItems < MAX_ITEMS) {
        inventory[*numItems].id = (*itemIdCounter)++;
        printf(ANSI_COLOR_YELLOW "Enter item name: " ANSI_COLOR_RESET);
        scanf("%s", inventory[*numItems].itemName);

        printf(ANSI_COLOR_YELLOW "Enter item price: " ANSI_COLOR_RESET);
        scanf("%f", &inventory[*numItems].price);

        (*numItems)++;
        printf(ANSI_COLOR_GREEN "Item added successfully. Item ID: %d\n", inventory[*numItems - 1].id, ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Maximum number of inventory items reached.\n" ANSI_COLOR_RESET);
    }
}

void inventory_modify(struct InventoryItem *inventory, int numItems) {
    int itemId;
    printf(ANSI_COLOR_YELLOW "Enter item ID to modify: " ANSI_COLOR_RESET);
    scanf("%d", &itemId);

    int index = -1;
    for (int i = 0; i < numItems; ++i) {
        if (inventory[i].id == itemId) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA "Modifying item ID %d:\n", inventory[index].id, ANSI_COLOR_RESET);
        printf(ANSI_COLOR_YELLOW "Enter new item name: " ANSI_COLOR_RESET);
        scanf("%s", inventory[index].itemName);

        printf(ANSI_COLOR_YELLOW "Enter new item price: " ANSI_COLOR_RESET);
        scanf("%f", &inventory[index].price);

        printf(ANSI_COLOR_GREEN "Item modified successfully.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Item with ID %d not found.\n", itemId, ANSI_COLOR_RESET);
    }
}

void inventory_delete(struct InventoryItem *inventory, int *numItems) {
    int itemId;
    printf(ANSI_COLOR_YELLOW "Enter item ID to delete: " ANSI_COLOR_RESET);
    scanf("%d", &itemId);

    int index = -1;
    for (int i = 0; i < *numItems; ++i) {
        if (inventory[i].id == itemId) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        for (int i = index; i < *numItems - 1; ++i) {
            inventory[i] = inventory[i + 1];
        }

        (*numItems)--;
        printf(ANSI_COLOR_GREEN "Item deleted successfully.\n" ANSI_COLOR_RESET);
    } else {
        printf( ANSI_COLOR_RED "Item with ID %d not found.\n", itemId, ANSI_COLOR_RESET);
    }
}

void manageInventory(struct InventoryItem *inventory, int *numItems, int *itemIdCounter) {
    int choice;

    do {
        printf(ANSI_COLOR_BLUE "\n***** Inventory Management *****\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "1. Add to Inventory\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "2. Modify Inventory\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "3. Delete from Inventory\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_MAGENTA "4. Back to Main Menu\n"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter your choice: "ANSI_COLOR_RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inventory_add(inventory, numItems, itemIdCounter);
                break;
            case 2:
                if (*numItems > 0) {
                    inventory_modify(inventory, *numItems);
                } else {
                    printf(ANSI_COLOR_BOLD ANSI_COLOR_RED "Inventory is empty. Please add an item first.\n" ANSI_COLOR_RESET);
                }
                break;
            case 3:
                if (*numItems > 0) {
                    inventory_delete(inventory, numItems);
                } else {
                    printf(ANSI_COLOR_RED "Inventory is empty.\n"ANSI_COLOR_RESET);
                }
                break;
            case 4:
                printf(ANSI_COLOR_MAGENTA "Returning to Main Menu.\n" ANSI_COLOR_RESET);
                break;
            default:
                printf(ANSI_COLOR_RED "Invalid choice. Please enter a valid option.\n" ANSI_COLOR_RESET);
        }
    } while (choice != 4);
}

int search_customer(struct Customer *customers, int numCustomers, int customerId) {
    for (int i = 0; i < numCustomers; ++i) {
        if (customers[i].id == customerId) {
            return i;
        }
    }
    return -1;
}

int search_order(struct Order *orders, int numOrders, int orderId) {
    for (int i = 0; i < numOrders; ++i) {
        if (orders[i].id == orderId) {
            return i;
        }
    }
    return -1;
}

void display_all_customers(struct Customer *customers, int numCustomers) {
    printf(ANSI_COLOR_BOLD ANSI_COLOR_BLUE "ID\tName\tContact Number\n" ANSI_COLOR_RESET);
    for (int i = 0; i < numCustomers; ++i) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "%d\t%s\t%s\n", customers[i].id, customers[i].name, customers[i].contactNumber, ANSI_COLOR_RESET);
    }
}

void display_all_orders(struct Order *orders, int numOrders) {
    printf(ANSI_COLOR_BOLD ANSI_COLOR_BLUE "ID\tCustomer ID\tStatus\t\tTotal Amount\n" ANSI_COLOR_RESET);
    for (int i = 0; i < numOrders; ++i) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "%d\t%d\t\t%s\t%.2f\n", orders[i].id, orders[i].customer.id,
               (orders[i].status == 0) ? "In Progress" : "Completed", orders[i].totalAmount, ANSI_COLOR_RESET);
    }
}

void display_inventory(struct InventoryItem *inventory, int numItems) {
    printf(ANSI_COLOR_BOLD ANSI_COLOR_BLUE "ID\tItem Name\tPrice\n" ANSI_COLOR_RESET);
    for (int i = 0; i < numItems; ++i) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "%d\t%s\t\t%.2f\n", inventory[i].id, inventory[i].itemName, inventory[i].price, ANSI_COLOR_RESET);
    }
}

void takeMeasurements(struct Measurement *measurements) {
    int choice;

    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Choose measurement type:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "1. Shirt\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"2. Pant\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter your choice: " ANSI_COLOR_RESET);
    scanf("%d", &choice);
    garmentType=choice;

    switch (choice) {
        case 1:
            takeShirtMeasurements(measurements);
            break;
        case 2:
            takePantMeasurements(measurements);
            break;
        default:
            printf(ANSI_COLOR_RED "Invalid choice. Defaulting to shirt measurements.\n" ANSI_COLOR_RESET);
            takeShirtMeasurements(measurements);
            break;
    }
}

void takeShirtMeasurements(struct Measurement *measurements) {

    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter chest measurement (in inches): " ANSI_COLOR_RESET);
    scanf("%f", &measurements->chest);

    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter waist measurement (in inches): " ANSI_COLOR_RESET);
    scanf("%f", &measurements->waist);

    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter hips measurement (in inches): " ANSI_COLOR_RESET);
    scanf("%f", &measurements->hips);

    measurements->inseam = 0.0;
}

void takePantMeasurements(struct Measurement *measurements) {

    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter waist measurement (in inches): " ANSI_COLOR_RESET);
    scanf("%f", &measurements->waist);

    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "Enter inseam measurement (in inches): " ANSI_COLOR_RESET);
    scanf("%f", &measurements->inseam);

    measurements->chest = 0.0;
    measurements->hips = 0.0;
}

float calculateTotalAmount(struct Measurement measurements) {
    const float shirtRate = 2.5;  
    const float pantRate = 3.0;  

    float totalAmount = 0.0;

    switch (garmentType) {
        case 1:  
            totalAmount = shirtRate * (measurements.chest + measurements.waist + measurements.hips);
            break;
        case 2:  
            totalAmount = pantRate * (measurements.inseam + measurements.waist);
            break;
        default:
            printf("Invalid garment type. Unable to calculate total amount.\n");
            break;
    }
    Amount=totalAmount;
    return totalAmount;
}

void saveDataToFile(struct Customer *customers, int numCustomers, struct Order *orders, int numOrders, struct InventoryItem *inventory, int numItems) {
    FILE *file = fopen("AMAR TAILOR.txt", "w");

    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "Customers:\n");
    for (int i = 0; i < numCustomers; ++i) {
        fprintf(file, "%d %s %s\n", customers[i].id, customers[i].name, customers[i].contactNumber);
    }

    fprintf(file, "\nOrders:\n");
    for (int i = 0; i < numOrders; ++i) {
        fprintf(file, "%d %d %f %f %f %f %d %f\n", orders[i].id, orders[i].customer.id,
                orders[i].measurements.chest, orders[i].measurements.waist, orders[i].measurements.hips,
                orders[i].measurements.inseam, orders[i].status, orders[i].totalAmount);
    }

    fprintf(file, "\nInventory:\n");
    for (int i = 0; i < numItems; ++i) {
        fprintf(file, "%d %s %f\n", inventory[i].id, inventory[i].itemName, inventory[i].price);
    }

    fclose(file);
}

void loadDataFromFile(struct Customer *customers, int *numCustomers, struct Order *orders, int *numOrders, struct InventoryItem *inventory, int *numItems,
                      int *customerIdCounter, int *orderIdCounter, int *itemIdCounter) {
    FILE *file = fopen("AMAR TAILOR.txt", "r");

    if (file == NULL) {
        printf(ANSI_COLOR_RED "No existing data file found.\n" ANSI_COLOR_RESET);
        return;
    }

    fscanf(file, "Customers:\n");
    while (fscanf(file, "%d %s %s", &customers[*numCustomers].id, customers[*numCustomers].name, customers[*numCustomers].contactNumber) == 3) {
        (*numCustomers)++;
    }

    fscanf(file, "\nOrders:\n");
    while (fscanf(file, "%d %d %f %f %f %f %d %f", &orders[*numOrders].id, &orders[*numOrders].customer.id,
                  &orders[*numOrders].measurements.chest, &orders[*numOrders].measurements.waist,
                  &orders[*numOrders].measurements.hips, &orders[*numOrders].measurements.inseam,
                  &orders[*numOrders].status, &orders[*numOrders].totalAmount) == 8) {
        (*numOrders)++;
    }

    fscanf(file, "\nInventory:\n");
    while (fscanf(file, "%d %s %f", &inventory[*numItems].id, inventory[*numItems].itemName, &inventory[*numItems].price) == 3) {
        (*numItems)++;
    }

    fclose(file);
    
    *customerIdCounter = customers[*numCustomers - 1].id + 1;
    *orderIdCounter = orders[*numOrders - 1].id + 1;
    *itemIdCounter = inventory[*numItems - 1].id + 1;
}

void deleteAllData(struct Customer *customers, int *numCustomers,
                   struct Order *orders, int *numOrders,
                   struct InventoryItem *inventory, int *numItems,
                   int *customerIdCounter, int *orderIdCounter, int *itemIdCounter) {
    *numCustomers = 0;
    *numOrders = 0;
    *numItems = 0;
    *customerIdCounter = 1;
    *orderIdCounter = 1;
    *itemIdCounter = 1;

    memset(customers, 0, sizeof(struct Customer) * MAX_CUSTOMERS);
    memset(orders, 0, sizeof(struct Order) * MAX_ORDERS);
    memset(inventory, 0, sizeof(struct InventoryItem) * MAX_ITEMS);

    FILE *file = fopen("AMAR TAILOR.txt", "w");
    fclose(file);

    printf(ANSI_COLOR_GREEN "All data " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED "deleted " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN "successfully\n" ANSI_COLOR_RESET);

}
