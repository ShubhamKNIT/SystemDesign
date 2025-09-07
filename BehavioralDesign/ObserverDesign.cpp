#include <iostream>
#include <exception>
#include <memory>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
using namespace std;


/*
    Why Observer?
    Subscribers can subscribe to a channel
    and get notified when a new video is uploaded.
    With different Subscriber strategies
    like Email, Push, SMS, etc.

    Later if we want to add more Subscriber strategies
    we don't want to change the YoutubeChannel class.
    So we can use the Observer Design Pattern here.
    This will help us to follow the Open/Closed Principle.
    
    We can use Observer Design Pattern to solve this problem.
*/

class Subscriber {
public:
    virtual void notify(const string& video) = 0;
    virtual ~Subscriber() {}
};

class EmailSubscriber: public Subscriber {
private:
    string email;
public:
    EmailSubscriber(const string& email) : email(email) {}
    void notify(const string& video) override {
        cout << "Email: New video out " + video << endl;
    }
};

class PushSubscriber: public Subscriber {
private:
    string deviceToken;
public:
    PushSubscriber(const string& deviceToken) : deviceToken(deviceToken) {}
    void notify(const string& video) override {
        cout << "Push: New video out " + video << endl;
    }
};

class YoutubeChannel {
private:
    const string& uid;
    const string& name;
    vector<shared_ptr<Subscriber>> Subscribers;
public:
    YoutubeChannel (const string& uid, const string& name) : uid(uid), name(name) {}

    void addSubscriber(shared_ptr<Subscriber> sub) {
        Subscribers.push_back(sub);
    }

    void removeSubscriber(const shared_ptr<Subscriber>& sub) {
        Subscribers.erase(remove(Subscribers.begin(), Subscribers.end(), sub), Subscribers.end());
    }

    void uploadContent(const string& video) {
        for (auto& Subscriber : Subscribers) {
            Subscriber -> notify(video);
        }
    }
};

int main() {
    YoutubeChannel yt("11", "CypherJet");

    auto sub1 = make_shared<EmailSubscriber>("example@example.com");
    auto sub2 = make_shared<PushSubscriber>("device_token_123");

    yt.addSubscriber(sub1);
    yt.addSubscriber(sub2);

    yt.uploadContent("LLM-DB-Search");

    yt.removeSubscriber(sub1);
    yt.removeSubscriber(sub2);

    return 0;
}