
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

static unsigned char CRC8(const unsigned char* data, unsigned short len) {
	unsigned char crc = 0x00;
	while (len--) {
		unsigned char extract = *data++;
		for (unsigned char tempI = 8; tempI; tempI--) {
			unsigned char sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum) {
				crc ^= 0x8C;
			}
			extract >>= 1;
		}
	}
	return crc;
}

static int memsearch(const char *hay, int haysize, const char *needle, int needlesize) {
	int haypos, needlepos;
	haysize -= needlesize;
	for (haypos = 0; haypos <= haysize; haypos++) {
		for (needlepos = 0; needlepos < needlesize; needlepos++) {
			if (hay[haypos + needlepos] != needle[needlepos]) {
				break;
			}
		}
		if (needlepos == needlesize) {
			return haypos;
		}
	}
	return -1;
}

#define PACKET_HEADER "EXTRBCRE"
//#define PULLER_HEADER "EXTRBPUL"
//#define WINDER_HEADER "EXTRBWDR"									
//#define EXTRUDER_HEADER "EXTRBEXT"

#define MAX_PAYLOAD_SIZE 512
#define RECEIVE_BUFFER_SIZE 512

enum PacketClassType : unsigned short
{
	COMMAND = 0,
	STATUS,
//	EXT_PROCESS,
//	PUL_PROCESS,
//	WND_PROCESS
};

template<typename T>
class DataPacket {
public:
	DataPacket() : packet() {
		this->packetSize = sizeof(T);
	}
	virtual ~DataPacket() {}
	void toBytes(unsigned char* data, unsigned int charSize = sizeof(T)) {
		if (charSize < packetSize)return;
		memcpy(data, &packet.bytes, packetSize);
	}
	void fromBytes(unsigned char* data, unsigned int charSize = sizeof(T)) {
		if (charSize < packetSize)return;
		memcpy(&packet.bytes, data, packetSize);
	}
	union {
		T				data;
		unsigned char	bytes[sizeof(T)];
	} packet;
	unsigned int packetSize;
};

struct HeaderDataPart {
	char				name[8];
	PacketClassType		classType;
	unsigned short		payloadSize;
};

template<typename T>
struct CompletePacket {
	HeaderDataPart		header;
	T					payload;
	unsigned char		checksum;
};

template<typename T>
class CompleteDataPacket : public DataPacket <CompletePacket<T>> {
public:
	CompleteDataPacket() : DataPacket<CompletePacket<T>>() {	}
	CompleteDataPacket(const char* header) {
		CompleteDataPacket();
		for (unsigned int i = 0; i < 8; i++) {
			this->packet.data.header.name[i] = header[i];
		}
	}
	CompleteDataPacket(const char* headerName, PacketClassType classType, unsigned short payloadSize) : CompleteDataPacket(headerName) {
		this->packet.data.header.classType = classType;
		this->packet.data.header.payloadSize = payloadSize;
	}
	void calculateChecksum() {
		this->packet.data.checksum = CRC8((const unsigned char*)&this->packet.data.payload, this->packet.data.header.payloadSize);
	}
	void setPayload(T data) {
		this->packet.data.payload = data;
	}
	T& getPayload() {
		return this->packet.data.payload;
	}
};

class DataPacketReceiver
{
public:
	DataPacketReceiver() : receiveIndex(0),
		headerData(),
		headerStartIndex(0)
	{
		headerSize = sizeof(HeaderDataPart);
		_reset();
	}
	~DataPacketReceiver() {}

	void readPacketByte(unsigned char c) {
		receiveBuffer[receiveIndex] = c;
		//cout <<"Buff["<<receiveIndex<<"] "<<c<<":"<< receiveBuffer << endl;
		//cout << "Read: "<<state<<" = " << int(receiveBuffer[receiveIndex])<<"("<< receiveBuffer[receiveIndex] <<")" <<" / recindex:"<< receiveIndex<<", headerStart:"<< headerStartIndex << endl;
		switch (state) {
		case WAITING_HEADER_START:
			if (_checkHeaderStart()) {
				state = WAITING_HEADER_END;
			}
			break;
		case WAITING_HEADER_END:
			if (_checkHeaderEnd()) {
				state = RECEIVING_DATA;
				//cout << headerData.packet.data.name << " cls:" << headerData.packet.data.classType << " payload size: " << headerData.packet.data.payloadSize << endl;
				receiveIndex = 0;
				return;
			}
			break;
		case RECEIVING_DATA:
			if (_recvData()) {
				state = CHECK_DATA_INTEGRITY;
			}
			break;
		case CHECK_DATA_INTEGRITY:
			unsigned char crc = CRC8((const unsigned char*)&receiveBuffer, headerData.packet.data.payloadSize);
			//cout << "CRC rec:" << (int)receiveBuffer[receiveIndex]<<" calc:"<<(int)crc << endl;
			if (crc == receiveBuffer[receiveIndex]) {
				OnReceivePacket(headerData.packet.data.classType, receiveBuffer, receiveIndex);
			}
			_reset();
			break;
		};
		receiveIndex = (receiveIndex + 1) % RECEIVE_BUFFER_SIZE;
	}
	virtual void OnReceivePacket(PacketClassType classType, unsigned char* buffer, unsigned short size) {	}

private:
	enum State {
		WAITING_HEADER_START = 0,
		WAITING_HEADER_END,
		RECEIVING_DATA,
		CHECK_DATA_INTEGRITY
	};
	void _reset() {
		state = WAITING_HEADER_START;
		receiveIndex = 0;
		headerStartIndex = 0;
		memset(&receiveBuffer[0], 0, RECEIVE_BUFFER_SIZE);
	}
	bool _checkHeaderStart() {
		int found = memsearch((char*)receiveBuffer, 512, PACKET_HEADER, 8);
		if (found >= 0) {
			headerStartIndex = found;
			return true;
		}
		return false;
	}
	bool _checkHeaderEnd() {
		if (receiveIndex - headerStartIndex + 1 == headerSize) {
			headerData.fromBytes(&receiveBuffer[headerStartIndex], headerSize);
			if (headerData.packet.data.payloadSize < (unsigned short)MAX_PAYLOAD_SIZE) {
				return true;
			}
			else _reset();
		}//else check for error
		return false;
	}
	bool _recvData() {
		if (receiveIndex + 1 >= headerData.packet.data.payloadSize) {
			return true;
		}
		return false;
	}

	State						state;
	unsigned char				receiveBuffer[RECEIVE_BUFFER_SIZE];
	DataPacket<HeaderDataPart>	headerData;
	unsigned short				receiveIndex;
	unsigned int				headerStartIndex;
	unsigned short				headerSize;
};
