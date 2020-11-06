//
// Created by amfur on 10/21/2020.
//

#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdint>
#include <unistd.h>
#include "../unp.h"
#include <fstream>

using namespace std;

bool isThereError(uint16_t expectedHash, uint8_t *data);

int main () {
    int n;
    struct sockaddr_in server;
    char startRequest[128];
    uint8_t packet[128];
    uint16_t expectedPacketNumber = 0;

    ofstream outfile;
    outfile.open("../output.txt", ios::out);
    if (outfile.is_open() and outfile.good())
    	cout << "file opened correctly" << endl;



    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVPORT);

    int sd = socket(AF_INET, SOCK_DGRAM, 0);

    if (bind(sd, (struct sockaddr *) &server, sizeof(server)) < 0)
    	cerr << "error binding socket" << endl;

    listen(sd, 5);
    // waiting for start request "PUT filename"
    for (;;) {

        n = recv(sd, startRequest, sizeof(startRequest), 0);

        if (startRequest[0] != '\0') {
        	cout << "received PUT" << endl;
            break;
        }
    }

    // receiving packet loop
    for (;;) {

        n = recv(sd, packet, sizeof(packet), 0);

        uint8_t firstByte = packet[0];
        uint8_t secondByte = packet[1];
        uint8_t thirdByte = packet[2];
        uint8_t fourthByte = packet[3];

        uint16_t packetNumber = (uint16_t)(secondByte << (uint8_t) 8) | firstByte;
        uint16_t hash = (uint16_t)(fourthByte << (uint8_t) 8) | thirdByte;
        uint8_t *data = &packet[4];

        if (n == 1) {
            break;
        }

        if (packetNumber != expectedPacketNumber) {
            cout << "There is a packet missing: " << expectedPacketNumber << endl;
            expectedPacketNumber = packetNumber;
        }

        if (!(isThereError(hash, data))) {
        	cout << "There is no error: " << packetNumber << endl;
			for (int i = 4; i < 52; i++) {
				cout << (char) packet[i];
			}
        }
        else {
            cout << "There is an error: " << packetNumber << endl;
        }

        for (int i = 4; i < n; i++) {
        	cout << (char) packet[i];
			if (packet[i] != 0)
        		outfile << (char) packet[i];
		}
        //outfile.write((char*)data, sizeof(data));

        expectedPacketNumber++;
    }

    // sending final PUT message and closing the socket and file
    sendto(sd, "PUT successfully completed", 26, 0, (struct sockaddr *) &server, sizeof(server));

    close(sd);

    outfile.close();

    return 0;
}

bool isThereError(uint16_t expectedHash, uint8_t *data) {

    uint16_t hash = checksum(data, 124);

    if (hash == expectedHash) {
        return false;
    }
    return true;
}