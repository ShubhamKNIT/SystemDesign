#include <iostream>
#include <exception>
#include <memory>
#include <string> 
using namespace std;

/*
    Why State?

    An object can change its behavior when its internal state changes.
    The object will appear to change its class.
    Example: Traffic Light System
    States
    - Red
    - Yellow
    - Green

    Transitions
    - Red -> Yellow
    - Yellow -> Green
    - Green -> Red
*/

class TrafficLightContext; // maintains current state

class TrafficLightState {
public:
    virtual void next(TrafficLightContext* context) = 0;
    virtual string getColor() = 0;
    virtual ~TrafficLightState() {}
};

class RedState;
class YellowState;
class GreenState;

class GreenState : public TrafficLightState {
public:
    void next(TrafficLightContext* context) override;

    string getColor() override {
        return "GREEN";
    }
};

class YellowState : public TrafficLightState {
public:
    void next(TrafficLightContext* context) override;
    string getColor() override {
        return "YELLOW";
    }
};

class RedState : public TrafficLightState {
public:
    void next(TrafficLightContext* context) override;

    string getColor() override {
        return "RED";
    }
};

class TrafficLightContext {
private:
    unique_ptr<TrafficLightState> currState;
public:
    TrafficLightContext() {
        currState = make_unique<RedState>();
    }

    void setState(unique_ptr<TrafficLightState> state) {
        currState = std::move(state);
    }

    void next() {
        currState -> next(this);
    }

    string getColor() {
        return currState -> getColor();
    }
};

void RedState::next(TrafficLightContext* context) {
    cout << "Switch from RED to YELLOW" << endl;
    context -> setState(make_unique<YellowState>());
}

void YellowState::next(TrafficLightContext* context) {
    cout << "Switch from YELLOW to GREEN" << endl;
    context -> setState(make_unique<GreenState>());
}

void GreenState::next(TrafficLightContext* context) {
    cout << "Switch from GREEN to RED" << endl;
    context -> setState(make_unique<RedState>());
}

int main() {
    TrafficLightContext tlc;
    for (int i = 0; i < 6; i++) {
        cout << "Current Color: " << tlc.getColor() << endl;
        tlc.next();
    }
    return 0;
}