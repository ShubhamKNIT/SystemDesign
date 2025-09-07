#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int main() {

    auto fun = [](int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    };

    thread t(fun, 10);
/*
    mutex m;
    m.lock();

    lock_guard<mutex> lock(m);

    if (m.try_lock()) // try locking 

    // Criticall Section Code

    m.unlock()
*/

/*

    thread t(function, params...);

    t.join();   // wait for thread // cna be used only once

    if (t.joinable()) { // if thread is joinable 
        t.join();   // waith for thread
    }

    t.detach(); // do not wait 

*/

    t.join();

    return 0;
}