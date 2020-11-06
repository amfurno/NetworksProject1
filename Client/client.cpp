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
void makePacket(uint8_t packet[], uint16_t packetNumber, vector<uint8_t> data);

int main (int argc, char *argv[]) {
//arguments expected [filename, errorChance]
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
            if (gremlin()) {//TODO add proper function call
                sendPacket();
            }
            packetNumber++;
            packetData.clear();
        }
    }
    if (!packetData.empty()) {
        packetData.resize(124, NULL);
        makePacket();
        if (gremlin()) {//TODO add proper function call
            sendPacket();
        }
    }
    //TODO send 1 Byte packet



    return 0;
}

void makePacket(uint8_t packet[], uint16_t packetNumber, vector<uint8_t> data) {
    int packetLocation = 4;
    for (int i = 0; i < data.size(); i++) {
        packet[packetLocation] = data[i];
    }

    uint8_t packNum1 = (packetNumber >> (8*0)) & 0xff;
    uint8_t packNum2 = (packetNumber >> (8*1)) & 0xff;
    packet[0] = packNum1;
    packet[1] = packNum2;

    uint16_t checksumValue = checksum(packet);
    uint8_t checkSum1 = (checksumValue >> (8*0)) & 0xff;
    uint8_t checkSum2 = (checksumValue >> (8*1)) & 0xff;
}