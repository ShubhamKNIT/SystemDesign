#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <exception>
#include <memory>
using namespace std;

/* 
    Why Singleton Design Pattern?
    Multiple instances should be avoided for 
    DB connection, Logging, Checkpointing, etc 
    Only one of the each instance can manage all applications 
*/

class Logger {
private:
    vector<string> logs;
    mutable mutex m; // mutable so we can lock in const functions

    Logger() = default; // private constructor

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getLogger() {
        static Logger logger;  // thread-safe init (since C++11)
        return logger;
    }

    void storeLog(const string& msg) {
        lock_guard<mutex> lock(m);   // automatic RAII locking/unlocking
        logs.push_back(msg);
    }

    void getLogs() const {
        lock_guard<mutex> lock(m);
        if (logs.empty()) {
            cout << "No logs are available\n";
        } else {
            cout << "--- Logs ---\n";
            for (const auto& log : logs) {
                cout << log << endl;
            }
        }
    }
};

class Application {
private:
    Logger& logger = Logger::getLogger();
public:
    void startApp() {
        string msg = "starting application";
        logger.storeLog(msg);
    }

    void closeApp() {
        string msg = "closing application";
        logger.storeLog(msg);
    }
};

// Simulate multiple threads writing logs
void workerTask(int id) {
    Logger::getLogger().storeLog("Worker " + to_string(id) + " started");
    this_thread::sleep_for(chrono::milliseconds(100));
    Logger::getLogger().storeLog("Worker " + to_string(id) + " finished");
}

int main() {
    Application app;
    app.startApp();

    // Launch multiple threads
    thread t1(workerTask, 1);
    thread t2(workerTask, 2);
    thread t3(workerTask, 3);

    t1.join();
    t2.join();
    t3.join();

    app.closeApp();

    cout << "Application Logs:\n";
    Logger::getLogger().getLogs();

    return 0;
}
