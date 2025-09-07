#include <iostream>
#include <exception>
#include <memory>
#include <string>
using namespace std;

/* 
    Why Abstract Factory Design Pattern?
    In a vehicle manufacturing system, we may have different brands
    like AstonMartin, Chervolet, etc. Each brand may have its own
    implementation for starting, moving, and stopping vehicles.

    Instead of using if-else or switch-case statements to create instances
    of different vehicle brands, we can use the Abstract Factory Design Pattern.
    This approach adheres to the Open/Closed Principle, allowing us to add
    new vehicle brands without modifying existing code.
*/

class Vehicle {
public:
    virtual void start() = 0;
    virtual void move() = 0;
    virtual void stop() = 0;
    virtual ~Vehicle() {}
};

class AstonMartin : public Vehicle {
public:
    void start()override {
        cout << "AstonMartin has started" << endl;
    }
    void move() override{
        cout << "AstonMartin is moving" << endl;
    }
    void stop() override {
        cout << "AstonMartin has stopped" << endl;
    }
    virtual ~AstonMartin() {}
};

class Chervolet : public Vehicle {
public:
    void start()override {
        cout << "Chervolet has started" << endl;
    }
    void move() override{
        cout << "Chervolet is moving" << endl;
    }
    void stop() override {
        cout << "Chervolet has stopped" << endl;
    }
    virtual ~Chervolet() {}
};


class VehicleFactory {
public:
    virtual unique_ptr<Vehicle> manufactureVehicle() = 0;
    virtual ~VehicleFactory() {}
};

class AstonMartinFactory : public VehicleFactory {
public:
    unique_ptr<Vehicle> manufactureVehicle() override {
        return make_unique<AstonMartin>();
    }
};

class ChervoletFactory : public VehicleFactory {
public:
    unique_ptr<Vehicle> manufactureVehicle() override {
        return make_unique<Chervolet>();
    }
};

int main() {
    try {
        AstonMartinFactory amf;
        auto amCar = amf.manufactureVehicle();

        amCar -> start();
        amCar -> move();
        amCar -> stop();
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
    }
}