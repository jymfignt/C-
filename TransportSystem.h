// TransportSystem.h
#ifndef TRANSPORTSYSTEM_H
#define TRANSPORTSYSTEM_H
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include "LandTransport.h"
#include "WaterTransport.h"
#include "AirTransport.h"
#include "Client.h"
using namespace std;

class TransportSystem {
private:
    vector<shared_ptr<Transport>> fleet;
    map<string, vector<string>> bookings; // contact -> transport IDs
    map<string, Client> clientInfo;
    void loadFleetFromFile(const string& filename)
    {
        fleet.clear();
        ifstream fin(filename);
        string line;
        while (getline(fin, line))
        {
            stringstream ss(line);
            string id,name,category,availableStr;
            int p, w;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, category, ',');
            ss >> p;
            ss.ignore();
            ss >> w;
            ss.ignore();
            getline(ss, availableStr);
            try {
            auto transport = createTransportByCategory(id, name, category, p, w);

            // set available
            bool isAvailable = (availableStr == "1");
            transport->setAvailable(isAvailable);

            fleet.push_back(transport);
            } catch (...) {
            cout << "Error loading transport: " << line << endl;
            }
        }
    }
public:
    TransportSystem() {reloadFleet();}

    void reloadFleet() {loadFleetFromFile("fleet.txt"); }
    void saveFleetToFile(const string& filename)
    {
        if (fleet.empty()) {
        cout << "Warning: Fleet is empty. "
         <<"Skipping saving to file to prevent overwriting." << endl;
        return;
    }
        ofstream fout(filename);
        for (auto& t : fleet) {
            fout << t->serialize() << endl;
        }
    }
    void listFleet() {
    for (const auto& t : fleet) {
        cout << t->getID() << " | "
             << t->getName() << " | "
             << t->getCategory() << " | "
             << t->getPeople() << " seats | "
             << t->getWeight() << " kg | "
             << "Fuel: " << t->getFuel() << "L | "
             << "Speed: " << t->getSpeed() << "km/h\n";
    }
}
    shared_ptr<Transport> createTransportByCategory(string id, string name, string category, int p, int w)
    {
    if (category == "land") return make_shared<LandTransport>(id, name, p, w);
    if (category == "water") return make_shared<WaterTransport>(id, name, p, w);
    if (category == "air") return make_shared<AirTransport>(id, name, p, w);
    throw invalid_argument("Unknown category: " + category);
    }
    vector<shared_ptr<Transport>> matchTransport(int people, int weight) //查找合适交通工具,如果有返回
    {
        vector<shared_ptr<Transport>> result;
        for (const auto& t : fleet) {
            if (t->getPeople() >= people && t->getWeight() >= weight) {
                result.push_back(t);
            }
        }
        return result;
    }
//customer information, selected vector
    void recordBooking(Client client, const vector<shared_ptr<Transport>>& transports)
    {
        vector<string> ids;
         for (auto& t : transports)
        {
            ids.push_back(t->getID());
            t->setAvailable(false);
        }
    // If there is a previous reservation, it will be appended
    bookings[client.contact].insert(bookings[client.contact].end(), ids.begin(), ids.end());
    clientInfo[client.contact] = client;
    }
    void saveBookingsToFile(const string& filename)
{
    ofstream fout(filename);
    for (const auto& entry : bookings)
    {
        const string& contact = entry.first;
        const vector<string>& ids = entry.second;
        const Client& c = clientInfo[contact];
        fout << "Client Name: " << c.name << endl;
        fout << "Contact Number: " << c.contact << endl;
        fout << "Booked: ";
        for (const auto& id : ids) fout << id << ",";
        fout << "\n---\n";
    }
}
    void addNewTransport()
    {
        string id, name, category;
        int p, w;
        cout << "Enter ID: "; cin >> id;
        cout << "Enter Name: "; cin >> name;
        cout << "Enter Category (land/water/air): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, category);
        cout << "Enter max people: ";
        while (!(cin >> p))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter a number for max people: ";
        }
        cout << "Enter max weight: ";
        while (!(cin >> w))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter a number for max weight: ";
        }
        try {
            auto t = createTransportByCategory(id, name, category, p, w);
            fleet.push_back(t);
            cout << "Transport added successfully.\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
}
    void deleteTransport(string id) {
        for (auto it = fleet.begin(); it != fleet.end(); ++it) {
            if ((*it)->getID() == id) {
                fleet.erase(it);
                cout << "Deleted." << endl;
                return;
            }
        }
        cout << "Not found." << endl;
    }
    void modifyTransport(string id, int newPeople, int newWeight) {
    for (auto it = fleet.begin(); it != fleet.end(); ++it) {
        if ((*it)->getID() == id) {
            string name = (*it)->getName();
            string category = (*it)->getCategory();

            fleet.erase(it);
            try {
    auto newT = createTransportByCategory(id, name, category, newPeople, newWeight);
    fleet.erase(it);
    fleet.push_back(newT);
    cout << "Modified." << endl;
} catch (...) {
    cout << "Modification failed. Keeping original." << endl;
}
            return;
        }
    }
    cout << "Not found." << endl;
 }
};
#endif //TRANSPORTSYSTEM_H





