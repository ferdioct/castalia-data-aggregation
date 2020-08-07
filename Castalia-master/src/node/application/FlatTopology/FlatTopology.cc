#include "FlatTopology.h"

Define_Module(FlatTopology);


// inisialisasi
void FlatTopology::startup()
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
void FlatTopology::timerFiredCallback(int index)
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
void FlatTopology::fromNetworkLayer(ApplicationPacket * rcvPacket,
        const char *source, double rssi, double lqi)
{
    double temperature = rcvPacket->getData();
    int humid = rcvPacket->getData2();
    if (isSink){
        trace() << "From " << source << " received value Temperature :" << temperature << " Degrees, Humidity : " << humid << "%";
        total++;
    }
}

/*
 * Method ini akan menangani data yang dikirim oleh sensor
*/
void FlatTopology::handleSensorReading(SensorReadingMessage * rcvReading)
{
    double temperature = rcvReading->getSensedValue();
    int humid = rcvReading->getSensedValue2();
    toNetworkLayer(createGenericDataPacket2(temperature, humid, totalPackets), SINK_NETWORK_ADDRESS);
    totalPackets++;
    setTimer(REQUEST_SAMPLE, sampleInterval);// timer akan diset ulang
}

void FlatTopology::finishSpecific(){
    if (isSink){
            trace() << "Total packets received from sensor :" << total;
    }
}
