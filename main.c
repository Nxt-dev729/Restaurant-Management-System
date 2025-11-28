#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include <process.h>
#include <ctype.h>
#include <time.h>

#define max_item_count 100
#define item_name_len 50
#define MAX_PASSWORD_LENGTH 50
#define CORRECT_PASSWORD "admin123" // Default shopkeeper password

// Function prototypes
void setTextColor(int color);
void speakText(char text[]);
void typeText(const char *text, int delay_ms);
void drawSeparator(char symbol, int length);
void clearScreen();
void clearInputBuffer();
void errorHandling(const char *message);
void displayWelcomeArt();
void displayExitArt();
void menu();
void addItem();
void displayItem();
void saveItems();
void loadItems();
void takeOrder();
void giveFeedback();
void showFeedback();
bool authenticateShopkeeper();
void addEmployeeDetail();
void seeEmployeeDetail();

struct MenuItem {
    char name[100];
    float price;
    char category[50];
    char description[250];
};
struct MenuItem items[max_item_count];
int item_count=0;
const char *filename="items.txt";

struct Employee {
    char name[50];
    char mobileNo[15];
    char gmail[100];
    float salary;
};

void speakText(char text[]){
    text[strcspn(text, "\n")] = '\0';
    char command[512];

    snprintf(command, sizeof(command),
             "PowerShell -Command \"Add-Type -AssemblyName System.Speech; "
             "$synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
             "$synth.Speak('%s');\"",
             text);

    system(command);
};

void setTextColor(int color){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void typeText(const char *text, int delay_ms){
    while (*text){
        putchar(*text++);
        fflush(stdout);
        Sleep(delay_ms);
    }
}

void displayWelcomeArt(){
    static int temp=1;
    time_t now;
    struct tm *localTime;
    
    //To get current time
    time(&now);

    //Convert to local time                  
    localTime=localtime(&now);

    int hour=localTime->tm_hour;

    if(temp==1){
        if(hour>=5 && hour<12){
            char greetingMessage[]="\t\t\t\t\t\t\t\tGood morning!";
            speakText(greetingMessage);
        }
        else if(hour>=12 && hour<17){
            char greetingMessage[]="\t\t\t\t\t\t\t\tGood afternoon!";
            speakText(greetingMessage);
        }
        else{
            char greetingMessage[]="\t\t\t\t\t\t\t\tGood evening!";
            speakText(greetingMessage);
        }
        char welcomeMessage[]="\t\t\t\t\t\t\t\tWelcome to our RESTAURANT!";
        speakText(welcomeMessage);
    }
    setTextColor(14);   //Yellow
    typeText("\t\t\t\t\t\t\t\t#######################################\n", 10);
    
    if(hour>=5 && hour<12){
        typeText("\t\t\t\t\t\t\t\t#            Good Morning             #\n", 10);
    }
    else if(hour>=12 && hour<17){
        typeText("\t\t\t\t\t\t\t\t#            Good Afternoon           #\n", 10);
    }
    else{
        typeText("\t\t\t\t\t\t\t\t#            Good Evening             #\n", 10);
    }

    typeText("\t\t\t\t\t\t\t\t#    WELCOME TO THE OUR RESTAURANT!   #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#######################################\n\n", 10);
    setTextColor(7);    //Light gray
    temp=0;
}


void displayExitArt(){
    setTextColor(10);   //Green
    char seeYa1[]="\t\t\t\t\t\t\t\tThank you for using this Restaurant!";
    typeText("\n\t\t\t\t\t\t\t\tThank you for using this Restaurant! \n", 10);
    speakText(seeYa1);
    typeText("\t\t\t\t\t\t\t\t#######################################\n", 10);
    typeText("\t\t\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#         SEE YOU NEXT TIME!          #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#######################################\n", 10);
    char seeYa2[]="\t\t\t\t\t\t\t\tSee you next time!";
    speakText(seeYa2);
    setTextColor(7);    //Light gray
}

void drawSeparator(char symbol, int length){
    for(int i=0; i<length; i++){
        putchar(symbol);
    }
    putchar('\n');
}

void clearScreen(){
    system("cls");
}

void errorHandling(const char *message){
    setTextColor(12);                                                           //Red
    printf("Error: %s\n", message);
    setTextColor(7);                                                            //Light gray
    exit(1);
}

void clearInputBuffer(){
    int c;
    while ((c=getchar())!='\n' && c!=EOF);
}

const struct MenuItem predefItems[] = {
    {"Pasta", 150.0, "Starters", "Delicious pasta with tomato sauce."},
    {"Momos", 70.0, "Snacks & Street Food", "Steamed dumplings."},
    {"Fruit Salad", 70.0, "Desserts", "Mixed fresh fruits."},
    {"Pav Bhaji", 90.0, "Snacks & Street Food", "Spicy vegetable mash with bread."},
    {"Dhokla", 40.0, "Snacks & Street Food", "Steamed savory cake."},
    {"Pani Puri", 50.0, "Snacks & Street Food", "Crispy puris with spicy water."},
    {"Chicken", 250.0, "Full Dishes", "Spicy grilled chicken."},
    {"Dal Makhani", 150.0, "Full Dishes", "Creamy black lentils."}
};

const int predefCount=sizeof(predefItems)/sizeof(predefItems[0]);

void addItem() {
    char choice;
    int hellonum2 = 1;
    do {
        if(item_count >= max_item_count){
            setTextColor(12); // Red
            char full[] = "\t\t\t\t\t\t\t\tRecipe storage is full!";
            typeText("Recipe storage is full!\n", 20);
            speakText(full);
            setTextColor(7); // Reset to default color
            return;
        }

        struct MenuItem newItem;

        if(hellonum2 == 1){
            clearScreen();
            setTextColor(14);
            typeText("You can add your own recipe here....\n\n", 20);
            char recipeMsg[] = "You can add your own recipe here";
            speakText(recipeMsg);
            hellonum2 = 0;
        }

        setTextColor(11); 
        typeText("Enter item name: ", 20);
        setTextColor(8); 
        fgets(newItem.name, item_name_len, stdin);
        newItem.name[strcspn(newItem.name, "\n")] = 0;

        setTextColor(11); 
        typeText("Enter item price: ", 20);
        setTextColor(8); 
        scanf("%f", &newItem.price);
        clearInputBuffer();

        setTextColor(9); 
        typeText("Choose the category from below options:\n", 20);
        char categoryMsg[] = "Choose the category from below options";
        speakText(categoryMsg);

        setTextColor(7);
        typeText("1. Starters\n", 20);
        typeText("2. Soups & Salads\n", 20);
        typeText("3. Full Dishes\n", 20);
        typeText("4. Rice & Bread\n", 20);
        typeText("5. Desserts\n", 20);
        typeText("6. Snacks & Street Food\n", 20);
        typeText("7. Beverages\n", 20);

        setTextColor(11); // Light cyan
        typeText("Enter your choice: ", 20);

        setTextColor(8); // Gray
        int category_choice;
        scanf("%d", &category_choice);
        clearInputBuffer();

        switch (category_choice) {
        case 1:
            strcpy(newItem.category, "Starters");
            break;
        case 2:
            strcpy(newItem.category, "Soups & Salads");
            break;
        case 3:
            strcpy(newItem.category, "Full Dishes");
            break;
        case 4:
            strcpy(newItem.category, "Rice & Bread");
            break;
        case 5:
            strcpy(newItem.category, "Desserts");
            break;
        case 6:
            strcpy(newItem.category, "Snacks & Street Food");
            break;
        case 7:
            strcpy(newItem.category, "Beverages");
            break;
        default:
            strcpy(newItem.category, "Uncategorized");
            typeText("Invalid category. Recipe added as 'Uncategorized'.\n", 20);
        }

        items[item_count++] = newItem;

        setTextColor(10); // Green
        char recipeAdded[100];
        sprintf(recipeAdded, "%s added successfully!", newItem.name);
        typeText("\n", 20);
        typeText(recipeAdded, 20);
        typeText("\n", 20);
        speakText(recipeAdded);
        setTextColor(7); // Reset to default color

        setTextColor(13); 
        typeText("\nDo you want to add another item? (y/n): ", 20);
        scanf(" %c", &choice);
        clearInputBuffer();
        printf("\n");

    } while (choice == 'y' || choice == 'Y');

    setTextColor(7); // Reset to default color
}

void displayItem() {
    setTextColor(14); // Blue
    typeText("Here are all the Items!\n", 20);
    char display[] = "Here are all the Items!";
    speakText(display);

    setTextColor(13); // Magenta
    printf("\n\t\t\t--- All Items ---\n");
    drawSeparator('-', 70);
    for (int i=0; i<predefCount + item_count; i++) {
        if (i < predefCount){
            printf("%-3d | %-20s | %-10.2f | %-25s\n", 
                   i + 1, predefItems[i].name, predefItems[i].price, predefItems[i].category);
        }
        else{
            printf("%-3d | %-20s | %-10.2f | %-25s\n", 
                   i + 1, items[i - predefCount].name, items[i - predefCount].price, items[i - predefCount].category);
        }
    }
    drawSeparator('-', 70);
    setTextColor(7); // Reset to default color
}

//Haystack=main string and needle= substring which we need to search
//Ptr is returned if found
char *strcasestr(const char *haystack, const char *needle){
    if (!haystack || !needle)
        return NULL;
    size_t needle_len = strlen(needle);

    while (*haystack) {
        if (strncasecmp(haystack, needle, needle_len) == 0)
            return (char *)haystack;
        haystack++;
    }

    return NULL;
}

void saveItems() {
    FILE *file = fopen("items.txt", "w");
    if (!file) {
        errorHandling("Failed to open file for writing.");
        return;
    }

    fprintf(file, "%d\n", item_count); // Save the item count
    for (int i = 0; i < item_count; i++) {
        fprintf(file, "%s\n%.2f\n%s\n%s\n", 
                items[i].name, 
                items[i].price, 
                items[i].category, 
                items[i].description);
    }

    fclose(file);
    printf("Recipes saved to file.\n");
}

void loadItems() {
    FILE *file = fopen("items.txt", "r");
    if (!file) {
        printf("No existing recipe file found. Starting fresh.\n");
        return;
    }

    fscanf(file, "%d\n", &item_count); // Load the item count
    for (int i = 0; i < item_count; i++) {
        fgets(items[i].name, sizeof(items[i].name), file);
        items[i].name[strcspn(items[i].name, "\n")] = 0; // Remove newline character

        fscanf(file, "%f\n", &items[i].price);

        fgets(items[i].category, sizeof(items[i].category), file);
        items[i].category[strcspn(items[i].category, "\n")] = 0; // Remove newline character

        fgets(items[i].description, sizeof(items[i].description), file);
        items[i].description[strcspn(items[i].description, "\n")] = 0; // Remove newline character
    }

    fclose(file);
    printf("Items loaded from file.\n");
}

bool authenticateShopkeeper() {
    char password[MAX_PASSWORD_LENGTH];
    int attempts = 3;

    clearScreen();
    typeText("Enter Shopkeeper Password: ", 20);
    char passwordMsg[]="Enter Shopkeeper Password";
    speakText(passwordMsg);
    
    
        setTextColor(14);
        fgets(password, MAX_PASSWORD_LENGTH, stdin);
        password[strcspn(password, "\n")] = 0;

        if (strcmp(password, CORRECT_PASSWORD) == 0) {
            setTextColor(10); // Green
            typeText("Login Successful!\n", 20);
            char loginSuccess[]="Login Successful";
            setTextColor(7);
            speakText(loginSuccess);
            setTextColor(7); // Reset to default
            return true;
        }
        else{
            setTextColor(12); // Red
            typeText("Incorrect Password!\n", 20);
            char incorrectPassword[]="Incorrect Password";
            speakText(incorrectPassword);
            setTextColor(7); // Reset to default
            return false;
        }
}

void showFeedback() {
    FILE *file = fopen("feedback.txt", "r");
    if (!file) {
        setTextColor(12); // Red
        typeText("No feedback records found.\n", 20);
        char noFeedback[] = "No feedback records found.";
        speakText(noFeedback);
        setTextColor(7); // Reset to default color
        return;
    }

    clearScreen();
    setTextColor(14); // Yellow
    typeText("Here are the feedbacks...\n\n", 20);
    char feedbackDetails[] = "Here are the feedbacks";
    speakText(feedbackDetails);

    char feedback[250];
    while (fgets(feedback, sizeof(feedback), file)) {
        setTextColor(11); // Light cyan
        printf("=> ");
        setTextColor(13); // Magenta
        printf("%s", feedback);
    }

    fclose(file);
    setTextColor(7); // Reset to default color
}

void menu(){
    int choice;
    while (1){
        clearScreen();
        displayWelcomeArt();

        setTextColor(9); // Magenta
        printf("\t\t\t\t\t\t\t\t");
        drawSeparator('=', 39);

        setTextColor(11); // cyan
        typeText("\t\t\t\t\t\t\t\t1. Customer\n", 10);
        typeText("\t\t\t\t\t\t\t\t2. Shopkeeper\n", 10);
        typeText("\t\t\t\t\t\t\t\t3. Show Feedback\n", 10);
        typeText("\t\t\t\t\t\t\t\t4. Final Exit\n", 10);

        setTextColor(9); // vlue
        printf("\t\t\t\t\t\t\t\t");
        drawSeparator('=', 39);

        setTextColor(13); // Magenta
        typeText("Enter your choice: ", 10);
        char textMsg[] = "Enter your choice";
        speakText(textMsg);
        scanf("%d", &choice);
        
        clearInputBuffer();
        switch (choice) {
        case 1: { // Customer Menu
            int customerChoice;
            do {
                clearScreen();
                setTextColor(14); // yello
                typeText("\n\t\t\t\t\t\t\t\t\tCustomer Menu\n", 10);
                typeText("\t\t\t\t\t\t\t", 10);

                setTextColor(9); // blue
                drawSeparator('=', 50); 

                setTextColor(11);   // cyn
                typeText("\t\t\t\t\t\t\t\t\t1. See All Items\n", 10);
                typeText("\t\t\t\t\t\t\t\t\t2. Take Order\n", 10);
                typeText("\t\t\t\t\t\t\t\t\t3. Give Feedback\n", 10);
                typeText("\t\t\t\t\t\t\t\t\t4. Return to Main Menu\n", 10);
                typeText("\t\t\t\t\t\t\t", 10);

                setTextColor(9); // blue
                drawSeparator('=', 50);

                setTextColor(13); // Magenta
                typeText("Enter your choice: ", 10);
                char customerMsg[] = "Enter your choice";
                speakText(customerMsg);
                scanf("%d", &customerChoice);

                clearInputBuffer();
                
            switch (customerChoice) {
            case 1:
                clearScreen();
                displayItem();
                break;
            case 2:
                clearScreen();
                takeOrder();
                break;
            case 3:
                clearScreen();
                giveFeedback();
                break;
            case 4:
                break; // Return to main menu
            default:
                setTextColor(12); // Red
                typeText("Invalid choice. Please try again.\n", 10);
                char invalidChoice[] = "Invalid choice. Please try again.";
                speakText(invalidChoice);
            }
            if (customerChoice != 4) {
                setTextColor(14); // cyan
                typeText("\n\nPress Enter to return to the Customer Menu...", 10);
                char returnMenu[] = "Press Enter to return to the Customer Menu";
                speakText(returnMenu);
                getchar();
            }
            } while (customerChoice != 4);
            break;
        }
        case 2: { // Shopkeeper Menu
            if (authenticateShopkeeper()) {
                int shopkeeperChoice;
                do {
                    clearScreen();
                    setTextColor(14); // Yellow
                    typeText("\n\t\t\t\t\t\t\t\t\tShopkeeper Menu\n", 10);
                    typeText("\t\t\t\t\t\t\t", 10);

                    setTextColor(9); // Blue
                    drawSeparator('=', 50);

                    setTextColor(11); // Light cyan
                    typeText("\t\t\t\t\t\t\t\t\t1. Add Item\n", 10);
                    typeText("\t\t\t\t\t\t\t\t\t2. See All Items\n", 10);
                    typeText("\t\t\t\t\t\t\t\t\t3. See Employee Details\n", 10);
                    typeText("\t\t\t\t\t\t\t\t\t4. Add Employee\n", 10);
                    typeText("\t\t\t\t\t\t\t\t\t5. Return to Main Menu\n", 10);
                    typeText("\t\t\t\t\t\t\t", 10);

                    setTextColor(9); // Blue
                    drawSeparator('=', 50);

                    setTextColor(13); // Magenta
                    typeText("Enter your choice: ", 10);
                    char shopkeeperMsg[] = "Enter your choice";
                    speakText(shopkeeperMsg);
                    scanf("%d", &shopkeeperChoice);
                    
                    clearInputBuffer();
                    switch (shopkeeperChoice) {
                    case 1:
                        clearScreen();
                        addItem();
                        break;
                    case 2:
                        clearScreen();
                        displayItem();
                        break;
                    case 3:
                        clearScreen();
                        seeEmployeeDetail();
                        break;
                    case 4:
                        clearScreen();
                        addEmployeeDetail();
                        break;
                    case 5:
                        break; // Return to main menu
                    default:
                        setTextColor(12); // Red
                        typeText("Invalid choice. Please try again.\n", 10);
                        char invalidChoice[] = "Invalid choice. Please try again.";
                        speakText(invalidChoice);
                        setTextColor(7); // Reset to default
                    }
                    if (shopkeeperChoice != 5) {
                        setTextColor(14); // Yellow
                        typeText("\n\n\nPress Enter to return to the Shopkeeper Menu...", 10);
                        char returnMenu[] = "Press Enter to return to the Shopkeeper Menu";
                        speakText(returnMenu);
                        getchar();
                    }
                } while (shopkeeperChoice != 5);
            }
            break;
        }
        case 3:
            clearScreen();
            showFeedback();
            break;
        case 4:
            displayExitArt();
            exit(0);
        default:
            setTextColor(12); // Red
            typeText("Invalid choice. Please try again.\n", 10);
            char invalidChoice[] = "Invalid choice. Please try again.";
            speakText(invalidChoice);
            setTextColor(7); // Reset to default
        }
        setTextColor(14); // Yellow
        typeText("\n\n\nPress Enter to return to the Main Menu...", 10);
        char returnMenu[] = "Press Enter to return to the Main Menu";
        speakText(returnMenu);
        getchar();
    }
}

void takeOrder() {
    int choice;
    int itemNumber;
    int quantity;
    int orderCount = 0;
    char confirm;
    float totalBill = 0.0;

    struct Order {
        char itemName[item_name_len];
        int quantity;
        float price;
    };

    struct Order orders[max_item_count];
    int orderIndex = 0;

    clearScreen();
    setTextColor(11); // Light cyan
    displayItem();

    
    int hellonum = 1;
    
    do {
        clearInputBuffer();
        
        while(hellonum == 1){
            typeText("\n\n\nYou can order from the above items:", 20);
            char orderMsg[] = "You can order from the above items!";
            speakText(orderMsg);
            hellonum = 0;
        }
        
        setTextColor(11); // cyan
        typeText("\nEnter item number: ", 20);

        setTextColor(8); // Gray
        scanf("%d", &itemNumber);
        clearInputBuffer();

        if (itemNumber < 1 || itemNumber > (predefCount + item_count)) {
            setTextColor(12); // Red
            typeText("Invalid item number! Please try again.\n", 20);
            char invalidItem[] = "Invalid item number! Please try again.";
            speakText(invalidItem);
            setTextColor(7); // Reset color
            continue;
        }
        setTextColor(11); // cyan
        typeText("Enter quantity: ", 20);
        setTextColor(8); // Gray
        scanf("%d", &quantity);
        clearInputBuffer();

        if (itemNumber <= predefCount) {
            setTextColor(9); // Blue
            printf("You ordered %d of %s\n", quantity, predefItems[itemNumber-1].name);
            strcpy(orders[orderIndex].itemName, predefItems[itemNumber-1].name);
            orders[orderIndex].quantity = quantity;
            orders[orderIndex].price = predefItems[itemNumber-1].price * quantity;
        } 
        else{
            setTextColor(9); // Blue
            printf("You ordered %d of %s\n", quantity, items[itemNumber-1-predefCount].name);
            strcpy(orders[orderIndex].itemName, items[itemNumber-1-predefCount].name);
            orders[orderIndex].quantity = quantity;
            orders[orderIndex].price = items[itemNumber-1-predefCount].price * quantity;
        }

        orderIndex++;
        setTextColor(14); // yellow
        printf("Do you want to add more items to the order? (y/n): ");
        scanf("%c", &confirm);

    } while (confirm == 'y' || confirm == 'Y');

    // Display order summary
    setTextColor(10); // Green
    typeText("\n\n----Order Summary----\n", 20);

    for (int i = 0; i < orderIndex; i++) {
        setTextColor(11); // Light cyan
        printf("Item: ");
        setTextColor(8); // Gray
        printf("%s\n", orders[i].itemName);

        setTextColor(11); // Light cyan
        printf("Quantity: ");
        setTextColor(8); // Gray
        printf("%d\n", orders[i].quantity);

        setTextColor(11); // Light cyan
        printf("Total Price: ");
        setTextColor(8); // Gray
        printf("%.2f\n\n", orders[i].price);

        totalBill += orders[i].price;
    }

    setTextColor(14); // yellow
    printf("Grand Total: %.2f", totalBill);

    setTextColor(13); // Magenta
    typeText("\nYour order has been successfully placed! We hope you enjoy your meal!", 20);
    char orderComplete[] = "Your order has been successfully placed! We hope you enjoy your meal!";
    speakText(orderComplete);
}

void giveFeedback() {
    clearScreen();
    char feedback[250];

    setTextColor(14); // Yellow
    typeText("Here you can give your feedback....\n\n", 20);
    char feedbackMsg[] = "Here you can give your feedback";
    speakText(feedbackMsg);

    setTextColor(11); // Light cyan
    typeText("Write your feedback: ", 20);

    setTextColor(8); // Gray
    fgets(feedback, sizeof(feedback), stdin);
    feedback[strcspn(feedback, "\n")] = 0; // Remove newline character

    // Save feedback to a file or process it as needed
    FILE *file = fopen("feedback.txt", "a");
    if (file) {
        fprintf(file, "\n%s", feedback);
        fclose(file);
        setTextColor(10); // Green
        typeText("\n\nFeedback saved successfully!\n", 20);
        char feedbackSaved[] = "Feedback saved successfully!";
        speakText(feedbackSaved);
    } 
    else{
        setTextColor(12); // Red
        errorHandling("Failed to save feedback.");
    }

    setTextColor(13); // magenta
    typeText("Thank you for your valuable feedback! Your thoughts help us improve and serve you better.\n", 20);
    char thankYou[] = "Thank you for your valuable feedback! Your thoughts help us improve and serve you better.";
    speakText(thankYou);

    setTextColor(7); // Reset to default color
}

void addEmployeeDetail() {
    FILE *file = fopen("employees.txt", "ab");
    if (!file) {
        errorHandling("Failed to open file for writing.");
        return;
    }

    struct Employee newEmployee;
    char choice;
    
    clearScreen();
    setTextColor(14); 
    typeText("You can add employee details here....\n\n", 20);
    char employeeDetails[] = "You can add employee details here";
    speakText(employeeDetails);

    do {
        setTextColor(11); // cyan
        typeText("Enter employee name: ", 20);
        setTextColor(8); // Gray
        fgets(newEmployee.name, sizeof(newEmployee.name), stdin);
        newEmployee.name[strcspn(newEmployee.name, "\n")] = 0;

        setTextColor(11); // Light cyan
        typeText("Enter employee mobile number: ", 20);
        setTextColor(8); // Gray
        fgets(newEmployee.mobileNo, sizeof(newEmployee.mobileNo), stdin);
        newEmployee.mobileNo[strcspn(newEmployee.mobileNo, "\n")] = 0;

        setTextColor(11); // Light cyan
        typeText("Enter employee Gmail ID: ", 20);
        setTextColor(8); // Gray
        fgets(newEmployee.gmail, sizeof(newEmployee.gmail), stdin);
        newEmployee.gmail[strcspn(newEmployee.gmail, "\n")] = 0;

        setTextColor(11); // Light cyan
        typeText("Enter employee salary: ", 20);
        setTextColor(8); // Gray
        scanf("%f", &newEmployee.salary);
        clearInputBuffer();

        fwrite(&newEmployee, sizeof(struct Employee), 1, file);
        printf("\n");

        setTextColor(13); // magenta
        typeText("Do you want to add another employee? (y/n): ", 20);
        scanf(" %c", &choice);
        clearInputBuffer();
        
    } while (choice == 'y' || choice == 'Y');


    setTextColor(10); // Green
    typeText("Employee added successfully and saved to file!", 20);
    char employeeAdded[] = "Employee added successfully and saved to file!";
    speakText(employeeAdded);

    fclose(file);
}

void seeEmployeeDetail() {
    FILE *file = fopen("employees.txt", "rb");
    if (!file) {
        setTextColor(12); // Red
        typeText("No employee records found.\n", 20);
        char noRecords[] = "No employee records found.";
        speakText(noRecords);
        setTextColor(7); // Reset to default color
        return;
    }

    clearScreen();
    struct Employee employee;

    setTextColor(14); // yellow
    typeText("Here are the employee details...\n\n", 20);
    char employeeDetails[] = "Here are the employee details";
    speakText(employeeDetails);

    while (fread(&employee, sizeof(struct Employee), 1, file)) {
        setTextColor(9); // Blue
        drawSeparator('-', 40);

        setTextColor(11); // Light cyan
        printf("Name: ");
        setTextColor(8); 
        printf("%s\n", employee.name);

        setTextColor(11); // Light cyan
        printf("Mobile Number: ");
        setTextColor(8); // Reset to default color
        printf("%s\n", employee.mobileNo);

        setTextColor(11); // Light cyan
        printf("Gmail ID: ");
        setTextColor(8); // Reset to default color
        printf("%s\n", employee.gmail);

        setTextColor(11); // Light cyan
        printf("Salary: ");
        setTextColor(8); // Reset to default color
        printf("%.2f\n", employee.salary);

    }
    setTextColor(9); // Blue
    drawSeparator('-', 40);

    setTextColor(7); // Reset to default color
    fclose(file);
}

int main() {
    loadItems();
    menu();
    saveItems();
    return 0;
}