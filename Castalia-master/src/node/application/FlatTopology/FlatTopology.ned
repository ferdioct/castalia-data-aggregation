package node.application.FlatTopology;

simple FlatTopology like node.application.iApplication {
 parameters:
 	string applicationID = default ("flatTopology");
	bool collectTraceInfo = default (true);
	int priority = default (1);
	int packetHeaderOverhead = default (8);	// in bytes
	int constantDataPayload = default (2);	// in bytes
	int sampleInterval = default (1000);	// in ms
	bool isSink = default (false);
	string nextRecipient = default ("0");

 gates:
 	output toCommunicationModule;
	output toSensorDeviceManager;
	input fromCommunicationModule;
	input fromSensorDeviceManager;
	input fromResourceManager;
}