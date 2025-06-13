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
void rgb_set(int wr, int wg, int wb, int br = 0, int bg = 0, int bb = 0){
    if (br + bg + bb == 0)
        printf("\033[38;2;%d;%d;%dm", wr, wg, wb); // 仅字体
    else
        printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm", wr, wg, wb, br, bg, bb); // 字体+背景
}

void reset_color() {
    printf("\033[0m");
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
        MessageBox(NULL, "No suitable transport found.", "Match Failed", MB_OK | MB_ICONWARNING);
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
                MessageBox(NULL, "You have already selected this transport.", "Duplicate Selection", MB_OK | MB_ICONWARNING);
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
        MessageBox(NULL, "Booking confirmed!", "Success", MB_OK | MB_ICONINFORMATION);
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
        rgb_set(0, 50, 100, 200, 230, 255); // 字深蓝，背景浅蓝
        cout << "\n--- Admin Panel ---\n";
        reset_color();

        // 菜单选项字体不同颜色
        rgb_set(60, 179, 113);  cout << "1. Add Transport\n";       reset_color();
        rgb_set(255, 140, 0);   cout << "2. Delete Transport\n";    reset_color();
        rgb_set(138, 43, 226);  cout << "3. Modify Transport\n";    reset_color();
        rgb_set(0, 206, 209);   cout << "4. View Fleet\n";          reset_color();
        rgb_set(220, 20, 60);   cout << "0. Back to Main Menu\n";   reset_color();

        rgb_set(0, 128, 0);
        cout << "Choice: ";
        reset_color();
        cin >> choice;

        if (choice == 1) {
            ts.addNewTransport();
            ts.saveFleetToFile("fleet.txt");
            MessageBox(NULL, "Transport added successfully.", "Success", MB_OK | MB_ICONINFORMATION);
        }
        else if (choice == 2) {
            string id;
            rgb_set(0, 128, 0); cout << "Enter transport ID to delete: "; reset_color();
            cin >> id;
            ts.deleteTransport(id);
            ts.saveFleetToFile("fleet.txt");
        }
        else if (choice == 3) {
            string id;
            int p, w;
            rgb_set(0, 128, 0); cout << "Enter ID to modify: "; reset_color();
            cin >> id;
            cout << "New max people: ";
            cin >> p;
            cout << "New max weight: ";
            cin >> w;
            ts.modifyTransport(id, p, w);
            ts.saveFleetToFile("fleet.txt");
        }
        else if (choice == 4) {
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
        MessageBox(NULL, "Incorrect password. Try again.", "Authentication Failed", MB_OK | MB_ICONERROR);
    }
    MessageBox(NULL, "Too many failed attempts. Returning to main menu.", "Access Denied", MB_OK | MB_ICONWARNING);
    return false;
}

int main() {
    rgb_init();
    TransportSystem ts;
    ts.reloadFleet();
    int identity;

    while (true) {
        rgb_set(0, 50, 100, 200, 230, 255); // 蓝底浅蓝字
        cout << "\n==== Universal Transports ====\n";
        reset_color();

        rgb_set(255, 105, 180);  cout << "1. Customer\n";        reset_color();
        rgb_set(30, 144, 255);   cout << "2. Administrator\n";   reset_color();
        rgb_set(255, 69, 0);     cout << "0. Exit\n";            reset_color();

        rgb_set(0, 128, 0); // 输入提示绿色
        cout << "Please enter your identity: ";
        reset_color();
        cin >> identity;

        if (identity == 0) {
            rgb_set(0, 128, 0); cout << "Exiting...\n"; reset_color();
            break;
        }
        else if (identity == 1) customerMenu(ts);
        else if (identity == 2) {
            if (verifyAdmin()) adminMenu(ts);
        }
        else {
            rgb_set(200, 0, 0);
            MessageBox(NULL, "Invalid input, try again.", "Error", MB_OK | MB_ICONERROR);
            reset_color();
        }
    }

    return 0;
}





