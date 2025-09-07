#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
using namespace std;

/*
    Why Composite?
    Consider a scenario of smart home where several devices
    like smart light, smart AC etc. are present.
    Each device can be treated as a leaf node.
    A room can be treated as a composite node which can have
    multiple devices.
    A house can be treated as a composite node which can have
    multiple rooms.
    
    To control these devices using a single interface
    we can use Composite Design Pattern.
*/

class SmartComponent {
protected:
    string id;
public:
    SmartComponent(string id) : id(std::move(id)) {}
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual ~SmartComponent() {}
    string getId() const { return id; }
};

class AirConditioner : public SmartComponent {
public:
    AirConditioner(string id) : SmartComponent(id) {}
    void turnOn() override { cout << "AC (" << id << ") turned on" << endl; }
    void turnOff() override { cout << "AC (" << id << ") turned off" << endl; }
};

class SmartLight : public SmartComponent {
public:
    SmartLight(string id) : SmartComponent(id) {}
    void turnOn() override { cout << "Light (" << id << ") turned on" << endl; }
    void turnOff() override { cout << "Light (" << id << ") turned off" << endl; }
};

class CompositeSmartComponent : public SmartComponent {
private:
    unordered_map<string, unique_ptr<SmartComponent>> components;
public:
    CompositeSmartComponent(string id) : SmartComponent(id) {}

    void addComponent(unique_ptr<SmartComponent> sc) {
        string key = sc -> getId();
        components[key] = std::move(sc);  // insert or replace
    }

    void removeComponent(const string& compId) {
        if (components.erase(compId)) return;   // remove the device

        for (auto& it: components) {    // recurse down to the room to remove the device
            auto* compositeComponent = dynamic_cast<CompositeSmartComponent*>(it.second.get());
            if (compositeComponent) {
                compositeComponent -> removeComponent(compId);
            }
        }
    }

    SmartComponent* getComponent(const string& compId) {
        auto it = components.find(compId);
        return (it != components.end()) ? it -> second.get() : nullptr;
    }

    void turnOn() override {
        cout << "Turning ON group: " << id << endl;
        for (auto& [id, component] : components) {
            component -> turnOn();
        }
    }

    void turnOff() override {
        cout << "Turning OFF group: " << id << endl;
        for (auto& [id, component] : components) {
            component -> turnOff();
        }
    }
};

int main() {
    auto ac = make_unique<AirConditioner>("AC1");
    auto light = make_unique<SmartLight>("Light1");

    auto room = make_unique<CompositeSmartComponent>("Room1");
    room -> addComponent(std::move(ac));
    room -> addComponent(std::move(light));

    auto house = make_unique<CompositeSmartComponent>("House1");
    house -> addComponent(std::move(room));

    cout << "\nBefore removal:" << endl;
    house -> turnOn();
    house -> turnOff();

    cout << "\nRemoving Light1..." << endl;
    house -> removeComponent("Light1"); // remove in O(1)

    cout << "\nAfter removal:" << endl;
    house -> turnOn();
    house -> turnOff();

    return 0;
}
