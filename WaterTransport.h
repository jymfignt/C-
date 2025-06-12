//WaterTransport.h
#ifndef WATERTRANSPORT_H_INCLUDED
#define WATERTRANSPORT_H_INCLUDED

#include "Transport.h"
class WaterTransport : public Transport {
public:
    WaterTransport(string id, string name, int p, int w) {
        setID(id); setName(name);  setCategory("water"); setPeople(p); setWeight(w);
    }

    string getType() const override { return "water"; }
    double getFuel() const override { return 0.5; }
    double getSpeed() const override { return 40.0; }
};
#endif // WATERTRANSPORT_H_INCLUDED

