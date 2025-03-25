// kevin g assignment 3
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// Custom exceptions for different types of transaction errors

class NegativeDepositException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Deposit amount can't be negative.";
    }
};

class OverdrawException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Withdrawal amount is invalid.";
    }
};

class InvalidAccountOperationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Account is closed.";
    }
};

// Basic bank account class to manage balance and status

class BankAccount {
private:
    std::string accountNumber;
    double balance;
    bool isActive;

public:
    // initializes account with an account number and starting balance
    BankAccount(std::string accNum, double initialBalance)
        : accountNumber(std::move(accNum)), balance(initialBalance), isActive(true) {}

    // Adds money to the account (if it's active and the amount is valid)
    void deposit(double amount) {
        if (!isActive) throw InvalidAccountOperationException();
        if (amount < 0) throw NegativeDepositException();
        balance += amount;
    }

    // Withdraws money from account and checks for closed accts ect
    void withdraw(double amount) {
        if (!isActive) throw InvalidAccountOperationException();
        if (amount < 0 || amount > balance) throw OverdrawException();
        balance -= amount;
    }

    // Returns the current balance
    double getBalance() const {
        return balance;
    }

    // Returns the account number
    std::string getAccountNumber() const {
        return accountNumber;
    }

    // Marks the account as closed
    void closeAccount() {
        isActive = false;
    }
};

int main() {
    // Create a new account with a balance of $1000
    auto account = std::make_unique<BankAccount>("123456", 1000.0);
    std::cout << "Bank Account Created: #" << account->getAccountNumber() << std::endl;

    try {
        // Ask the user for deposit amount and try to deposit it
        double depositAmount;
        std::cout << "Enter deposit amount: $";
        std::cin >> depositAmount;
        account->deposit(depositAmount);
        std::cout << "Deposited $" << depositAmount << " successfully.\n";

        // Ask the user for a withdrawal amount and try to withdraw it
        double withdrawAmount;
        std::cout << "Enter withdrawal amount: $";
        std::cin >> withdrawAmount;
        account->withdraw(withdrawAmount);
        std::cout << "Withdrew $" << withdrawAmount << " successfully.\n";

        // Show current balance after.
        std::cout << "Current Balance: $" << account->getBalance() << std::endl;

        // Try to withdraw more than the current balance — this should throw an error
        std::cout << "Attempting to withdraw $600...\n";
        account->withdraw(600); // Should trigger OverdrawException
    }
    catch (const std::exception& e) {
        // Catch and display any error that occurs during the transactions
        std::cerr << "Transaction failed: " << e.what() << std::endl;
    }

    return 0;
}

