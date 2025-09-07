#include <iostream>
#include <memory>
#include <string>
using namespace std;

/*
    Why ChainOfResponsibility?

    The Chain of Responsibility pattern is a behavioral design pattern 
    that allows an object to pass a request along a chain of potential 
    handlers until one of them handles the request. 
    
    This pattern is useful when multiple objects can handle a request,
    but the specific handler is not known in advance.
*/

class Approver {
protected:
    shared_ptr<Approver> nextApprover;  
public:
    void setNextApprover(shared_ptr<Approver> next) {
        nextApprover = next;
    }
    virtual void processLeaveRequest(int leaveDays) = 0;
    virtual ~Approver() = default;
};

class Supervisor : public Approver {
public:
    void processLeaveRequest(int leaveDays) override {
        if (leaveDays <= 3) {
            cout << "Supervisor approved the leave\n";
        } 
        else if (nextApprover) {
            nextApprover -> processLeaveRequest(leaveDays);
        }
    }
};

class Manager : public Approver {
public:
    void processLeaveRequest(int leaveDays) override {
        if (leaveDays <= 7) {
            cout << "Manager approved the leave\n";
        } 
        else if (nextApprover) {
            nextApprover -> processLeaveRequest(leaveDays);
        }
    }
};

class Director : public Approver {
public:
    void processLeaveRequest(int leaveDays) override {
        if (leaveDays <= 14) {
            cout << "Director approved the leave\n";
        } 
        else if (nextApprover != nullptr) {
            nextApprover -> processLeaveRequest(leaveDays);
        }
        else {
            cout << "Leave request denied\n";
        }
    }
};

int main() {
    auto supervisor = make_shared<Supervisor>();
    auto manager = make_shared<Manager>();
    auto director = make_shared<Director>();

    supervisor -> setNextApprover(manager);
    manager -> setNextApprover(director);

    int leaveDays = 16;
    cout << "Employee requests " << leaveDays << " days of leave.\n";
    supervisor -> processLeaveRequest(leaveDays);

    return 0;
}
