#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initial account balance
float balance = 10000.0;

// Transaction history for mini statement
char miniStatement[3][50];
int statementIndex = 0;
int withdrawCount = 0;

int langMode = 1;       // 1 = English, 2 = Hindi
int storedPIN = -1;     // User PIN

// Draw border box
void drawBox() {
    printf("---------------------------------------------------------------\n");
}

// Add transaction to mini statement
void addTransaction(const char *msg) {
    if (statementIndex < 3) {
        strcpy(miniStatement[statementIndex], msg);
        statementIndex++;
    } else {
        // Pehla transaction hata ke naya add karo (FIFO)
        for (int i = 0; i < 2; i++) {
            strcpy(miniStatement[i], miniStatement[i + 1]);
        }
        strcpy(miniStatement[2], msg);
    }
}

// User PIN set karna
int registerPIN() {
    int pin1, pin2;
    if (langMode == 1)
        printf("\nSet your 4-digit ATM PIN: ");
    else
        printf("\n4-ankhon wali ATM PIN set kijiye: ");
    scanf("%d", &pin1);

    if (langMode == 1)
        printf("Confirm your PIN: ");
    else
        printf("PIN dobara daalein: ");
    scanf("%d", &pin2);

    if (pin1 == pin2) {
        // PIN match ho gaya
        if (langMode == 1)
            printf("PIN registered successfully.\n");
        else
            printf("PIN safaltapoorvak set ho gayi.\n");
        return pin1;
    } else {
        // PIN mismatch
        if (langMode == 1)
            printf("PINs do not match. Try again.\n");
        else
            printf("PIN match nahi kar rahi. Dobara koshish karein.\n");
        return -1;
    }
}

// PIN verify karna (3 attempts allowed)
int authenticateUser(int setPin) {
    int inputPin, attempts = 0;
    while (attempts < 3) {
        if (langMode == 1)
            printf("Enter your PIN: ");
        else
            printf("Apni PIN daalein: ");
        scanf("%d", &inputPin);

        if (inputPin == setPin) {
            if (langMode == 1)
                printf("PIN verified. Welcome!\n");
            else
                printf("PIN sahi hai. Swagat hai!\n");
            return 1;
        } else {
            attempts++;
            int left = 3 - attempts;
            if (left == 0) {
                // 3 galat attempts ke baad exit
                if (langMode == 1)
                    printf("3 wrong attempts. Exiting...\n");
                else
                    printf("3 galat koshishon ke baad ATM band ho raha hai...\n");
                exit(0);
            } else {
                if (langMode == 1)
                    printf("Wrong PIN. Attempts left: %d\n", left);
                else
                    printf("Galat PIN. Bache hue mauke: %d\n", left);
            }
        }
    }
    return 0;
}

// Mini statement print karna
void showMiniStatement() {
    if (langMode == 1)
        printf("\nLast 3 Transactions:\n");
    else
        printf("\nAakhri 3 len-den:\n");

    for (int i = 0; i < statementIndex; i++) {
        printf("%s\n", miniStatement[i]);
    }

    if (statementIndex == 0) {
        if (langMode == 1)
            printf("No transactions yet.\n");
        else
            printf("Koi transaction nahi hua ab tak.\n");
    }
}

// Show current balance
void checkBalance() {
    if (langMode == 1)
        printf("\nYour balance: ₹%.2f\n", balance);
    else
        printf("\nAapka balance hai: ₹%.2f\n", balance);
}

// Deposit money into account
void depositMoney() {
    float amount;
    if (langMode == 1)
        printf("Enter deposit amount: ₹");
    else
        printf("Kitni rakam jama karni hai: ₹");
    scanf("%f", &amount);

    if (amount <= 0) {
        // Negative ya 0 amount
        if (langMode == 1)
            printf("Invalid amount.\n");
        else
            printf("Galat rashi.\n");
    } else {
        balance += amount;
        char msg[50];
        sprintf(msg, "Deposit: +₹%.2f", amount);
        addTransaction(msg);

        if (langMode == 1)
            printf("₹%.2f deposited. New balance: ₹%.2f\n", amount, balance);
        else
            printf("₹%.2f jama. Naya balance: ₹%.2f\n", amount, balance);
    }
}

// Withdraw money from account
void withdrawMoney() {
    if (withdrawCount >= 3) {
        // 3 baar se zyada withdraw nahi
        if (langMode == 1)
            printf("Withdrawal limit (3) reached for this session.\n");
        else
            printf("3 baar se zyada withdraw nahi kar sakte.\n");
        return;
    }

    float amount;
    if (langMode == 1)
        printf("Enter amount to withdraw: ₹");
    else
        printf("Kitni rakam nikalni hai: ₹");
    scanf("%f", &amount);

    if (amount <= 0 || amount > balance) {
        // Invalid ya insufficient amount
        if (langMode == 1)
            printf("Invalid or insufficient balance.\n");
        else
            printf("Galat ya kam balance.\n");
    } else {
        balance -= amount;
        withdrawCount++;
        char msg[50];
        sprintf(msg, "Withdraw: -₹%.2f", amount);
        addTransaction(msg);

        if (langMode == 1)
            printf("Please collect your cash. Balance: ₹%.2f\n", balance);
        else
            printf("Cash lein. Bacha hua balance: ₹%.2f\n", balance);
    }
}

// Show main menu
void showMenu() {
    int choice;
    do {
        drawBox();
        if (langMode == 1) {
            printf("|                 ATM MAIN MENU                  |\n");
            printf("| 1. Check Balance                               |\n");
            printf("| 2. Deposit Money                               |\n");
            printf("| 3. Withdraw Money                              |\n");
            printf("| 4. Mini Statement                              |\n");
            printf("| 5. Exit                                        |\n");
        } else {
            printf("|                ATM MUKHYA MENU                |\n");
            printf("| 1. Balance Dekhein                            |\n");
            printf("| 2. Jama Karein                                |\n");
            printf("| 3. Paise Nikalein                             |\n");
            printf("| 4. Mini Statement                             |\n");
            printf("| 5. Bahar Niklein                              |\n");
        }
        drawBox();

        if (langMode == 1)
            printf("Enter your choice: ");
        else
            printf("Apna vikalp chuniye: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: checkBalance(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: showMiniStatement(); break;
            case 5:
                if (langMode == 1)
                    printf("Thank you! Visit again.\n");
                else
                    printf("Shukriya! Phir milenge.\n");
                break;
            default:
                if (langMode == 1)
                    printf("Invalid option. Try again.\n");
                else
                    printf("Galat vikalp. Dobara try karein.\n");
        }

    } while (choice != 5);
}

// Language selection
void selectLanguage() {
    drawBox();
    printf("| Choose Language / Bhasha Chuniye               |\n");
    printf("| 1. English                                     |\n");
    printf("| 2. Hindi                                       |\n");
    drawBox();
    printf("Enter your choice: ");
    scanf("%d", &langMode);
    if (langMode != 1 && langMode != 2) langMode = 1;
}

// Main ATM program start
int main() {
    selectLanguage();

    if (langMode == 1)
        printf("\nWelcome to Smart ATM!\n");
    else
        printf("\nSmart ATM mein aapka swagat hai!\n");

    printf("\n1. Register (New User)\n2. Login\n");
    printf(langMode == 1 ? "Choose an option: " : "Vikalp chuniye: ");
    int opt;
    scanf("%d", &opt);

    if (opt == 1) {
        storedPIN = registerPIN();
        if (storedPIN == -1) return 0;
    }

    if (storedPIN == -1) {
        if (langMode == 1)
            printf("Please register first.\n");
        else
            printf("Pehle register karein.\n");
        return 0;
    }

    if (authenticateUser(storedPIN)) {
        showMenu();
    }

    return 0;
}
