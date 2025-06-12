#include <iostream>
#include "TransportSystem.h"
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

void rgb_init(){    //prevent ANSI garbled characters
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);   //input handle
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);   //output handle
    DWORD dwInMode, dwOutMode;
    GetConsoleMode(hIn, &dwInMode);   
    GetConsoleMode(hOut, &dwOutMode); 
    dwInMode |= 0x0200;
    dwOutMode |=0x0004;
    SetConsoleMode(hIn,dwInMode);    
    SetConsoleMode(hOut,dwOutMode);    
}
void rgb_set(int wr, int wg,int wb,int br,int bg, int bb){
    printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%dm",wr,wg,wb,br,bg,bb);
    // \033[38 indicates forehead，\033[48 indicates background，三个%d indicates mixed numbers
}
void customerMenu(TransportSystem& ts) //iteraction with customer
{
    ts.reloadFleet();   //read the txt file
    string name, contact;
    int people, weight;
    cout<<"\nWelcome to our company to choose transportation!\n";
    cout << "Enter your name: "; cin >> name;
    cout << "Enter the contact number: "; cin >> contact;
    cout<<"Got it.\n"
    <<"\nWe have multiple modes of transportation that may carry people or goods or both.\n";
    cout << "Enter the number of people: "; cin >> people;
    cout << "Enter total weight of goods(kg): "; cin >> weight;

    auto matched = ts.matchTransport(people, weight);  //finding the match one
    if (matched.empty()) {
        cout << "No suitable transport found." << endl;
        return;
    }
//else, has matching ones
   cout << "\nAvailable options:\n";
    for (size_t i = 0; i < matched.size(); ++i) //notice: ++i
    {
        cout << i + 1 << ". " << matched[i]->getID()
        << " - " << matched[i]->getName()
        << " (" << matched[i]->getType() << ")" ;

    //cout << " [Debug: Available=" << matched[i]->isAvailable() << "]";
        if (!matched[i]->isAvailable()) {
        cout << " [Already Booked]";
    }
    cout << endl;
    }
    cout << "\nEnter indices of transports to book (0 to finish): ";
    vector<shared_ptr<Transport>> selected;  
    //save multiple selected pointers to Transport, much safer
    while (true) {
        int idx;
        cin >> idx;
        if (idx == 0) break;
        if (idx >= 1 && idx <= matched.size())
        {
            if (!matched[idx-1]->isAvailable()) {
                cout << "This transport is already booked."
                << "Please choose another one." << endl;
                continue;
            }
            if (find(selected.begin(), selected.end(),
                     matched[idx - 1]) != selected.end()) {
                cout << "You have already selected this transport." << endl;
                cout << "\nEnter indices of transports to book (0 to finish): ";
                continue;
            }
                selected.push_back(matched[idx - 1]);  //add into selected
                cout << selected.size() << " transport(s) selected." << endl;
        }
        else
            cout << "Invalid index." << endl;
            cout << "Enter indices of transports to book (0 to finish): ";
    }

    if (!selected.empty()) //if selected
    {
        ts.recordBooking(Client(name, contact), selected);
        ts.saveFleetToFile("fleet.txt");
        ts.saveBookingsToFile("bookings.txt");
        cout << "\nBooking confirmed.\nTransport IDs: ";
        for (auto& t : selected) cout << t->getID() << " ";
        cout << endl;
        cout<<"Name: ";
        for (auto& t : selected) cout << t->getName() << " ";
        cout << endl;
    }
}

void adminMenu(TransportSystem& ts)
{
    ts.reloadFleet();
    int choice;
    do {
        cout << "\n--- Admin Panel ---\n";
        cout << "1. Add Transport\n";
        cout << "2. Delete Transport\n";
        cout << "3. Modify Transport\n";
        cout << "4. View Fleet\n";
        cout << "0. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            ts.addNewTransport();
            ts.saveFleetToFile("fleet.txt");
        }
        else if (choice == 2)
        {
            string id;
            cout << "Enter transport ID to delete: ";
            cin >> id;
            ts.deleteTransport(id);
            ts.saveFleetToFile("fleet.txt");
        }
        else if (choice == 3)
        {
            string id;
            int p, w;
            cout << "Enter ID to modify: ";
            cin >> id;
            cout << "New max people: ";
            cin >> p;
            cout << "New max weight: ";
            cin >> w;
            ts.modifyTransport(id, p, w);
            ts.saveFleetToFile("fleet.txt");
        }
        else if (choice == 4)
        {
            ts.reloadFleet();
            ts.listFleet();
        }
    } while (choice != 0);
}
bool verifyAdmin() {
    string password;
    for (int i = 0; i < 3; ++i) {
        cout << "Enter admin password: ";
        cin >> password;
        if (password == "thisispasswd") return true;
        cout << "Incorrect. Try again." << endl;
    }
    cout << "Too many failed attempts. Returning to main menu.\n";
    return false;
}

int main() {
    rgb_init();
    TransportSystem ts;
    ts.reloadFleet();
    int identity;

    rgb_set(255,255,255,225,255,255);
    while (true) {
        cout << "\n==== Universal Transports ====\n";
        cout << "1. Customer\n" ;
        cout << "2. Administrator\n";
        cout << "0. Exit\n";
        //rgb_set(255,255,255,235,148,247);
        cout << "Please enter your identity: ";
        cin >> identity;

        if (identity == 0)
        {
            cout<<"Exiting...\n";
            break;
        }
        else if (identity == 1) customerMenu(ts);
        else if (identity == 2) {
        if (verifyAdmin()) adminMenu(ts);
        }

        else cout << "Invalid input, try again." << endl;
    }

    return 0;
}

