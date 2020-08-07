/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Dimosthenis Pediaditakis                 *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *  
 ****************************************************************************/

#ifndef _DATAAGGREGATION_H_
#define _DATAAGGREGATION_H_

#include "VirtualApplication.h"
#include "ApplicationPacket_m.h"
#include <ctime>

using namespace std;


enum DataAggregationTimers {
	REQUEST_SAMPLE = 1
};

class DataAggregation: public VirtualApplication {
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

#endif				// _DATAAGGREGATION_APPLICATIONMODULE_H_
