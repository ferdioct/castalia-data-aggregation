#include "DataAggregation.h"

Define_Module(DataAggregation);


// inisialisasi
void DataAggregation::startup()
{
	sampleInterval = (double)par("sampleInterval") / 1000; // dalam satuan detik
	recipientAddress = par("nextRecipient").stringValue(); // alamat penerima paket
	recipientId = atoi(recipientAddress.c_str()); // ID penerima paket
	lastSensedValue = 0;// nilai terakhir yang dideteksi oleh sensor
	totalPackets = 0;// menampung nilai seberapa banyaknya paket yang berhasil dikirim
	setTimer(REQUEST_SAMPLE, 0);// set waktu untuk meminta sensor mengirim data
}

/*Method untuk menangani event
 */
void DataAggregation::timerFiredCallback(int index)
{
    //minta sensor untuk sensing
	switch (index) {
		case REQUEST_SAMPLE:{
			requestSensorReading();// meminta hasil sensing dari sensor
			break;
		}
	}
}


/* Method untuk menganani packet yang diterima dari network layer
 */
void DataAggregation::fromNetworkLayer(ApplicationPacket * rcvPacket,
		const char *source, double rssi, double lqi)
{
	double temperature = rcvPacket->getData();
	int humid = rcvPacket->getData2();
	if (isSink){
		trace() << "From CH " << source << " received value Temperature :" << temperature << " Degrees, Humidity : " << humid << "%";
		total++;
	}
}

/*
 * Method ini akan menangani data yang dikirim oleh sensor
*/
void DataAggregation::handleSensorReading(SensorReadingMessage * rcvReading)
{
	double temperature = rcvReading->getSensedValue();
	int humid = rcvReading->getSensedValue2();
	toNetworkLayer(createGenericDataPacket2(temperature, humid, totalPackets), recipientAddress.c_str());
	totalPackets++;
    setTimer(REQUEST_SAMPLE, sampleInterval);// timer akan diset ulang
}

void DataAggregation::finishSpecific(){
    if (isSink){
            trace() << "Total packets received from CH :" << total;
    }
}
