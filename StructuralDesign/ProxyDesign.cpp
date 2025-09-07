#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
using namespace std;

/*
    Why Proxy?
    Consider a video streaming service where users can
    watch videos. The service has two types of users:
    Free and Premium. Free users have limited access
    to videos and can only watch a certain number of videos
    in a day, while Premium users have unlimited access.

    To manage user access and optimize resource usage,
    we can use the Proxy Design Pattern.

    Here, VideoService is the subject interface
    RealVideoService is the real subject
    ProxyVideoService is the proxy that controls access to RealVideoService
*/

bool beginWith(string a, string b) {
    auto it = a.find(b);
    if (it != string::npos && it == (size_t)0) return true;
    return false;
}

class VideoService {
public:
    virtual void playVideo(string userType, string videoName) = 0;
};

class RealVideoService : public VideoService {
public:
    void playVideo(string userType, string videoName) override {
        cout << "Streaming Video: " + videoName << endl;
    }
};

class ProxyVideoService : public VideoService {
private:
    unique_ptr<RealVideoService> realVideoService;
    unordered_map<string, int> requestCounts;
    unordered_map<string, string> cachedVideos;
public:
    ProxyVideoService(unique_ptr<RealVideoService> rvs) {
        realVideoService = std::move(rvs);
    }
    
    void playVideo(string userType, string videoName) override {
        // user content rights validation
        if (userType != "Premium" && beginWith(videoName, "Premium")) {
            cout << "Access Denied: Subscribe to access premium content." << endl;
            return;
        }

        // rate limitter
        requestCounts[userType]++;
        if (requestCounts[userType] > 5) {
            cout << "Access Denied: Too many requests." << endl;
            return;
        }

        // check video in cache
        if (cachedVideos.find(videoName) != cachedVideos.end()) {
            cout << "Streaming Cached Video: " + videoName << endl;
        }
        else {
            realVideoService -> playVideo(userType, videoName);
            cachedVideos[videoName] = videoName;
        }
    }
};

int main() {
    unique_ptr<RealVideoService> realService = make_unique<RealVideoService>();
    unique_ptr<ProxyVideoService> proxyService = make_unique<ProxyVideoService>(std::move(realService));

    proxyService -> playVideo("Free", "Free Video 1");
    proxyService -> playVideo("Premium", "Premium Video 1");

    proxyService -> playVideo("Free", "Premium Video 1");

    for (int i = 0; i < 6; i++) {
        proxyService -> playVideo("Free", "Free Video 1");
    }

    return 0;
}