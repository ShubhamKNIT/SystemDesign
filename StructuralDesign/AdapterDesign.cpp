#include <iostream>
#include <memory>
#include <string>
using namespace std;

/*
    Why Adapter?
    Consider a scenario of smart home where several devices
    like smart light, smart AC etc. are present.
    Each device has its own way of communication like
    smart light uses WiFi and smart AC uses Bluetooth.
    To control these devices using a single interface
    we can use Adapter Design Pattern.

    Here, SmartDevice is the target interface
    AirConditioner and SmartLight are the devices
    AirConditionerAdapter and SmartLightAdapter are the adapters
*/

class SmartDevice {
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual ~SmartDevice() {}
};

class AirConditioner {
public:
    void connectViaBluetooth() {
        cout << "AC connected via bluetooth" << endl;
    }

    void startCooling() {
        cout << "AC started cooling" << endl;
    }

    void stopCooling() {
        cout << "AC stopped cooling" << endl;
    }

    void disconnectBluetooth() {
        cout << "AC disconnected" << endl;
    }
};

class SmartLight {
public:
    void connectToWiFi() {
        cout << "Smart Light connected to WiFi" << endl;
    }

    void switchOn() {
        cout << "Smart Light is switched on" << endl;
    }

    void switchOff() {
        cout << "Smart Light is switched off" << endl;
    }

    void disconnectWiFi() {
        cout << "Smart Light disconnected from WiFi" << endl;
    }
};

class AirConditionerAdapter : public SmartDevice {
private:
    unique_ptr<AirConditioner> airConditioner;
public:
    AirConditionerAdapter(unique_ptr<AirConditioner> ac) {
        airConditioner = std::move(ac);
    }

    void turnOn() override {
        airConditioner -> connectViaBluetooth();
        airConditioner -> startCooling();
    }

    void turnOff() override {
        airConditioner -> stopCooling();
        airConditioner -> disconnectBluetooth();
    }
};

class SmartLightAdapter : public SmartDevice {
private:
    unique_ptr<SmartLight> smartLight;
public:
    SmartLightAdapter(unique_ptr<SmartLight> sm) {
        smartLight = std::move(sm);
    }

    void turnOn() override {
        smartLight -> connectToWiFi();
        smartLight -> switchOn();
    }

    void turnOff() override {
        smartLight -> switchOff();
        smartLight -> disconnectWiFi();
    }
};

int main() {
    // Smart Home Controller

    auto smartLight = make_unique<SmartLight>();
    auto airConditioner = make_unique<AirConditioner>();

    auto smartLightAdapter = make_unique<SmartLightAdapter>(std::move(smartLight));
    auto airConditionerAdapter = make_unique<AirConditionerAdapter>(std::move(airConditioner));

    smartLightAdapter -> turnOn();
    airConditionerAdapter -> turnOn();
    
    airConditionerAdapter -> turnOff();
    smartLightAdapter -> turnOff();

    return 0;
}