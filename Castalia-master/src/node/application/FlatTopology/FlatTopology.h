#ifndef _FLATTOPOLOGY_H_
#define _FLATTOPOLOGY_H_

#include "VirtualApplication.h"
#include "ApplicationPacket_m.h"
#include <ctime>

using namespace std;


enum FlatTopologyTimers {
    REQUEST_SAMPLE = 1
};

class FlatTopology: public VirtualApplication {
 private:
    double lastSensedValue;
    double sampleInterval;
    int totalPackets;
    int recipientId;
    string recipientAddress;
    int total;

 protected:
    void startup();
    void finishSpecific();
    void timerFiredCallback(int);
    void handleSensorReading(SensorReadingMessage *);
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
};

#endif              // _FLATTOPOLOGY_APPLICATIONMODULE_H_
