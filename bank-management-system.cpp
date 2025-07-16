// Bank Management System - C++ Console Application
// Developed with OOP & File Handling Concepts

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class Account {
private:
    int accountNumber;
    string holderName;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cin.ignore();
        cout << "Enter Account Holder Name: ";
        getline(cin, holderName);
        cout << "Enter Initial Balance: ";
        cin >> balance;
        cout << "\nAccount Created Successfully!\n";
    }

    void displayAccount() const {
        cout << "\nAccount Number: " << accountNumber;
        cout << "\nAccount Holder: " << holderName;
        cout << "\nBalance: Rs." << fixed << setprecision(2) << balance << endl;
    }

    void modifyAccount() {
        cout << "\nModify Account Holder Name: ";
        cin.ignore();
        getline(cin, holderName);
        cout << "Modify Account Balance: ";
        cin >> balance;
        cout << "\nAccount Modified Successfully!\n";
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "\nInsufficient Balance! Transaction Aborted.\n";
        } else {
            balance -= amount;
        }
    }
};

void addNewAccount() {
    Account acc;
    acc.createAccount();
    ofstream outFile("accounts.dat", ios::binary | ios::app);
    if (outFile) {
        outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
        outFile.close();
    }
}

void displayAccountDetails(int accNo) {
    Account acc;
    ifstream inFile("accounts.dat", ios::binary);
    bool found = false;

    if (inFile) {
        while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            if (acc.getAccountNumber() == accNo) {
                acc.displayAccount();
                found = true;
                break;
            }
        }
        inFile.close();
    }
    if (!found) {
        cout << "\nAccount Number " << accNo << " not found.\n";
    }
}

void updateAccount(int accNo) {
    Account acc;
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    if (file) {
        while (!file.eof() && !found) {
            streampos pos = file.tellg();
            file.read(reinterpret_cast<char*>(&acc), sizeof(Account));
            if (acc.getAccountNumber() == accNo) {
                acc.displayAccount();
                acc.modifyAccount();
                file.seekp(pos);
                file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
                found = true;
                break;
            }
        }
        file.close();
    }
    if (!found) {
        cout << "\nAccount Number " << accNo << " not found for modification.\n";
    }
}

void deleteAccount(int accNo) {
    Account acc;
    ifstream inFile("accounts.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    bool found = false;

    if (inFile && outFile) {
        while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            if (acc.getAccountNumber() != accNo) {
                outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            } else {
                found = true;
            }
        }
        inFile.close();
        outFile.close();

        remove("accounts.dat");
        rename("temp.dat", "accounts.dat");
    }
    if (found) {
        cout << "\nAccount Deleted Successfully.\n";
    } else {
        cout << "\nAccount Number " << accNo << " not found for deletion.\n";
    }
}

void transactOnAccount(int accNo, double amount, bool isDeposit) {
    Account acc;
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    if (file) {
        while (!file.eof() && !found) {
            streampos pos = file.tellg();
            file.read(reinterpret_cast<char*>(&acc), sizeof(Account));
            if (acc.getAccountNumber() == accNo) {
                if (isDeposit) {
                    acc.deposit(amount);
                } else {
                    acc.withdraw(amount);
                }
                file.seekp(pos);
                file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
                found = true;
                break;
            }
        }
        file.close();
    }
    if (!found) {
        cout << "\nTransaction failed. Account Number " << accNo << " not found.\n";
    }
}

void displayAllAccounts() {
    Account acc;
    ifstream inFile("accounts.dat", ios::binary);
    if (inFile) {
        cout << "\n----- List of All Accounts -----\n";
        while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            acc.displayAccount();
            cout << "----------------------------\n";
        }
        inFile.close();
    } else {
        cout << "\nNo account records found.\n";
    }
}

int main() {
    int choice, accNo;
    double amount;

    do {
        cout << "\n===== Bank Management System Menu =====\n";
        cout << "1. Create New Account\n";
        cout << "2. Display Account Details\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Modify Existing Account\n";
        cout << "7. Delete an Account\n";
        cout << "8. Exit\n";
        cout << "Select an option (1-8): ";
        cin >> choice;

        switch (choice) {
            case 1:
                addNewAccount();
                break;
            case 2:
                cout << "\nEnter Account Number to Display: ";
                cin >> accNo;
                displayAccountDetails(accNo);
                break;
            case 3:
                cout << "\nEnter Account Number for Deposit: ";
                cin >> accNo;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                transactOnAccount(accNo, amount, true);
                break;
            case 4:
                cout << "\nEnter Account Number for Withdrawal: ";
                cin >> accNo;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                transactOnAccount(accNo, amount, false);
                break;
            case 5:
                displayAllAccounts();
                break;
            case 6:
                cout << "\nEnter Account Number to Modify: ";
                cin >> accNo;
                updateAccount(accNo);
                break;
            case 7:
                cout << "\nEnter Account Number to Delete: ";
                cin >> accNo;
                deleteAccount(accNo);
                break;
            case 8:
                cout << "\nThank you for using the Bank Management System. Goodbye!\n";
                break;
            default:
                cout << "\nInvalid Option. Please try again.\n";
        }

    } while (choice != 8);

    return 0;
}
