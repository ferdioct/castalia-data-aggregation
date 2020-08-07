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

#include "SimpleAggregation.h"

Define_Module(SimpleAggregation);


// inisialisasi
void SimpleAggregation::startup()
{
	sampleInterval = (double)par("sampleInterval") / 1000; // dalam satuan detik
	aggregatedValue = 0.0; // untuk menampung nilai data yang sudah diaggregasikan
	waitingTimeForLowerLevelData = 0.0;
	lastSensedValue = 0.0;// nilai terakhir yang dideteksi oleh sensor
	totalPackets = 0;// menampung nilai seberapa banyaknya paket yang berhasil dikirim
	setTimer(REQUEST_SAMPLE, 0);// set waktu untuk meminta sensor mengirim data
	setTimer(SEND_AGGREGATED_VALUE,0);// set waktu untuk meminta sensor mengirim data yang sudah diagregasikan
}

/*
 * Saat timer sudah mencapai waktu yang sudah ditentukan sebelumnya
 * maka akan terjadi sebuah event. Index event yang terjadi akan sesuai
 * dengan index timer.
*/
void SimpleAggregation::timerFiredCallback(int index)
{
	switch (index) {
		
		case REQUEST_SAMPLE:{
			requestSensorReading();// meminta hasil sensing dari sensor
			setTimer(REQUEST_SAMPLE, sampleInterval);// timer akan diset ulang
			break;
		}
		
		case SEND_AGGREGATED_VALUE:{
		    // mengirim hasil aggregasi ke network layer dengan address sink
			toNetworkLayer(createGenericDataPacket(lastSensedValue, totalPackets), SINK_NETWORK_ADDRESS);
			totalPackets = totalPackets + 1;
			setTimer(SEND_AGGREGATED_VALUE, sampleInterval);
			break;
		}
	}
}

/*
 * Method untuk menganani packet yang dikirimkan ke network layer
 *
*/
void SimpleAggregation::fromNetworkLayer(ApplicationPacket * rcvPacket, 
		const char *source, double rssi, double lqi)
{
	double theData = rcvPacket->getData();
	aggregatedValue = (aggregatedValue + lastSensedValue);
	if (isSink)
		trace() << "from " << source << " received value " << theData;
	    aggregatedValue = totalValue/totalPackets;
	    trace() << "The aggregated value is: " << aggregatedValue;
	    trace() << "Total packets sent is: " << totalPackets;
}

/*
 * Method ini akan menangani data yang dikirim oleh sensor
*/
void SimpleAggregation::handleSensorReading(SensorReadingMessage * rcvReading)
{
	string sensType(rcvReading->getSensorType());
	double sensValue = rcvReading->getSensedValue();
	lastSensedValue = sensValue;
	totalValue += sensValue;
}

void SimpleAggregation::finishSpesific(){
    aggregatedValue = aggregatedValue/totalPackets;
    trace() << "The aggregated value is: " << aggregatedValue;
    trace() << "Total packets sent is: " << totalPackets;
}


/*
void SimpleAggregation::handleNeworkControlMessage(cMessage * msg)
{
/*
    switch(msg->getKind()) {
    
	case TREE_LEVEL_UPDATED: {
	    // this message notifies the application of routing state (level)
	    // for certain routing protocols.
	    Network_ControlMessage levelMsg = check_and_cast<Network_ControlMessage >(msg);
	    routingLevel = levelMsg->getLevel();

	    waitingTimeForLowerLevelData = sampleInterval/pow((double)2,routingLevel);
	    trace() << "Routing level " << routingLevel;
	
	    setTimer(SEND_AGGREGATED_VALUE,waitingTimeForLowerLevelData);
	    break;
	}

	case CONNECTED_TO_TREE: {
	    Network_ControlMessage *connectedMsg = check_and_cast<Network_ControlMessage *>(msg);

	    int treeLevel = connectedMsg->getLevel();
	    string parents;
	    parents.assign(connectedMsg->getParents());

	    trace() << "Tree level " << treeLevel;

	    routingLevel = treeLevel;

	    waitingTimeForLowerLevelData = sampleInterval/pow((double)2,routingLevel);
	    setTimer(SEND_AGGREGATED_VALUE,waitingTimeForLowerLevelData);
	    break;
	}
    }
}
*/


