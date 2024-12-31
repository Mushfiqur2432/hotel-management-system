#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants and Macros
#define MAX_ROOMS 100

// Enumerations for Room Types and Packages
typedef enum {
    ROOM_STANDARD = 1,
    ROOM_DELUXE,
    ROOM_SUITE,
    ROOM_EXECUTIVE
} RoomType;

typedef enum {
    PACKAGE_1DAY = 1,
    PACKAGE_2DAY,
    PACKAGE_3DAY,
    PACKAGE_1WEEK,
    PACKAGE_2WEEK,
    PACKAGE_1MONTH
} PackageType;

// Struct Definitions
typedef struct {
    char name[100];
    char phone[20];
    int roomNumber;
    int stayDuration;
    RoomType roomType;
    PackageType packageType;
    double totalBill;
    int status; // 0: Vacant, 1: Occupied
} Guest;


// Define a structure to store user credentials
typedef struct {
    char username[20];
    char password[20];
} User;

// Function Prototypes
void initializeSystem();
void showMainMenu();
void checkInGuest();
void checkOutGuest();
void viewRoomStatus();
void viewGuestList();
double calculateTotalBill(RoomType roomType, PackageType packageType);
int login(User *user);

// Global Variables
Guest guestDatabase[MAX_ROOMS] = {0};

// Room and Package Descriptions
const char* roomTypeNames[] = {"Standard", "Deluxe", "Suite", "Executive"};
double roomRateMultipliers[] = {1.0, 1.5, 2.0, 2.5};

const char* packageNames[] = {"1 Day", "2 Days", "3 Days", "1 Week", "2 Weeks", "1 Month"};
int packageDurations[] = {1, 2, 3, 7, 14, 30};
double packageBasePrices[] = {100.0, 190.0, 270.0, 600.0, 1100.0, 2200.0};

// Function to login
int login(User *user) {
    char input_username[20];
    char input_password[20];

    printf("Enter username: ");
    scanf("%s", input_username);
    printf("Enter password: ");
    scanf("%s", input_password);

    // Check if credentials match
    if (strcmp(input_username, user->username) == 0 && strcmp(input_password, user->password) == 0) {
        printf("Login successful!\n");
        return 1; // Login successful
    } else {
        printf("Invalid username or password\n");
        return 0; // Login failed
    }
}

// Function to logout
void logout() {
    printf("You have been logged out\n");
}

// System Initialization
void initializeSystem() {
    printf("=== Welcome to Hotel Management System ===\n");
    printf("Initializing database...\n");
    memset(guestDatabase, 0, sizeof(guestDatabase));
    printf("System ready!\n");

    showMainMenu();
}

void showMainMenu() {
    printf("=== Main Menu ===\n");
    printf("1. Check-in Guest\n");
    printf("2. Check-out Guest\n");
    printf("3. View Room Status\n");
    printf("4. View Guest List\n");
    printf("5. Exit\n");
    int choice;
    scanf("%d", &choice);

    // Perform actions based on user choice
    switch (choice) {
        case 1:
            checkInGuest();
            break;
        case 2:
            checkOutGuest();
            break;
        case 3:
            viewRoomStatus();
            break;
        case 4:
            viewGuestList();
            break;
        case 5:
            return; // Exit the program
        default:
            printf("Invalid choice\n");
            break;
    }

    showMainMenu(); // Call showMainMenu() recursively to display the main menu again
}

// Main Function
int main() {
    User user;
    strcpy(user.username, "admin");
    strcpy(user.password, "password");

    int logged_in = 0;

    while (1) {
        if (!logged_in) {
            printf("1. Login\n");
            printf("2. Exit\n");
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                logged_in = login(&user);
                if (logged_in) {
                    initializeSystem();//call the initialize system function
                }
            } else if (choice == 2) {
                break;
            }
        } else {
            printf("1. Logout\n");
            printf("2. Exit\n");
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                logout();
                logged_in = 0;
            } else if (choice == 2) {
                break;
            }
        }
    }

    return 0;
}
// Check-In Functionality
void checkInGuest() {
    Guest newGuest;
    int availableRooms = MAX_ROOMS;

    printf("=== Guest Check-In ===\n");

    // Input Guest Details
    printf("Enter guest name: ");
    getchar(); // Clear input buffer
    fgets(newGuest.name, sizeof(newGuest.name), stdin);
    newGuest.name[strcspn(newGuest.name, "\n")] = 0;

    printf("Enter phone number: ");
    fgets(newGuest.phone, sizeof(newGuest.phone), stdin);
    newGuest.phone[strcspn(newGuest.phone, "\n")] = 0;
    
    printf("Enter room number (1-%d): ", MAX_ROOMS);
    scanf("%d", &newGuest.roomNumber);

    if (guestDatabase[newGuest.roomNumber - 1].status == 1) {
        printf("Room is already occupied. Please choose another room.\n");
        return;
    }

    // Select Room Type
    printf("\nRoom Types:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s (Multiplier: x%.1f)\n", i + 1, roomTypeNames[i], roomRateMultipliers[i]);
    }
    printf("Select room type: ");
    scanf("%d", (int*)&newGuest.roomType);

    // Select Package
    printf("\nPackages:\n");
    for (int i = 0; i < 6; i++) {
        printf("%d. %s (%d days, $%.2f)\n", i + 1, packageNames[i], packageDurations[i], packageBasePrices[i]);
    }
    printf("Select package: ");
    scanf("%d", (int*)&newGuest.packageType);

    // Calculate Total Bill
    newGuest.totalBill = calculateTotalBill(newGuest.roomType, newGuest.packageType);
    printf("Total Bill: $%.2f\n", newGuest.totalBill);

    // Update Guest Database
    guestDatabase[newGuest.roomNumber - 1] = newGuest;
    guestDatabase[newGuest.roomNumber - 1].status = 1;

    MAX_ROOMS-1;
    printf("Available rooms: %d\n", MAX_ROOMS);

    printf("Guest checked in successfully!\n");
}

// Check-Out Functionality
void checkOutGuest() {
        int roomNumber;

    printf("=== Guest Check-Out ===\n");
    printf("Enter room number to check out: ");
    scanf("%d", &roomNumber);

    if (guestDatabase[roomNumber - 1].status == 1) {
        guestDatabase[roomNumber - 1].status = 0;
        printf("Guest checked out successfully! Room is now vacant.\n");
    } else {
        printf("Room is already vacant or does not exist.\n");
    }
}

// View Room Status
void viewRoomStatus() {
    printf("=== Room Status ===\n");
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (guestDatabase[i].status == 1) {
            printf("Room %d: Booked by %s\n", i + 1, guestDatabase[i].name);
        } else {
            printf("Room %d: Available\n", i + 1);
        }
    }
    printf("You Can Book Any Room From The Available Rooms\n");
}

// View Guest List
void viewGuestList() {
      printf("=== Guest List ===\n");
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (guestDatabase[i].status == 1) {
            printf("Room %d:\n", i + 1);
            printf("  Name: %s\n", guestDatabase[i].name);
            printf("  Phone: %s\n", guestDatabase[i].phone);
            printf("  Room Type: %s\n", roomTypeNames[guestDatabase[i].roomType - 1]);
            printf("  Package: %s\n", packageNames[guestDatabase[i].packageType - 1]);
            printf("  Total Bill: $%.2f\n", guestDatabase[i].totalBill);
        }
    }
}

// Bill Calculation
double calculateTotalBill(RoomType roomType, PackageType packageType) {
    double basePrice = packageBasePrices[packageType - 1];
    double multiplier = roomRateMultipliers[roomType - 1];
    return basePrice * multiplier;
}
