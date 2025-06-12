//LandTransport.h
#ifndef LANDTRANSPORT_H_INCLUDED
#define LANDTRANSPORT_H_INCLUDED

#include "Transport.h"

class LandTransport : public Transport {
public:
    LandTransport(string id, string name, int p, int w) {
        setID(id); setName(name);  setCategory("land"); setPeople(p); setWeight(w);
    }

    string getType() const override { return "land"; }
    double getFuel() const override { return 0.2; }
    double getSpeed() const override { return 80.0; }
};

#endif // LANDTRANSPORT_H_INCLUDED
