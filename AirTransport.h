//AirTransport.h
#ifndef AIRTRANSPORT_H_INCLUDED
#define AIRTRANSPORT_H_INCLUDED

#include "Transport.h"
class AirTransport : public Transport {
public:
    AirTransport(string id, string name, int p, int w) {
        setID(id); setName(name); setCategory("air"); setPeople(p); setWeight(w);
    }

    string getType() const override { return "air"; }
    double getFuel() const override { return 2.5; }
    double getSpeed() const override { return 900.0; }
};

#endif // AIRTRANSPORT_H_INCLUDED

