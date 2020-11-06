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
    uint8_t *packet[128];
    uint16_t expectedPacketNumber = 0;

    ofstream outfile;
    outfile.open("output.txt", ios::binary | ios::out);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(SERVPORT);

    int sd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(sd, (struct sockaddr *) &server, sizeof(server));

    // waiting for start request "PUT filename"
    for (;;) {

        n = recv(sd, startRequest, sizeof(startRequest), 0);

        if (startRequest[0] != '\0') {
            break;
        }
    }

    // receiving packet loop
    for (;;) {

        n = recv(sd, packet, sizeof(packet), 0);

        uint8_t firstByte = *packet[0];
        uint8_t secondByte = *packet[1];
        uint8_t thirdByte = *packet[2];
        uint8_t fourthByte = *packet[3];

        uint16_t packetNumber = (uint16_t)(secondByte << (uint8_t) 8) | firstByte;
        uint16_t hash = (uint16_t)(fourthByte << (uint8_t) 8) | thirdByte;
        uint8_t *data = packet[4];

        if (n == 1) {
            break;
        }

        if (packetNumber != expectedPacketNumber) {
            cout << "There is a packet missing." << endl;
        }

        if (!(isThereError(hash, data))) {
            cout << "There is no error." << endl;
        }
        else {
            cout << "There is an error." << endl;
        }

        outfile.write((char*)data, sizeof(data));

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