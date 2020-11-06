//
// Created by amfur on 10/21/2020.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "../unp.h"
#include "gremlin.h"
#include <cstdint>
#include <unistd.h>
#include <socket.h>

using namespace std;

void sendPacket(); //TODO write this
void makePacket(uint8_t packet[], uint16_t packetNumber, vector<uint8_t> data);
void put(ifstream& inputFile, float damage, float drop);
ifstream openFile(string fileName);

int main (int argc, char *argv[]) {

	float damage;
	float drop;

	//arguments expected [filename, damageChance, dropChance]
	if (argc != 3) {
	    exit(3);
	}

	damage = stof(argv[1]);
	drop = stof(argv[1]);

	string input;
    cout << "enter file name";
    cin >> input;
    ifstream inputFile = openFile(input);

    put(inputFile, damage, drop);


    return 0;
}

ifstream openFile(string fileName) {
    ifstream inputFile;
    inputFile.open (fileName, ios::in);

    if (!inputFile.is_open()) {
        cout << "Unable to open file, exiting";
        exit(1);
    }
    else {
        cout << "file opened successfully.\n";
    }
    return inputFile;
}

void put(ifstream& inputFile, float damage, float drop){
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

    uint16_t checksumValue = checksum(packet, 128);
    uint8_t checkSum1 = (checksumValue >> (8*0)) & 0xff;
    uint8_t checkSum2 = (checksumValue >> (8*1)) & 0xff;
}

void sendPacket(uint8_t packet[]) {
    int sd;
    struct sockaddr_in server;

    sd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVPORT);

    for (;;) {
        sendto(sd, packet, 128, 0, (struct sockaddr *) &server, sizeof(server));
        sleep(2);
    }
    close(sd);
}