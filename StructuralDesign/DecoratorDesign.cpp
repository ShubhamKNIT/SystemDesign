#include <iostream>
#include <memory>
#include <string>
using namespace std;

/*
    Why Decorator?
    The Decorator Pattern is useful for adding new 
    functionality to an existing object without 
    altering its structure.
    
    It provides a flexible alternative to 
    subclassing for extending functionality.
    Open/Closed principle
*/

class Coffee {
public:
    virtual string getDescription() = 0;
    virtual double getCost() = 0;
    virtual ~Coffee() {}
};

class Mocha : public Coffee {
public:
    string getDescription() override {
        return "Mocha";
    }

    double getCost() override {
        return 3.0;
    }
};

class Espresso : public Coffee {
public:
    string getDescription() override {
        return "Espresso";
    }
    double getCost() override {
        return 4.5;
    }
};

class CoffeeDecorator : public Coffee {
protected:
    unique_ptr<Coffee> coffee;
public:
    CoffeeDecorator(unique_ptr<Coffee> coffee) {
        this -> coffee = std::move(coffee);
    }

    string getDescription() override {
        return coffee -> getDescription();
    }

    double getCost() override {
        return coffee -> getCost();
    }
};

class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(unique_ptr<Coffee> coffee) : CoffeeDecorator(std::move(coffee)) {}

    string getDescription() override {
        return coffee -> getDescription() + ", Milk";
    }

    double getCost() override {
        return coffee -> getCost() + 0.5;
    }
};

class SugarDecorator : public CoffeeDecorator {
public:
    SugarDecorator(unique_ptr<Coffee> coffee) : CoffeeDecorator(std::move(coffee)) {}

    string getDescription() override {
        return coffee -> getDescription() + ", Sugar";
    }

    double getCost() override {
        return coffee -> getCost() + 0.25;
    }
};

int main() {
    // Coffee shop

    unique_ptr<Coffee> mochaCoffee = make_unique<Mocha>();
    mochaCoffee = make_unique<MilkDecorator>(std::move(mochaCoffee));
    mochaCoffee = make_unique<SugarDecorator>(std::move(mochaCoffee));

    cout << "Order " + mochaCoffee -> getDescription() << endl;
    cout << "Total Cost: $" + to_string(mochaCoffee -> getCost()) << endl;

    // unique_ptr<Coffee> espressoCoffee = make_unique<Espresso>();
    // espressoCoffee = make_unique<MilkDecorator>(std::move(espressoCoffee));
    // espressoCoffee = make_unique<SugarDecorator>(std::move(espressoCoffee));
    unique_ptr<Coffee> espressoCoffee = make_unique<SugarDecorator>(
                                            make_unique<MilkDecorator>(
                                                make_unique<Espresso>()
                                            )
                                        );

    cout << "Order " + espressoCoffee -> getDescription() << endl;
    cout << "Total Cost: $" + to_string(espressoCoffee -> getCost()) << endl;
    
    return 0;
}