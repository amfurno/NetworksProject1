//
// Created by amfur on 10/21/2020.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "../unp.h"
#include "gremlin.h"
#include <cstdint>

using namespace std;

void sendPacket(); //TODO write this
void makePacket(uint8_t packet[], uint16_t packetNumber, const vector<uint8_t>& data);

int main (int argc, char *argv[]) {

	float damage;
	float drop;

	//arguments expected [filename, damageChance, dropChance]
    ifstream inputFile;
    inputFile.open (/*innput file*/, ios::in);

    if (!inputFile.is_open()) {
        cout << "Unable to open file, exiting";
        exit(1);
    }
    else {
        cout << "file opened successfully.\n";
    }

    uint8_t ch;
    vector<uint8_t> packetData;
    uint8_t packet[128] = {0};
    uint16_t packetNumber = 0;

    while (inputFile >> noskipws >> ch) {
        packetData.push_back(ch);
        if (packetData.size() == 124) {
            makePacket(packet, packetNumber, packetData);
            if (gremlin(packet, damage, drop)) {
                sendPacket();
            }
            packetNumber++;
            packetData.clear();
        }
    }
    if (!packetData.empty()) {
        packetData.resize(124, NULL);
        makePacket(packet, packetNumber, packetData);
        if (gremlin(packet, damage, drop)) {
            sendPacket();
        }
    }
    //TODO send 1 Byte packet



    return 0;
}

void makePacket(uint8_t packet[], uint16_t packetNumber, const vector<uint8_t>& data) {
    int packetLocation = 4;
    for (unsigned char i : data) {
        packet[packetLocation] = i;
    }

    uint8_t packNum1 = (uint16_t) (packetNumber >> (uint8_t) (8*0)) & (uint8_t) 0xff;
    uint8_t packNum2 = (uint16_t) (packetNumber >> (uint8_t) (8*1)) & (uint8_t) 0xff;
    packet[0] = packNum1;
    packet[1] = packNum2;

    uint16_t checksumValue = checksum(packet, 128);
    uint8_t checkSum1 = (uint16_t) (checksumValue >> (uint8_t) (8*0)) & (uint8_t) 0xff;
    uint8_t checkSum2 = (uint16_t) (checksumValue >> (uint8_t) (8*1)) & (uint8_t) 0xff;
}