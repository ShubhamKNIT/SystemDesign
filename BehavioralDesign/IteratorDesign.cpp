#include <iostream>
#include <memory>
#include <exception>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
using namespace std;

/*
    Why Iterator?
    Suppose we have a Playlist class which has a list of songs.
    We want to iterate over the songs in different ways
    like Simple, Shuffled, etc.

    We can use Iterator Design Pattern to solve this problem.
    This will help us to follow the Open/Closed Principle.
    
    We can use Iterator Design Pattern to solve this problem.
*/

// --------- Playlist ---------
class Playlist : public enable_shared_from_this<Playlist> {
private:
    vector<string> songs;
public:
    void addSong(const string& song) {
        songs.push_back(song);
    }

    vector<string> getSongs() const {
        return songs;
    }

    // will be defined later after iterators
    unique_ptr<class PlaylistIterator> iterator(const string& type);
};

// --------- Playlist Iterators ---------
class PlaylistIterator {
public:
    virtual bool hasNext() = 0;
    virtual string next() = 0;
    virtual ~PlaylistIterator() {}
};

class SimplePlaylistIterator : public PlaylistIterator {
private:
    int idx;
    shared_ptr<Playlist> playlist;
public:
    SimplePlaylistIterator(shared_ptr<Playlist> p) : playlist(p), idx(0) {}

    bool hasNext() override {
        return idx < playlist -> getSongs().size();
    }

    string next() override {
        return playlist -> getSongs().at(idx++);
    }
};

class ShuffledPlaylistIterator : public PlaylistIterator {
private:
    int idx;
    vector<string> shuffledSongs;
public:
    ShuffledPlaylistIterator(shared_ptr<Playlist> p) {
        shuffledSongs = p -> getSongs();
        random_device rd;
        mt19937 g(rd());
        shuffle(shuffledSongs.begin(), shuffledSongs.end(), g);
        idx = 0;
    }

    bool hasNext() override {
        return idx < shuffledSongs.size();
    }

    string next() override {
        return shuffledSongs.at(idx++);
    }
};

// now implement Playlist::iterator
unique_ptr<PlaylistIterator> Playlist::iterator(const string& type) {
    if (type == "Simple") {
        return make_unique<SimplePlaylistIterator>(shared_from_this());
    } else if (type == "Shuffled") {
        return make_unique<ShuffledPlaylistIterator>(shared_from_this());
    } else {
        throw runtime_error("Invalid iterator type");
    }
}

// --------- Main ---------
int main() {
    auto playlist = make_shared<Playlist>();
    playlist -> addSong("Sajna Barse");
    playlist -> addSong("Shaam Se");
    playlist -> addSong("Jee Na Paye");
    playlist -> addSong("Gazab Ka Hai Din");
    playlist -> addSong("Rait Zara Si");

    cout << "--- Simple ---\n";
    auto simplePIT = playlist -> iterator("Simple");
    while (simplePIT -> hasNext()) {
        cout << simplePIT -> next() << endl;
    }

    cout << "\n--- Shuffled ---\n";
    auto shuffledPIT = playlist -> iterator("Shuffled");
    while (shuffledPIT -> hasNext()) {
        cout << shuffledPIT -> next() << endl;
    }

    return 0;
}
