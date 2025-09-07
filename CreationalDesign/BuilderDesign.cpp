#include <iostream>
#include <string>
#include <exception>
#include <memory>
using namespace std;

/* 
    Why Builder Design Pattern?
    In a car manufacturing system, we may have different configurations
    for cars like engine type, number of wheels, color, seats, sunroof, etc.

    Instead of creating multiple constructors or using complex initialization
    logic, we can use the Builder Design Pattern to construct car objects
    step by step. This approach adheres to the Single Responsibility Principle,
    allowing us to separate the construction logic from the car representation.
*/

class Car {
public:
    class CarBuilder;
private:
    string engine;
    int wheels;
    int seats;
    string color;
    bool sunroof;
    bool navigationSys;

    // Private constructor – only Builder can call
    Car(const CarBuilder& builder)
        : engine(builder.engine),
          wheels(builder.wheels),
          seats(builder.seats),
          color(builder.color),
          sunroof(builder.sunroof),
          navigationSys(builder.navigationSys) {}
public:
    string toString() const {
        return "Car specs:\nEngine: " + engine +
               "\nWheels: " + to_string(wheels) +
               "\nSeats: " + to_string(seats) +
               "\nColor: " + color +
               "\nSunroof: " + string(sunroof ? "Yes" : "No") +
               "\nNavigation: " + string(navigationSys ? "Yes" : "No");
    }

    // -------- Nested Builder Class --------
    class CarBuilder {
    private:
        string engine = "V8";
        int wheels = 4;
        int seats = 4;
        string color = "Black";
        bool sunroof = false;
        bool navigationSys = false;

    public:
        // Chained setters (return builder reference)
        CarBuilder& setEngine(const string& e) { engine = e; return *this; }
        CarBuilder& setWheels(int w) { wheels = w; return *this; }
        CarBuilder& setSeats(int s) { seats = s; return *this; }
        CarBuilder& setColor(const string& c) { color = c; return *this; }
        CarBuilder& setSunroof(bool sr) { sunroof = sr; return *this; }
        CarBuilder& setNavigationSys(bool nav) { navigationSys = nav; return *this; }

        // Final build
        Car build() const { return Car(*this); }

        // Allow Car to access builder fields
        friend class Car;
    };
};
      

// -------- Example Usage --------
int main() {
    Car car = Car::CarBuilder()
                    .setEngine("V12")
                    .setColor("Red")
                    .setSeats(2)
                    .setSunroof(true)
                    .build();

    cout << car.toString() << endl;
}
