#include <iostream>
#include <exception>
using namespace std;

class Vehicle {
public:
    virtual void start() = 0;
    virtual void move() = 0;
    virtual void stop() = 0;
    virtual ~Vehicle() {}
};

class Bike : public Vehicle {
public:
    void start() override {
        cout << "Bike has started" << endl;
    }
    void move() override {
        cout << "Bike is moving" << endl;
    }
    void stop() override {
        cout << "Bike has stopped" << endl;
    }
};

class Car : public Vehicle {
public:
    void start()override {
        cout << "Car has started" << endl;
    }
    void move() override{
        cout << "Car is moving" << endl;
    }
    void stop() override {
        cout << "Car has stopped" << endl;
    }
};

class VehicleFactory {
public:
    static unique_ptr<Vehicle> manufactureVehicle(const string& vtype) {
        if (vtype == "Car") {
            return make_unique<Car>();
        } 
        else if (vtype == "Bike") {
            return make_unique<Bike>();
        } 
        else {
            throw runtime_error("Bad input value");
        }
    }
};

int main() {

    try {
        auto car = VehicleFactory::manufactureVehicle("Car");
        car -> start();
        car -> move();
        car -> stop();

        // delete car; // since unique_ptr
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}