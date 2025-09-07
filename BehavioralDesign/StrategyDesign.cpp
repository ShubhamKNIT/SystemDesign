#include <iostream>
#include <exception>
#include <memory>
#include <string>
using namespace std;

#define move std::move 

/*
    Why Strategy Design Pattern?
    Like for payment processing several methods
    are available like credit card, debit card, UPI, net banking etc.

    Using if-else or switch-case for selecting payment method
    is not a good idea as it violates Open/Closed Principle.
    So, we use Strategy Design Pattern to solve this problem.
*/

class PaymentStrategy {
public:
    virtual void processPayment(int amt) = 0;
    virtual ~PaymentStrategy() {}
};

class CreditCard : public PaymentStrategy {
public:
    void processPayment(int amt) override {
        cout <<  to_string(amt) + " Paid using CreditCard" << endl;
    }
};

class DebitCard : public PaymentStrategy {
public:
    void processPayment(int amt) override {
        cout <<  to_string(amt) + " Paid using DebitCard" << endl;
    }
};

class PaymentProcessor {
private:
    unique_ptr<PaymentStrategy> paymentStrategy;
public:
    PaymentProcessor(unique_ptr<PaymentStrategy> pm) : paymentStrategy(move(pm)){}

    void processPayment(int amt) {
        paymentStrategy -> processPayment(amt);
    }

    void setPaymentStrategy(unique_ptr<PaymentStrategy> pm) {
        paymentStrategy = move(pm);
    }
};

int main() {
    auto cc = make_unique<CreditCard>();
    auto dc = make_unique<DebitCard>();

    PaymentProcessor pp(move(cc));
    pp.processPayment(1000);

    pp.setPaymentStrategy(move(dc));
    pp.processPayment(2000);

    return 0;
}