//Transport.h
#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <iostream>
#include <string>
using namespace std;

class Transport {
protected:
    string id;
    string name;
    int maxPeople;
    int maxWeight;
    string category;
    bool available=true;

public:

    Transport(string id_, string name_, string category_, int p, int w)
        : id(id_), name(name_), category(category_), maxPeople(p), maxWeight(w) {}
    Transport()=default;
    virtual ~Transport() = default;
    virtual bool isAvailable() const { return available; }
    virtual void setAvailable(bool status) { available = status; }
    void setID(string i){id=i;}
    void setName(string n){name=n;}
    void setCategory(string c)  { category=c; }
    void setPeople(int p){maxPeople=p;}
    void setWeight(int w){maxWeight=w;}

    string getID() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    int getPeople() const { return maxPeople; }
    int getWeight() const { return maxWeight; }

    // Pure virtual function for abstraction
    virtual string getType() const = 0;
    virtual double getFuel() const = 0;
    virtual double getSpeed() const = 0;
    virtual string serialize() const {
    return id + "," + name + "," + category + "," +
           to_string(maxPeople) + "," + to_string(maxWeight)
            +"," + (available?"1":"0");
}
    virtual void displayInfo() const {
        cout << "ID: " << id
             << "\n Name: " << name
             << "\n Type: " << getType()
             << "\n Maximum People: " << maxPeople
             << "\n Maximum Weight: " << maxWeight << "kg"
             << "\n Maximum Speed: " << getSpeed() << "km/h"
             << "\n Fuel Comsumption: " << getFuel() << "L/km" << endl;
    }

};

#endif //TRANSPORT_H



#endif //TRANSPORT_H







