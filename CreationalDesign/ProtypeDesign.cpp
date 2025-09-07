#include <iostream>
#include <exception>
using namespace std;

class Character {
private:
    string name;
    int health;
    int attackPower;
    int level;
public:
    Character(string name, int health, int attackPower, int level) { 
        this -> name = name;
        this -> health = health;
        this -> attackPower = attackPower;
        this -> level = level;
    }

    unique_ptr<Character> clone() const {
        return make_unique<Character>(name, health, attackPower, level);
    }

    void setName(const string& newName) {
        name = newName;
    }

    void setHealth(int newHealth) {
        health = newHealth;
    }

    void setAttackPower(int newAttackPower) {
        attackPower = newAttackPower;
    }

    void setLevel(int newLevel) {
        level = newLevel;
    }

    void display() const {
        cout << "Name: " << name << ", Health: " << health 
             << ", Attack Power: " << attackPower << ", Level: " << level << endl;
    }

    virtual ~Character() {
        // cout << name << " destroyed\n";
    }
};

class CharacterFactory {
private:
    Character prototypeCharacter;
public:
    CharacterFactory() : prototypeCharacter("Default Hero", 100, 50, 1) {}

    unique_ptr<Character> createCharacterWithNewName(const string& newName) const {
        unique_ptr<Character> newCharacter = prototypeCharacter.clone();
        newCharacter -> setName(newName);
        return newCharacter;
    }

    unique_ptr<Character> createCharacterWithHealth(int newHealth) const {
        unique_ptr<Character> newCharacter = prototypeCharacter.clone();
        newCharacter -> setHealth(newHealth);
        return newCharacter;
    }

    unique_ptr<Character> createCharacterWithAttackPower(int newAttackPower) const {
        unique_ptr<Character> newCharacter = prototypeCharacter.clone();
        newCharacter -> setAttackPower(newAttackPower);
        return newCharacter;
    }
    unique_ptr<Character> createCharacterWithLevel(int newLevel) const {
        unique_ptr<Character> newCharacter = prototypeCharacter.clone();
        newCharacter -> setLevel(newLevel);
        return newCharacter;
    }
};

int main() {
    CharacterFactory factory;
    unique_ptr<Character> hero1 = factory.createCharacterWithNewName("Warrior");
    unique_ptr<Character> hero2 = factory.createCharacterWithHealth(150);
    unique_ptr<Character> hero3 = factory.createCharacterWithAttackPower(70);

    hero1 -> display();
    hero2 -> display();
    hero3 -> display();

    return 0;
}