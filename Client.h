//Client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
using namespace std;

class Client {
public:
    string name;
    string contact;
    Client()=default;
    Client(string n, string c) : name(n), contact(c) {}
};
#endif // CLIENT_H
